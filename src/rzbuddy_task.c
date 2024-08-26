/*
 * rzbuddy_task.c
 *
 *  Created on: Aug 12, 2024
 *      Author: Lucas
 */

#include "FreeRTOS.h"
#include "task.h"
#include "hal_data.h"

#include <stdint.h>

#include "rzbuddy_task.h"
#include "rzbuddy_common.h"

static StaticTask_t rzbuddy_thread_memory;
#if defined(__ARMCC_VERSION)           /* AC6 compiler */
static uint8_t rzbuddy_thread_stack[512] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#else
static uint8_t rzbuddy_thread_stack[512] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.rzbuddy_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif

const rm_freertos_port_parameters_t rzbuddy_thread_parameters =
{ .p_context = (void*) NULL, };

static const TickType_t SERVO_ROTATION_PERIOD_MS = 1500;
/** MIN_NUDGE_TIME_MS tuned arbitrarily */
static const TickType_t MIN_NUDGE_TIME_MS = 40;

static void rzbuddy_root_task(void * pvParameters);

void rzbuddy_thread_create(void)
{
    TaskHandle_t blinky_thread = xTaskCreateStatic (rzbuddy_root_task,
                                                    (const char*) "Rzbuddy Thread",
                                                    512 / 4, // In words, not bytes
                                                    (void*) &rzbuddy_thread_parameters, //pvParameters
                                                    1,
                                                    (StackType_t*) &rzbuddy_thread_stack,
                                                    (StaticTask_t*) &rzbuddy_thread_memory);

    assert(blinky_thread!=NULL);
}

static void rzbuddy_root_task(void *pvParameters)
{
    (void) pvParameters;

    R_GPT_Open (&g_timer_servo_ctrl, &g_timer_servo_cfg);

    fsp_err_t err = FSP_SUCCESS;
    gpio_handler_mapping_t const gpio_mappings[2] =
    {
    { .gpio_pin = RZBUDDY_FEED_GPIO,  .gpio_trigger = BSP_IO_LEVEL_HIGH, .handler = rzbuddy_dispense },
    { .gpio_pin = RZBUDDY_NUDGE_GPIO, .gpio_trigger = BSP_IO_LEVEL_LOW,  .handler = rzbuddy_pwm_nudge } // default low
    };

    handle_error (R_GPT_Open (&g_timer_servo_ctrl, &g_timer_servo_cfg));

    size_t n_mappings = sizeof(gpio_mappings) / sizeof(gpio_mappings[0]);
    while (1)
    {
        // Traverse the mappings and handle signals forever
        for (size_t i = 0; i < n_mappings; i++)
        {
            if (gpio_mappings[i].handler == NULL)
                continue;

            if (R_BSP_PinRead ((bsp_io_port_pin_t) gpio_mappings[i].gpio_pin) == gpio_mappings[i].gpio_trigger)
            {
                err = gpio_mappings[i].handler ();
                handle_error (err);
            }
        }
    }

}

fsp_err_t rzbuddy_dispense()
{
    fsp_err_t retval = FSP_SUCCESS;

    retval = R_GPT_Start (&g_timer_servo_ctrl);
    if (retval != FSP_SUCCESS)
        return retval;

    vTaskDelay (pdMS_TO_TICKS(SERVO_ROTATION_PERIOD_MS));

    retval = R_GPT_Stop (&g_timer_servo_ctrl);
    if (retval != FSP_SUCCESS)
        return retval;

    // Calling side will trigger a feed event, then have to modulate the LED to
    // get another feed event
    while (R_BSP_PinRead (RZBUDDY_FEED_GPIO) == BSP_IO_LEVEL_HIGH)
        ;

    return retval;
}

fsp_err_t rzbuddy_pwm_nudge()
{
    fsp_err_t retval = FSP_SUCCESS;

    /** starting timer because this function should only be called when nudge trigger happens */
    retval = R_GPT_Start (&g_timer_servo_ctrl);
    if (retval != FSP_SUCCESS)
        return retval;

    // user switch is low on press
    while (R_BSP_PinRead (RZBUDDY_NUDGE_GPIO) == BSP_IO_LEVEL_LOW)
    {
        vTaskDelay (pdMS_TO_TICKS(MIN_NUDGE_TIME_MS));
    }

    return R_GPT_Stop (&g_timer_servo_ctrl);
}

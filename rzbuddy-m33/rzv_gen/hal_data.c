/* generated HAL source file - do not edit */
#include "hal_data.h"

gpt_instance_ctrl_t g_timer_servo_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_servo_pwm_extend =
{
    .trough_ipl          = 4,
    .trough_irq          = GPT7_UNF_IRQn,
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_servo_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = 4, .capture_b_ipl = 4, .capture_a_irq =
                  GPT7_CCMPA_IRQn,
          .capture_b_irq = GPT7_CCMPB_IRQn, .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE, .capture_filter_gtiocb =
                  GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer_servo_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
        };
const timer_cfg_t g_timer_servo_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.02 seconds. Actual duty: 8.5%. */.period_counts = (uint32_t) 0x1e8480,
  .duty_cycle_counts = 0x29810, .source_div = (timer_source_div_t) 0, .channel = 7, .p_callback = NULL, .p_context =
          NULL,
  .p_extend = &g_timer_servo_extend, .cycle_end_ipl = 4, .cycle_end_irq = GPT7_OVF_IRQn, };
/* Instance structure to use this module. */
const timer_instance_t g_timer_servo =
{ .p_ctrl = &g_timer_servo_ctrl, .p_cfg = &g_timer_servo_cfg, .p_api = &g_timer_on_gpt };
gtm_instance_ctrl_t g_timer2_ctrl;
const gtm_extended_cfg_t g_timer2_extend =
{ .generate_interrupt_when_starts = GTM_GIWS_TYPE_DISABLED, .gtm_mode = GTM_TIMER_MODE_INTERVAL, };
const timer_cfg_t g_timer2_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period_counts = 99999 /* Actual period: 0.001 seconds. */, .channel = 2, .p_callback =
          NULL,
  .p_context = NULL, .cycle_end_ipl = 255, .p_extend = &g_timer2_extend,
#if defined(VECTOR_NUMBER_GTM2_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GTM2_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = GTM2_OSTM2INT_IRQn,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer2 =
{ .p_ctrl = &g_timer2_ctrl, .p_cfg = &g_timer2_cfg, .p_api = &g_timer_on_gtm };
extern void SysTick_Handler(timer_callback_args_t *p_args);
void vPortSetupTimerInterrupt(void) /* PRIVILEGED_FUNCTION */
{
    /* Stop and reset the SysTick. */
    g_timer2.p_api->open (g_timer2.p_ctrl, g_timer2.p_cfg);
    /* Configure SysTick to interrupt at the requested rate. */
    g_timer2.p_api->callbackSet (g_timer2.p_ctrl, SysTick_Handler, NULL, NULL);
    /* Start the SysTick. */
    g_timer2.p_api->start (g_timer2.p_ctrl);
#if (configUSE_TICKLESS_IDLE == 1)
 #error Set configUSE_TICKLESS_IDLE = 0. Tickless Idle is not supported
#endif                                 /* configUSE_TICKLESS_IDLE */

}
void vPortConfigureTimerForRunTimeStatus(void)
{
#if configGENERATE_RUN_TIME_STATS
    /* Stop and reset the Status Timer. */
    FSP_NOT_DEFINED.p_api->open(FSP_NOT_DEFINED.p_ctrl, FSP_NOT_DEFINED.p_cfg);
    /* Start the Status Timer. */
    FSP_NOT_DEFINED.p_api->start(FSP_NOT_DEFINED.p_ctrl);
#endif
}

/* Count clock = 100MHz
 10-bits-right-shift makes 10.24us counting interval */
#define runtimeCLOCK_SCALE_SHIFT    ( 10 )

/* GTM overflows 32bit */
#define runtimeOVERFLOW_SHIFT       ( 32 - runtimeCLOCK_SCALE_SHIFT )
unsigned long ulGetRunTimeCounterValue(void)
{
#if configGENERATE_RUN_TIME_STATS
    static unsigned long ulLastCounterValue = 0UL, ulOverflows = 0;
    unsigned long ulValueNow = 0;
    timer_status_t timer_status;

    /* Get count value */
    if (FSP_SUCCESS == FSP_NOT_DEFINED.p_api->statusGet(FSP_NOT_DEFINED.p_ctrl, &timer_status))
    {
        ulValueNow = timer_status.counter;
        
        /* Has the value overflowed since it was last read. */
        if( ulValueNow < ulLastCounterValue )
        {
            ulOverflows++;
        }
        ulLastCounterValue = ulValueNow;

        /* move the count value to  */
        ulValueNow >>= runtimeCLOCK_SCALE_SHIFT;
        ulValueNow += ulOverflows << runtimeOVERFLOW_SHIFT;
    }

    return ulValueNow;
#else
    return 0;
#endif
}
void g_hal_init(void)
{
    g_common_init ();
}

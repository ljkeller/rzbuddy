/*
 * rzbuddy_task.h
 *
 *  Created on: Aug 12, 2024
 *      Author: Lucas
 */

#ifndef RZBUDDY_TASK_H_
#define RZBUDDY_TASK_H_

#include "bsp_api.h"

void rzbuddy_thread_create(void);

typedef struct st_gpio_to_handler
{
    bsp_io_port_pin_t const gpio_pin;
    uint32_t const gpio_trigger;
    fsp_err_t (*const handler)();
} gpio_handler_mapping_t;

/** Trigger one rotation of the RZBuddy feeding mechanism */
fsp_err_t rzbuddy_dispense();

/** 'Nudge' the servo forward slowly while the user sends the appropriate signal. Useful
 *   for adjusting the feeding mechanism on RZBuddy
 */
fsp_err_t rzbuddy_pwm_nudge();

#endif /* RZBUDDY_TASK_H_ */

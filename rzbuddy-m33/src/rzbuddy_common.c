/*
 * rzbuddy_common.c
 *
 *  Created on: Aug 12, 2024
 *      Author: Lucas
 */

#include "rzbuddy_common.h"

const bsp_io_port_pin_t RZBUDDY_NUDGE_GPIO = BSP_IO_PORT_39_PIN_00; ///< USER Push Button Switch
const bsp_io_port_pin_t RZBUDDY_FEED_GPIO = BSP_IO_PORT_08_PIN_01;  ///< RED LED
const bsp_io_port_pin_t RZBUDDY_SERVO_GPIO = BSP_IO_PORT_15_PIN_01; ///< SERVO GPIO (32 on pi-hat)
const bsp_io_port_pin_t RZBUDDY_GREEN_GPIO = BSP_IO_PORT_17_PIN_02; ///< GREEN LED

void handle_error(fsp_err_t err)
{
    while (err != FSP_SUCCESS)
        ;
}

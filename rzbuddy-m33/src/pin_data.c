#include "bsp_api.h"
#include "r_ioport_api.h"

const ioport_pin_cfg_t g_bsp_pin_cfg_data[] =
{
  // Servo pin
  { .pin = BSP_IO_PORT_12_PIN_00, .pin_cfg = (  (uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                                              | (uint32_t) IOPORT_PERIPHERAL_MODE4
                                              | (uint32_t) IOPORT_CFG_DRIVE_B01
                                              | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST) },
  // LED pin
  { .pin = BSP_IO_PORT_08_PIN_01, .pin_cfg = (  (uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT_INPUT) }

};

const ioport_cfg_t g_bsp_pin_cfg =
{ .number_of_pins = sizeof(g_bsp_pin_cfg_data) / sizeof(ioport_pin_cfg_t), .p_pin_cfg_data = &g_bsp_pin_cfg_data[0], };

/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_gtm.h"
#include "FreeRTOS.h"
#include "r_timer_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_servo;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer_servo_ctrl;
extern const timer_cfg_t g_timer_servo_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** GTM Timer Instance */
extern const timer_instance_t g_timer2;

/** Access the GTM instance using these structures when calling API functions directly (::p_api is not used). */
extern gtm_instance_ctrl_t g_timer2_ctrl;
extern const timer_cfg_t g_timer2_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
extern void vPortSetupTimerInterrupt(void); /* PRIVILEGED_FUNCTION */
#define portGENERATED_GET_WAKEUP_IRQ
__STATIC_INLINE IRQn_Type vPortGetWakeUpIrq(void)
{
    return GTM2_OSTM2INT_IRQn;
}
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */

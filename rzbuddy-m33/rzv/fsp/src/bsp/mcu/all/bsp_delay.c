/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "bsp_delay.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_DELAY_NS_PER_SECOND    (1000000000)
#define BSP_DELAY_NS_PER_US        (1000)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 *              Delay for at least the specified duration in units and return. This function shouldn't be used to
 *              generate an accurate delay time. A running time of the function is depending on the region of memory
 *              where the function is excuted on and a frequency of the CPU clock.
 *
 *              The funcion calls bsp_prv_software_delay_loop() which loops over the requested delay time.
 *              One loop time through bsp_prv_software_delay_loop() takes roughly 500ns. This value is based on the
 *              actual measured time in the condition that a frequency of the CPU clock is 200MHz and the code is
 *              excuted on DDR3L-1333 DRAM(133MHz).
 *              One loop running time of software_delay_loop() can be modified by redefining BSP_DELAY_LOOP_CYCLES.
 *
 * @param[in]   delay  The number of 'units' to delay.
 * @param[in]   units  The 'base' (bsp_delay_units_t) for the units specified. Valid values are:
 *              BSP_DELAY_UNITS_SECONDS, BSP_DELAY_UNITS_MILLISECONDS, BSP_DELAY_UNITS_MICROSECONDS.@n
 *              For example:@n
 *              One run through bsp_prv_software_delay_loop() takes roughly 500ns.
 *              A delay of 100 us therefore requires 100000ns/500ns -> 200 loops.
 *
 *              The 'theoretical' maximum delay that may be obtained is determined by a full 32 bit loop count and the
 *              system clock rate.
 *              @200MHz:  ((0xFFFFFFFF loops * 500ns) = 2147 seconds.
 *
 *              Note that requests for very large delays will be affected by rounding in the calculations and the
 *              actual delay achieved may be slightly off of a setting time.
 *
 *              Note also that if the calculations result in a loop_cnt of zero, the bsp_prv_software_delay_loop()
 *              function is not called at all. In this case the requested delay is too small (nanoseconds) to be
 *              carried out by the loop itself, and the overhead associated with executing the code to just get to this
 *              point has certainly satisfied the requested delay.
 *
 * @note This function uses SystemCoreClock and therefore requires that the BSP has already initialized the variable
 *       (which it does as part of the SystemInit).
 *       Care should be taken to ensure this remains the case if in the future this function were to be called as part
 *       of the BSP initialization.
 **********************************************************************************************************************/

void R_BSP_SoftwareDelay (uint32_t delay, bsp_delay_units_t units)
{
    uint32_t iclk_hz;
    uint32_t cycles_requested;
    uint32_t ns_per_cycle;
    uint32_t loops_required = 0;
    uint32_t total_us       = (delay * units);
    uint64_t ns_64bits;

    iclk_hz      = SystemCoreClock;
    ns_per_cycle = BSP_DELAY_NS_PER_SECOND / iclk_hz;
    ns_64bits    = (uint64_t) total_us * (uint64_t) BSP_DELAY_NS_PER_US;

    /* Have we overflowed 32 bits? */
    if (ns_64bits <= UINT32_MAX)
    {
        /* No, we will not overflow. */
        cycles_requested = ((uint32_t) ns_64bits / ns_per_cycle);
        loops_required   = cycles_requested / BSP_DELAY_LOOP_CYCLES;
    }
    else
    {
        /* We did overflow. Try dividing down first. */
        total_us  = (total_us / (ns_per_cycle * BSP_DELAY_LOOP_CYCLES));
        ns_64bits = (uint64_t) total_us * (uint64_t) BSP_DELAY_NS_PER_US; // Convert to ns.

        /* Have we overflowed 32 bits? */
        if (ns_64bits <= UINT32_MAX)
        {
            /* No, we will not overflow. */
            loops_required = (uint32_t) ns_64bits;
        }
        else
        {
            /* We still overflowed, use the max count for cycles */
            loops_required = UINT32_MAX;
        }
    }

    /** Only delay if the supplied parameters constitute a delay. */
    if (loops_required > (uint32_t) 0)
    {
        bsp_prv_software_delay_loop(loops_required);
    }
}

/** @} (end addtogroup BSP_MCU) */

/*******************************************************************************************************************//**
 *        This assembly language routine takes roughly 500ns per one loop.
 *        The 'naked' attribute  indicates that the specified function does not need prologue/epilogue sequences
 *        generated by the compiler.
 * @param[in]     loop_cnt  The number of loops to iterate.
 **********************************************************************************************************************/
BSP_ATTRIBUTE_STACKLESS void bsp_prv_software_delay_loop (__attribute__((unused)) uint32_t loop_cnt)
{
    __asm volatile ("sw_delay_loop:         \n"

#if defined(__ICCARM__) || defined(__ARMCC_VERSION)
                    "   subs r0, #1         \n"
#elif defined(__GNUC__)
                    "   sub r0, r0, #1      \n"
#endif
                    "   cmp r0, #0          \n"

                    "   bne.n sw_delay_loop \n"

                    "   bx lr               \n");
}

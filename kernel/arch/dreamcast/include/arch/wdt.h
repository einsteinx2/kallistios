/* KallistiOS ##version##

   arch/dreamcast/include/wdt.h
   Copyright (c) 2023 Falco Girgis

*/

/** \file   arch/wdt.h
    \brief  Watchdog Timer API

    This file provides an API built around utilizing the SH4's watchdog timer.
    There are two different modes of operation which are supported:
        - watchdog mode: counter overflow causes a reset interrupt
        - interval timer mode: counter overflow causes a timer interrupt

    To start the WDT in watchdog mode, use wdt_enable_watchdog(). To use the 
    WDT as a general-purpose interval timer, use wdt_enable_timer().

    The timer can be stopped in either mode by calling wdt_disable_timer().

    \sa timer.h, wdt.h

    \author Falco Girgis
*/

#ifndef __ARCH_WDT_H
#define __ARCH_WDT_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <stdint.h>

/** \brief Clock divider settings 
 
    Denominators used to set the frequency divider
    for the input clock to the WDT.
 */
typedef enum WDT_CLK_DIV {
    WDT_CLK_DIV_32,     /** \brief Period: 41us */
    WDT_CLK_DIV_64,     /** \brief Period: 82us */
    WDT_CLK_DIV_128,    /** \brief Period: 164us */
    WDT_CLK_DIV_256,    /** \brief Period: 328us */
    WDT_CLK_DIV_512,    /** \brief Period: 656us */
    WDT_CLK_DIV_1024,   /** \brief Period: 1.31ms */
    WDT_CLK_DIV_2048,   /** \brief Period: 2.62ms */
    WDT_CLK_DIV_4096    /** \brief Period: 5.25ms */
} WDT_CLK_DIV;

/** \brief Reset signal type
 
    Specifies the kind of reset to be performed when the WDT
    overflows in watchdog mode.
*/
typedef enum WDT_RST {
    WDT_RST_POWER_ON,   /** \brief Power-On Resest */
    WDT_RST_MANUAL      /** \brief Manual Reset */
} WDT_RST;

/* \brief WDT interval timer callback function type */
typedef void (*wdt_callback)(void *user_data);

/** \brief  Enables the WDT as an interval timer

    Stops the WDT if it was previously running and reconfigures it 
    to be used as a generic interval timer, calling the given callback
    periodically at the requested interval (or as close to it as possible
    without calling it prematurely).

    \note 
    The internal resolution for each tick of the WDT in this mode is 
    41us, meaning a requested \p microsec_period of 100us will result
    in an actual callback interval of 123us.

    \warning
    \p callback is invoked within an interrupt context, meaning that 
    special care should be taken to not perform any logic requiring 
    additional interrupts. Data that is accessed from both within
    and outside of the callback should be atomic or protected by a 
    lock.

    \param  initial_count   Initial value of the WDT counter (Normally 0).
    \param  microsec_period Timer callback interval in microseconds
    \param  callback        User function to invoke periodically
    \param  user_data       Arbitrary user-provided data for the callback
*/
void wdt_enable_timer(uint8_t initial_count,
                      uint32_t microsec_period,
                      wdt_callback callback,
                      void *user_data);


void wdt_enable_watchdog(uint8_t initial_count,
                         WDT_CLK_DIV clk_config,
                         WDT_RST reset_select);

uint8_t wdt_get_counter(void);
void wdt_set_counter(uint8_t value);
void wdt_pet(void);

void wdt_disable(void);
int wdt_is_enabled(void);

__END_DECLS

#endif  /* __ARCH_WDT_H */


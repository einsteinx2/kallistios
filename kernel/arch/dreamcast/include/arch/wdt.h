/* KallistiOS ##version##

   arch/dreamcast/include/wdt.h
   Copyright (c) 2023 Falco Girgis

*/

/** \file   arch/wdt.h
    \brief  Watchdog timer API

    This file provides an API built around utilizing the SH4's watchdog timer.
    There are two different modes which are supported:
        - watchdog mode: counter overflow causes a reset interrupt
        - interval timer mode: counter overflow causes a timer interrupt

    \author Falco Girgis
*/

#ifndef __ARCH_WDT_H
#define __ARCH_WDT_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <stdint.h>

typedef enum WDT_CLK_DIV {
    WDT_CLK_DIV_32,
    WDT_CLK_DIV_64,
    WDT_CLK_DIV_128,
    WDT_CLK_DIV_256,
    WDT_CLK_DIV_512,
    WDT_CLK_DIV_1024,
    WDT_CLK_DIV_2048,
    WDT_CLK_DIV_4096
} WDT_CLK_DIV;

typedef enum WDT_RST {
    WDT_RST_POWER_ON,
    WDT_RST_MANUAL
} WDT_RST;

typedef void (*wdt_callback)(void *user_data);

void wdt_enable_timer(WDT_CLK_DIV clk_config,
                      uint8_t initial_count,
                      wdt_callback callback,
                      void *user_data);


void wdt_enable_watchdog(WDT_CLK_DIV clk_config,
                         uint8_t initial_count,
                         WDT_RST reset_select);

uint8_t wdt_get_counter(void);
void wdt_set_counter(uint8_t value);
void wdt_pet(void);

void wdt_disable(void);
int wdt_is_enabled(void);

__END_DECLS

#endif  /* __ARCH_WDT_H */


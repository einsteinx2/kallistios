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


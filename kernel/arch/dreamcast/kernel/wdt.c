#include <stdint.h>
#include <arch/wdt.h>
#include <arch/irq.h>
#include <stdio.h>

#define WDT_BASE        0xffc00008
#define WTCNT_HIGH      0x5a
#define WTCSR_HIGH      0xa5

#define WDT(o, t)       (*((volatile t *)(WDT_BASE + o)))
#define WDT_READ(o)     (WDT(o, uint8_t))
#define WDT_WRITE(o, v) (WDT(o, uint16_t) = ((o##_HIGH << 8) | ((v) & 0xff)))

#define WTCNT           0x0
#define WTCSR           0x4

#define WTCSR_TME       7
#define WTCSR_WTIT      6
#define WTCSR_RSTS      5
#define WTCSR_WOVF      4
#define WTCSR_IOVF      3
#define WTCSR_CKS2      2   
#define WTCSR_CKS1      1
#define WTCSR_CKS0      0

static void *user_data = NULL;
static wdt_callback callback = NULL;

static void wdt_isr(irq_t src, irq_context_t *cxt) {
    printf("WDT IRQ!!!!!!!");
    //fflush(stdout);
    callback(user_data);
}

void wdt_enable_timer(WDT_CLK_DIV clk_config,
                      uint8_t initial_count,
                      wdt_callback callback_,
                      void *user_data_) {
    WDT_WRITE(WTCSR, 0);

    callback = callback_;
    user_data = user_data_;
    irq_set_handler(EXC_WDT_ITI, wdt_isr);

    WDT_WRITE(WTCNT, initial_count);
    WDT_WRITE(WTCSR, (1 << WTCSR_TME) | clk_config);
}


void wdt_enable_watchdog(WDT_CLK_DIV clk_config,
                         uint8_t initial_count,
                         WDT_RST reset_select) {
    WDT_WRITE(WTCSR, (1 << WTCSR_WTIT));
    WDT_WRITE(WTCNT, initial_count);
    WDT_WRITE(WTCSR, (1 << WTCSR_TME) | (1 << WTCSR_WTIT) | 
                     (reset_select << WTCSR_RSTS) | clk_config);
}

void wdt_set_counter(uint8_t count) {
    WDT_WRITE(WTCNT, count);
}

uint8_t wdt_get_counter(void) {
    return WDT_READ(WTCNT);
}

void wdt_pet(void) {
    wdt_set_counter(0);
}

void wdt_disable(void) {
    WDT_WRITE(WTCSR, ~(1 << WTCSR_TME));
}

int wdt_is_enabled(void) {
    return WDT_READ(WTCSR) & (1 << WTCSR_TME);
}


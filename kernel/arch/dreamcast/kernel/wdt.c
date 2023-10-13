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

#define IPR_BASE        0xffd00004
#define IPR(o)          (*((volatile uint16_t *)(IPR_BASE + o)))
#define IPRA            0x0
#define IPRB            0x4
#define IPRC            0x8
#define IPRD            0xc

#define IPRB_WDT        12

#define WDT_CLK_DEFAULT WDT_CLK_DIV_32
#define WDT_INT_DEFAULT 41

static void *user_data = NULL;
static wdt_callback callback = NULL;
static uint32_t us_interval = 0;
static uint32_t us_elapsed = 0;

static void wdt_isr(irq_t src, irq_context_t *cxt) {
    (void)src;
    (void)cxt;

    us_elapsed += WDT_INT_DEFAULT;
    
    if(us_elapsed >= us_interval) { 
        callback(user_data);
        us_elapsed = 0;
    }

    WDT_WRITE(WTCSR, WDT_READ(WTCSR) & (~(1 << WTCSR_IOVF)));
}

void wdt_enable_timer(uint8_t initial_count,
                      uint32_t micro_seconds,
                      wdt_callback callback_,
                      void *user_data_) {
    

    /* Stop WDT, Enable Interval Timer, Set Clock Divisor */
    WDT_WRITE(WTCSR, WDT_CLK_DEFAULT);

    /* Store user callback data for later */
    callback = callback_;
    user_data = user_data_;
    us_elapsed = 0;
    us_interval = micro_seconds;

    /* Register our interrupt handler */
    irq_set_handler(EXC_WDT_ITI, wdt_isr);

    /* Unmask the WDTIT interrupt */
    IPR(IPRB) = IPR(IPRB) | (5 << IPRB_WDT);

    /* Reset the WDT counter */
    WDT_WRITE(WTCNT, initial_count);

    /* Write same configuration plus the enable bit set to start the WDT */
    WDT_WRITE(WTCSR, (1 << WTCSR_TME) | WDT_CLK_DEFAULT);
}


void wdt_enable_watchdog(uint8_t initial_count,
                         WDT_CLK_DIV clk_config,
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
    /* Mask the WDTIT interrupt */
    IPR(IPRB) = IPR(IPRB) & ~(7 << IPRB_WDT);

    WDT_WRITE(WTCSR, WDT_READ(WTCSR) & ~(1 << WTCSR_TME));

    wdt_pet();
}

int wdt_is_enabled(void) {
    return WDT_READ(WTCSR) & (1 << WTCSR_TME);
}

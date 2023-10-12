#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <kos.h>

static void quit(void) {
     printf("DISABLING WATCHDOG!\n");
     fflush(stdout); 
     wdt_disable();
     exit(EXIT_SUCCESS);
}

static atomic_bool triggered = false;

static void timer_callback(void* user_data) {
    triggered = true;
}

int main(int argc, char* argv[]) { 
    /* If the face buttons are all pressed, exit the app */
    cont_btn_callback(0, CONT_START | CONT_A | CONT_B | CONT_X | CONT_Y,
                      (cont_btn_callback_t)quit);

    wdt_enable_timer(WDT_CLK_DIV_4096,
                     0,
                     timer_callback,
                     "trololo random userdata");
    while(1) {
        if(triggered) {
            printf("WDT event fired!\n");
            triggered = false;
        }
    }

    return EXIT_SUCCESS;
}
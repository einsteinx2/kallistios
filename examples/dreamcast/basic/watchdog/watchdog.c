#include <stdlib.h>
#include <stdio.h>
#include <kos.h>

static void quit(void) {
     printf("DISABLING WATCHDOG!\n");
     fflush(stdout); 
     wdt_disable();
     exit(EXIT_SUCCESS);
}

int callbackFired = 0;

void timer_callback(void* user_data) {
     printf("USER TIMER CALLBACK!!!\n");
     callbackFired = !callbackFired;
}

int main(int argc, char* argv[]) { 
    /* If the face buttons are all pressed, exit the app */
    cont_btn_callback(0, CONT_START | CONT_A | CONT_B | CONT_X | CONT_Y,
                      (cont_btn_callback_t)quit);

#if 0
     wdt_enable_watchdog(WDT_CLK_DIV_2048,
                         12, 
                         WDT_RST_POWER_ON);
#else 
     wdt_enable_timer(WDT_CLK_DIV_64,
                      0,
                      timer_callback,
                      NULL);
     #endif

     while(1) {
          if(!callbackFired)
               printf("WDT counter: %u\n", wdt_get_counter());
          else 
               printf("FIIIRE!\n");
                         wdt_pet();
          fflush(stdout);
     }


     return EXIT_SUCCESS;
}
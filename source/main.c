/*
 * main.c
 */
#define __MAIN_C_dc83edef7fb74d0f88488010fe346ac7	1

#include "main.h"
#include "pwm.h"
#include "ws2812b.h"

#include "libraries/API/iocomfort.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"
#include "libraries/avrlibs-baerwolf/include/cpucontext.h"
#include "libraries/avrlibs-baerwolf/include/hwclock.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* http://nongnu.org/avr-libc/user-manual/modules.html */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/delay.h>


void init_cpu(void) {
  cli();
  bootupreason=MCUBOOTREASONREG;
  MCUBOOTREASONREG=0;
  wdt_disable();

}

int main(void) {
  init_cpu();
  extfunc_initialize();
  EXTFUNC_callByName(cpucontext_initialize);
  EXTFUNC_callByName(pwm_initialize);
  EXTFUNC_callByName(hwclock_initialize);  /* hwclock needs to initialize timers, which will start PWM in pwm.c */
  EXTFUNC_callByName(ws2812b_initialize);

  // YOUR CODE HERE:
  sei();

  {
#define BLINKTICKS HWCLOCK_UStoTICK(100000) /*100ms*/
      hwclock_time_t last, now;
      uint32_t       tdiff;

      last=EXTFUNC_callByName(hwclock_now);
      CFG_OUTPUT(SYSLED);
      CFG_PULLUP(BUTTONPROG);
      while (1) {
        now=EXTFUNC_callByName(hwclock_now);
        tdiff=EXTFUNC_callByName(hwclock_tickspassed, last, now);
        if (tdiff >= BLINKTICKS) {
            if (IS_PRESSED(BUTTONPROG)) {
              pwm_setw(0);
            } else {
              pwm_setw(pwm_getw()+1);
            }
            TOGGLE(SYSLED);
            last=EXTFUNC_callByName(hwclock_modify, last, BLINKTICKS);
        }
      }
  }


  EXTFUNC_callByName(ws2812b_finalize);
  EXTFUNC_callByName(hwclock_finalize);
  EXTFUNC_callByName(pwm_finalize);
  EXTFUNC_callByName(cpucontext_finalize);
  extfunc_finalize();
  return 0;
}

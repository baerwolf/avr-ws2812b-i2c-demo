/*
 * pwm.c
 */
#define __PWM_C_372746308b4011eaab120800200c9a66	1

#include "pwm.h"
#include "libraries/API/iocomfort.h"
#include "libraries/avrlibs-baerwolf/include/hwclock.h"

EXTFUNC_void(int8_t, pwm_initialize) {
  CFG_OUTPUT(PWMLEDR);
  CFG_OUTPUT(PWMLEDG);
  CFG_OUTPUT(PWMLEDB);
  CFG_OUTPUT(PWMLEDW);
  return 0;
}

EXTFUNC_void(int8_t, pwm_finalize) {
  return 0;
}


/* this stuff is for hwclock */
#if (defined (__AVR_ATmega8__) || defined (__AVR_ATmega8A__) || \
     defined (__AVR_ATmega88__) || defined (__AVR_ATmega88P__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88PA__) || \
     defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168A__) || defined (__AVR_ATmega168PA__) || \
     defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328A__) || defined (__AVR_ATmega328PA__) || \
     defined (__AVR_ATmega164__) || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__) || defined (__AVR_ATmega164PA__) || \
     defined (__AVR_ATmega324__) || defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__) || defined (__AVR_ATmega324PA__) || \
     defined (__AVR_ATmega644__) || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644PA__) || \
     defined (__AVR_ATmega1284__)|| defined (__AVR_ATmega1284P__)||                                 defined (__AVR_ATmega32__)    || \
   0)
void __hwclock_timer_init(void) {
  OCR0A=0xff;
  OCR0B=0xff;
  OCR2A=0xff;
  OCR2B=0xff;
  
  TCNT2=0;
  TCNT1=0;
  TCNT0=0;
}

void __hwclock_timer_start(void) {
  /* activate timer 1 with prescaler 256 */
  TCCR1A=0b00000000;
  TCCR1B=0b00000100;

  /* we select a specific PWM mode where OC is set on compare match - this means all pwm values need to be complemented */
  /* so we can completly switch of PWM instead of always having one pulse on... */
  TCCR2A=0b11110011; /* activate fast PWM of timer 2 */
  TCCR0A=0b11110011; /* activate fast PWM of timer 0 */

  TCCR2B=0b00000001; /* activate timer2 running */
  TCCR0B=0b00000001; /* activate timer0 running */
  /* calibrate timer0 to prescaler of timer1                */
  /* in general we need to use 2-cycle memory access:       */
  /* This introduces a problem, since the prescaler can     */
  /* overflow not just inbetween the two tcnthl accesses    */
  /* but also during execution of the second "LDS"...       */
  /* BUT IT IS NOT THE SAME AS: "IN r18" - "NOP" - "IN r19" */
  asm volatile (
      "timer0_calibrate_again%=:       \n\t"
      "lds  r18, %[tcnthl]             \n\t" /* 2 cycles - sampled after 1 cycle */
      "lds  r19, %[tcnthl]             \n\t" /* 2 cycles - sampled after 1 cycle */
      "inc  r18                        \n\t" /* 1 cycle  */
      "nop                             \n\t" /* 1 cycle to gcd()=1 */
      "cp   r18, r19                   \n\t" /* 1 cycle  */
      "brne timer0_calibrate_again%=   \n\t" /* 2 cycles jumping - 1 cycle continue */


      /* we configure here, as overflow would have happend during sampling of "LDS r19" */
      "ldi  r20, 7                     \n\t" /* 1 cycle  */
      "out  %[tcnt], r20               \n\t" /* 1 cycle  */
      "nop                             \n\t" /* 1 cycle - here TCNT0 must be 7 */

      /* if prescaler overflowed starting at "LDS r19" before sampling... */
      /* ...it overflows again in 254 cycles after that.                  */
      /* (That means if LDS is executed in 253 cycles and tcnthl sampled  */
      /*  in 254 cycles to be incremented one more...)                    */
      /* taking the other opcodes into account leaves us with 246 cycles  */
      "ldi  r20, 246                   \n\t" /* 1 cycle  */
      "timer0_calibrate_busyloop%=:    \n\t"
      "subi r20, 3                     \n\t" /* 1 cycle  */
      "brne timer0_calibrate_busyloop%=\n\t" /* 2 cycles jumping - 1 cycle continue */

      /* here we are 1 cycle ahead of overflow? (due to brne) - 245 cycles passed  */
      /* 253 cycles since last "LDS r19" ...                                       */
      "lds  r19, %[tcnthl]             \n\t" /* 2 cycles - sampled after 1 cycle   */
      "ldi  r20, 3                     \n\t" /* 1 cycle - in case of equal, overflow happens here */
      "cpse r18, r19                   \n\t" /* 1 cycle - equal means overflow when sampled */
      "out  %[tcnt], r20               \n\t" /* 1 cycle - overflow happend before sampling */


      :
      : [tcnt]          "i" (_SFR_IO_ADDR(TCNT0)),
        [tcnthl]		"i"	(_SFR_MEM_ADDR(HWCLOCK_MSBTIMER_VALUEREG_LOW)),
        [tcntll]		"i"	(_SFR_MEM_ADDR(HWCLOCK_LSBTIMER_VALUEREG_LOW))
      : "r20", "r19", "r18"
           );
} /* end of hardware selected "__hwclock_timer_start" */
#else
#	error unsupported AVR
#endif

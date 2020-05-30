/*
 * main.c
 */
#define __MAIN_C_dc83edef7fb74d0f88488010fe346ac7	1

#include "main.h"
#include "pwm.h"
#include "ws2812b.h"
#include "protocol.h"

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
  sei(); /* so SREG of protocol-/twithred becomes initialzed with interrupts on */
  EXTFUNC_callByName(protocol_initialize);

  sei();

  {
#define BLINKTICKS HWCLOCK_UStoTICK(100000) /*100ms*/
#define LEDDOFFSET 6
#define LEDS       28
      hwclock_time_t last, now;
      uint32_t       tdiff;
      uint16_t       lastFrameOperand;
      bool           simpleAutoFrame;
      static uint8_t fb[LEDDOFFSET+LEDS][3];
      uint8_t        helper[3];

      lastFrameOperand=LEDS;
      simpleAutoFrame=false;
      last=EXTFUNC_callByName(hwclock_now);
      CFG_OUTPUT(SYSLED);
      CFG_PULLUP(BUTTONPROG);
      _delay_ms(100);
      memset(fb, 0, sizeof(fb));
      memset(fb, 0xff, 18);
      EXTFUNC_callByName(ws2812b_txbuffer, fb, (lastFrameOperand+LEDDOFFSET)*sizeof(helper));

      while (1) {
        protocol_packet_t *packet;
        packet = EXTFUNC_callByName(protocol_poll);
        if (packet) {
            switch (packet->payload.command) {
                case PROTOCOL_CMD_DOSIMPLEAUTOFRAME: {
                    if (lastFrameOperand>0) {
                        simpleAutoFrame=true;
                    }
                    break;
                }
                case PROTOCOL_CMD_RORPIXELS: {
                    if (lastFrameOperand > 1) {
                        helper[0]=fb[LEDDOFFSET+lastFrameOperand-1][0];
                        helper[1]=fb[LEDDOFFSET+lastFrameOperand-1][1];
                        helper[2]=fb[LEDDOFFSET+lastFrameOperand-1][2];
                        memmove(&fb[LEDDOFFSET+1][0],&fb[LEDDOFFSET][0],(lastFrameOperand-1)*sizeof(helper));
                        fb[LEDDOFFSET+0][0]=helper[0];
                        fb[LEDDOFFSET+0][1]=helper[1];
                        fb[LEDDOFFSET+0][2]=helper[2];
                    }
                    break;
                }
                case PROTOCOL_CMD_DOFRAME: {
                    /* clamp LEDs to limit */
                    if (packet->payload.operand > LEDS) packet->payload.operand=LEDS;

                    simpleAutoFrame=false;

                    lastFrameOperand=packet->payload.operand;
                    if (lastFrameOperand>0) {
                        EXTFUNC_callByName(ws2812b_txbuffer, fb, (lastFrameOperand+LEDDOFFSET)*sizeof(helper));
                    }
                    break;
                }
                case PROTOCOL_CMD_SETPIXEL: {
                    if (packet->payload.operand<LEDS) {
                        fb[LEDDOFFSET+packet->payload.operand][0]=packet->payload.color.g;
                        fb[LEDDOFFSET+packet->payload.operand][1]=packet->payload.color.r;
                        fb[LEDDOFFSET+packet->payload.operand][2]=packet->payload.color.b;
                    } else {
                        pwm_setr(packet->payload.color.r);
                        pwm_setg(packet->payload.color.g);
                        pwm_setb(packet->payload.color.b);
                        pwm_setw(packet->payload.color.w);
                    }
                    break;
                }
            }
        }

        now=EXTFUNC_callByName(hwclock_now);
        tdiff=EXTFUNC_callByName(hwclock_tickspassed, last, now);
        if (tdiff >= BLINKTICKS) {
            if (IS_PRESSED(BUTTONPROG)) {
            } else {
                TOGGLE(SYSLED);
            }
            if (simpleAutoFrame) EXTFUNC_callByName(ws2812b_txbuffer, fb, (lastFrameOperand+LEDDOFFSET)*sizeof(helper)); 
            last=EXTFUNC_callByName(hwclock_modify, last, BLINKTICKS);
        }
      }
  }


  EXTFUNC_callByName(protocol_finalize);
  EXTFUNC_callByName(ws2812b_finalize);
  EXTFUNC_callByName(hwclock_finalize);
  EXTFUNC_callByName(pwm_finalize);
  EXTFUNC_callByName(cpucontext_finalize);
  extfunc_finalize();
  return 0;
}

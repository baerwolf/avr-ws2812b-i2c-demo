/*
 * twithread.c
 */
#define __TWITHREAD_C_4e7982a9bae74c01a264b6578463045b	1

#include "twithread.h"
#include "twi.h"

#include "libraries/avrlibs-baerwolf/include/extfunc.h"
#include "libraries/avrlibs-baerwolf/include/cpucontext.h"

#include <stdint.h>

#include <avr/interrupt.h>


static uint8_t              __twithread_stack[TWITHREAD_STACK_BYTESIZE];
static cpucontext_t         __twithread;

cpucontext_t*               twithread = &__twithread;

       volatile uint8_t     twiRXlen;
static volatile uint8_t     twiRXsize;
static uint8_t              *twiRXbuffer;


static EXTFUNCFAR void __twithread_yield(void) {
    /* voluntary give up the CPU */
    EXTFUNC_callByName(cpucontext_switch, cpucontext_current->previous_running);
}


static EXTFUNCFAR uint8_t __twithread_waitMasterAction(void) {
  while (!twi_hasAction()) {
      __twithread_yield();
      
    /* check for requested changes on the TWI */
    if (0) {
        /* change sth. */
    }
  }

  return TW_STATUS;
}

static EXTFUNCFAR uint8_t __twithread_waitAction(void) {
  while (!twi_hasAction()) {
      __twithread_yield();
  }

  return TW_STATUS;
}


/* this state is entered if TWI has entered an legal RX-Mode */
static EXTFUNCFAR void __twithread_inRX(const uint8_t twistatus) {
  uint8_t __twistatus=twistatus;

  twiRXlen=0;
  while (__twistatus!=TW_SR_STOP) {
      twi_release(twi_defaultACK());    /* acknowlede the data */
      __twistatus=__twithread_waitAction();
      if (__twistatus==TW_SR_STOP) break;

      /* also wait, if data is processed */
      while (twiRXlen>=twiRXsize) __twithread_yield();
      twiRXbuffer[twiRXlen++]=TWDR;     /* receive some data */
  }
}

/* this state is entered if TWI has entered an unsupported Mode which is not used */
static EXTFUNCFAR void __twithread_inTX(const uint8_t twistatus) {
  long    __addr=0; 
  uint8_t *__ram=NULL;
  uint8_t __twistatus=twistatus;

  /* try to cancel arbitration asap */
  while (1) {
      switch (__twistatus) {
        case TW_ST_SLA_ACK:
        case TW_ST_ARB_LOST_SLA_ACK:
        case TW_ST_DATA_ACK: {
            if ((__addr >= RAMSTART) &&
                (__addr <= RAMEND)) TWDR=__ram[__addr];
            else                    TWDR=0xff;
            __addr++;
            twi_release(twi_defaultNACK()); /* for now we NACK, because we don't want to send stuff */
            break;
        }

        case TW_ST_DATA_NACK:
        case TW_ST_LAST_DATA: {
            twi_release(twi_defaultACK());
            return;
        }
        
        default: {
            /* don't know what is happening here - go back to mainloop */
            twi_release(twi_defaultACK());
            return;
        }
      }
      __twistatus=__twithread_waitAction();
  }
}



EXTFUNC(int8_t, __twithread_main, void* parameters)  {
  /* this thread is its own cpucontext and has its own SREG */
  /* so activate interrups here as in mainthread, too       */
  sei();

  while (1) {
    uint8_t __twistatus;
    /* at this point we (as a slave) are not aquired and are just listenin gto the bus */
    __twistatus=__twithread_waitMasterAction(); /* __twithread_waitMasterAction is only for waiting here, where TWI is changeable */
    switch (__twistatus) {
        /* SLA+W*/
        case TW_SR_SLA_ACK:
        case TW_SR_ARB_LOST_SLA_ACK:
        case TW_SR_GCALL_ACK:
        case TW_SR_ARB_LOST_GCALL_ACK: {
            __twithread_inRX(__twistatus);
            break;
        }

        /* SLA+R - unsupported */
        case TW_ST_SLA_ACK:
        case TW_ST_ARB_LOST_SLA_ACK: {
            __twithread_inTX(__twistatus);
            break;
        }

        default: {
            /* don't know what is happening here */
            TWDR=0; /* don't leak any data */
            twi_release(twi_defaultACK());
        }
    }
  }
  return 0;
}



// ** these function don't run within the twithread context ** //

EXTFUNC_void(int8_t, twithread_initialize) {
  if (__twithread.flags & _BV(CPUCONTEXT_FLAGBM_initialized)) {
      return 1;
  } else {
      EXTFUNC_functype(CPUCONTEXT_entry_t) __twithread_main_ptr;

      twiRXlen=0;
      twiRXsize=0;
      twiRXbuffer=NULL;
      EXTFUNC_callByName(cpucontext_initialize);
      EXTFUNC_callByName(twi_initialize);
      __twithread_main_ptr = EXTFUNC_getPtr(__twithread_main, CPUCONTEXT_entry_t);

      EXTFUNC_callByName(cpucontext_create, &__twithread, __twithread_stack, sizeof(__twithread_stack), __twithread_main_ptr, NULL);
  }
  return 0;
}

EXTFUNC_void(int8_t, twithread_finalize) {
  if (__twithread.flags & _BV(CPUCONTEXT_FLAGBM_initialized)) {
      __twithread.flags=0;
      EXTFUNC_callByName(cpucontext_finalize);
      EXTFUNC_callByName(twi_finalize);
  } else {
      return 1;
  }
  return 0;
}

EXTFUNC_void(int8_t, twithread_poll) {
  EXTFUNC_callByName(cpucontext_switch, &__twithread);
  return 0;
}

EXTFUNC(int8_t, twithread_setRXBuffer, void* buffer, uint8_t bytesize) {
  twiRXlen=0;
  twiRXbuffer=buffer;
  twiRXsize=bytesize;
  return 0;
}

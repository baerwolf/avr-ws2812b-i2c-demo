/*
 * protocol.c
 */
#define __PROTOCOL_C_a31a1f168b584d09b11c547754a5c5cb	1

#include "protocol.h"
#include "twithread.h"

#include <stdint.h>

#include <avr/cpufunc.h> 
// #include <avr/eeprom.h> 


static uint8_t currentRX=0;
static protocol_packet_t __RXpacket[2];


EXTFUNC_void(int8_t, protocol_initialize) {
  int8_t i = EXTFUNC_callByName(twithread_initialize);
  if (i==0) EXTFUNC_callByName(twithread_setRXBuffer, &(__RXpacket[currentRX].payload), sizeof(protocol_packetdata_t));
  return i;
}

EXTFUNC_void(int8_t, protocol_finalize) {
  int8_t i = EXTFUNC_callByName(twithread_finalize);
  return i;
}


EXTFUNC_void(protocol_packet_t*, protocol_poll) {
  protocol_packet_t *result=NULL;

  EXTFUNC_callByName(twithread_poll);
  if (twiRXlen==sizeof(protocol_packetdata_t)) {
      _MemoryBarrier();
//       eeprom_update_block(&(__RXpacket[currentRX].payload), (void*)(512-8), 8);
      if (__RXpacket[currentRX].payload.version == PROTOCOL_VERSION) {
        __RXpacket[currentRX].rcvflags=currentRX;
        result=&__RXpacket[currentRX];
        currentRX=1-currentRX;
        EXTFUNC_callByName(twithread_setRXBuffer, &(__RXpacket[currentRX].payload), sizeof(protocol_packetdata_t));
      } else twiRXlen=0;
  }

  return result;
}

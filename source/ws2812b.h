/*
 * ws2812b.h
 * 
 * This driver for ws2812b LED timings implements a clever way to transmit
 * colour data signals without utilizing the CPU the whole TX time.
 * 
 * This is done by using the USART's SPI master mode. Thanks to double buffering
 * the transmit can happen in parallel to other code. Only the buffer refills
 * take time.
 * In order to save as much time as possible, the code uses exclusive registers
 * and thus avoids some push and pop...
 * In order to maintain realtime, the code uses interrups and should be the only
 * interrupt sourcer...
 * 
 * more details
 * ^^^^^^^^^^^^
 * In 20MHz system clock with USART prescaler of value 8 the bit timing is 0.4us.
 * This is a good factor of the WS2812B timings needed.
 * Every single color data bit is a timing of some high followed by some low part.
 * This can be represented by 3 SPI data bits emulating the NRZI: 
 *  --> 100 for 0.4us high followed by 0.8us low (equals to color bit ->0<-)
 *  --> 110 for 0.8us high followed by 0.4us low (equals to color bit ->1<-)
 * 
 * While transmitting one SPI data buffer of 8 bits there are then 64 cpu cycles
 * time to refill the double buffer.
 * Taking the cycles burned for interrupt entering into account, this code should
 * do it in about 46cycles - leaving the rest for the cpu to do other things...
 * 
 * Color value 0xff is an ESC sequence to send only SPI-zeros.
 * Because in USART SPI TX pin is high by default, we need 50us (min) low to reset
 * This is achieved by sending 16byte of 0xff (16byte * 8bit * 0.4us --> 51.2us)
 * 
 * 
 * info
 * ^^^^
 * This could could support (8bit or 12bit) color paletts in order to save pixel buffer.
 * However this is the uncompressed version with no paletting.
 *
 * 
 * Stephan Baerwolf (matrixstorm@gmx.de), Schwansee 2020
 * (please contact me at least before commercial use)
 */

#ifndef __WS2812B_H_735f31908ba311eaab120800200c9a66
#define __WS2812B_H_735f31908ba311eaab120800200c9a66	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#ifdef __WS2812B_C_735f31908ba311eaab120800200c9a66
#	define	WS2812BPUBLIC
#else
#	define	WS2812BPUBLIC		extern
#endif


#   ifndef WS2812B_TXPin
#       define WS2812B_TXPin   D,1
#   endif

#if (defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88P__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88PA__)  || \
     defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168A__) || defined (__AVR_ATmega168PA__) || \
     defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328A__) || defined (__AVR_ATmega328PA__) || \
  0)

#   ifndef WS2812B_CLKPin
#       define WS2812B_CLKPin   D,4
#   endif

#   ifndef WS2812B_REFILLISR_vect
#       define WS2812B_REFILLISR_vect   USART_UDRE_vect
#   endif

#   ifndef WS2812B_TXComplete_vect
#     define WS2812B_TXComplete_vect    USART_TX_vect
#   endif

#elif(defined (__AVR_ATmega164__) || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__) || defined (__AVR_ATmega164PA__) || \
      defined (__AVR_ATmega324__) || defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__) || defined (__AVR_ATmega324PA__) || \
      defined (__AVR_ATmega644__) || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644PA__) || \
      defined (__AVR_ATmega1284__)|| defined (__AVR_ATmega1284P__)||                                                                  \
  0)

#   ifndef WS2812B_CLKPin
#       define WS2812B_CLKPin   B,0
#   endif

#   ifndef WS2812B_REFILLISR_vect
#     define WS2812B_REFILLISR_vect     USART0_UDRE_vect
#   endif

#   ifndef WS2812B_TXComplete_vect
#     define WS2812B_TXComplete_vect    USART0_TX_vect
#   endif

#else
#   error unsupported AVR - unknown USART SPI master
#endif


WS2812BPUBLIC EXTFUNC_voidhead(int8_t, ws2812b_initialize);
WS2812BPUBLIC EXTFUNC_voidhead(int8_t, ws2812b_finalize);

#define ws2812b_inTX(x) ((__register8!=0)||(__register9!=0))
WS2812BPUBLIC EXTFUNC_head(int8_t, ws2812b_txbuffer, const void* buffer, size_t bufferbytesize);


/* work around some avr libc bugs */
#ifndef UDORD0
#   ifdef UCSZ01
#       define UDORD0   UCSZ01
#   endif
#endif

#endif

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

WS2812BPUBLIC EXTFUNC_voidhead(int8_t, ws2812b_initialize);
WS2812BPUBLIC EXTFUNC_voidhead(int8_t, ws2812b_finalize);

#endif

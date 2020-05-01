/*
 * pwm.h
 */
#ifndef __PWM_H_372746308b4011eaab120800200c9a66
#define __PWM_H_372746308b4011eaab120800200c9a66	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#include <avr/io.h>

#if (defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88P__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88PA__)  || \
     defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168A__) || defined (__AVR_ATmega168PA__) || \
     defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328A__) || defined (__AVR_ATmega328PA__) || \
     defined (__AVR_ATmega164__) || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__) || defined (__AVR_ATmega164PA__) || \
     defined (__AVR_ATmega324__) || defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__) || defined (__AVR_ATmega324PA__) || \
     defined (__AVR_ATmega644__) || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644PA__) || \
     defined (__AVR_ATmega1284__)|| defined (__AVR_ATmega1284P__)||                                                                  \
   0)
#else
#	error unsupported AVR
#endif

#ifdef __PWM_C_372746308b4011eaab120800200c9a66
#	define	PWMPUBLIC
#else
#	define	PWMPUBLIC		extern
#endif

#ifndef PWMLEDR
#   define PWMLEDR  B,3 /*OC2A*/
#endif
#ifndef PWMREGR
#   define PWMREGR  OCR2A
#endif


#ifndef PWMLEDG
#   define PWMLEDG  D,3 /*OC2B*/
#endif
#ifndef PWMREGG
#   define PWMREGG  OCR2B
#endif


#ifndef PWMLEDB
#   define PWMLEDB  D,6 /*OC0A*/
#endif
#ifndef PWMREGB
#   define PWMREGB  OCR0A
#endif


#ifndef PWMLEDW
#   define PWMLEDW  D,5 /*OC0B*/
#endif
#ifndef PWMREGW
#   define PWMREGW  OCR0B
#endif


PWMPUBLIC EXTFUNC_voidhead(int8_t, pwm_initialize);
PWMPUBLIC EXTFUNC_voidhead(int8_t, pwm_finalize);

#define pwm_setr(x) {PWMREGR=~(x);}
#define pwm_setg(x) {PWMREGG=~(x);}
#define pwm_setb(x) {PWMREGB=~(x);}
#define pwm_setw(x) {PWMREGW=~(x);}

#define pwm_getr(x) (~(PWMREGR))
#define pwm_getg(x) (~(PWMREGG))
#define pwm_getb(x) (~(PWMREGB))
#define pwm_getw(x) (~(PWMREGW))

#endif

/*
 * pwm.h
 */
#ifndef __PWM_H_372746308b4011eaab120800200c9a66
#define __PWM_H_372746308b4011eaab120800200c9a66	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#include <avr/io.h>

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

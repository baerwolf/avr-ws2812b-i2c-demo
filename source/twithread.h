/*
 * twithread.h
 */
#ifndef __TWITHREAD_H_4e7982a9bae74c01a264b6578463045b
#define __TWITHREAD_H_4e7982a9bae74c01a264b6578463045b	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"
#include "libraries/avrlibs-baerwolf/include/cpucontext.h"

#include <stdint.h>

#define     TWITHREAD_STACK_BYTESIZE    128


#ifdef __TWITHREAD_C_4e7982a9bae74c01a264b6578463045b
#	define	TWITHREADPUBLIC
#else
#	define	TWITHREADPUBLIC		extern
#endif

TWITHREADPUBLIC cpucontext_t*       twithread;
TWITHREADPUBLIC volatile uint8_t    twiRXlen;

TWITHREADPUBLIC EXTFUNC_voidhead(int8_t, twithread_initialize);
TWITHREADPUBLIC EXTFUNC_voidhead(int8_t, twithread_finalize);

TWITHREADPUBLIC EXTFUNC_voidhead(int8_t, twithread_poll);

TWITHREADPUBLIC EXTFUNC_head(int8_t, twithread_setRXBuffer, void* buffer, uint8_t bytesize);

#endif

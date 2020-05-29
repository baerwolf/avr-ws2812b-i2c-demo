/*
 * twithread.h
 */
#ifndef __TWITHREAD_H_4e7982a9bae74c01a264b6578463045b
#define __TWITHREAD_H_4e7982a9bae74c01a264b6578463045b	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#ifdef __TWITHREAD_C_4e7982a9bae74c01a264b6578463045b
#	define	TWITHREADPUBLIC
#else
#	define	TWITHREADPUBLIC		extern
#endif


TWITHREADPUBLIC EXTFUNC_voidhead(int8_t, twithread_initialize);
TWITHREADPUBLIC EXTFUNC_voidhead(int8_t, twithread_finalize);

#endif

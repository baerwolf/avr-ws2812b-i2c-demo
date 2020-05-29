/*
 * protocol.h
 */
#ifndef __PROTOCOL_H_a31a1f168b584d09b11c547754a5c5cb
#define __PROTOCOL_H_a31a1f168b584d09b11c547754a5c5cb	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#ifdef __PROTOCOL_C_a31a1f168b584d09b11c547754a5c5cb
#	define	PROTOCOLPUBLIC
#else
#	define	PROTOCOLPUBLIC		extern
#endif


PROTOCOLPUBLIC EXTFUNC_voidhead(int8_t, protocol_initialize);
PROTOCOLPUBLIC EXTFUNC_voidhead(int8_t, protocol_finalize);

#endif

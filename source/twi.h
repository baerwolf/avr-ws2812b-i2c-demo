/*
 * twi.h
 */
#ifndef __TWI_H_c8855ccc975748f0bccf6cdfc9e62e7a
#define __TWI_H_c8855ccc975748f0bccf6cdfc9e62e7a	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#ifdef __TWI_C_c8855ccc975748f0bccf6cdfc9e62e7a
#	define	TWIPUBLIC
#else
#	define	TWIPUBLIC		extern
#endif


TWIPUBLIC EXTFUNC_voidhead(int8_t, twi_initialize);
TWIPUBLIC EXTFUNC_voidhead(int8_t, twi_finalize);

#endif

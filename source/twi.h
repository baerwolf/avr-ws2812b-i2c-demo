/*
 * twi.h
 */
#ifndef __TWI_H_c8855ccc975748f0bccf6cdfc9e62e7a
#define __TWI_H_c8855ccc975748f0bccf6cdfc9e62e7a	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#include <util/twi.h>

#ifndef TWI_DEFAULTADDRESS
#   define TWI_DEFAULTADDRESS (0x48)
#endif


#ifdef __TWI_C_c8855ccc975748f0bccf6cdfc9e62e7a
#	define	TWIPUBLIC
#else
#	define	TWIPUBLIC		extern
#endif


#define twi_release(x) TWCR=(_BV(TWINT)|(x))
#define twi_hasAction(x) (TWCR & _BV(TWINT))

#define twi_defaultACK(x)   ((1*_BV(TWEA)) | _BV(TWEN))
#define twi_defaultNACK(x)  ((0*_BV(TWEA)) | _BV(TWEN))


TWIPUBLIC EXTFUNC_voidhead(int8_t, twi_initialize);
TWIPUBLIC EXTFUNC_voidhead(int8_t, twi_finalize);

#endif

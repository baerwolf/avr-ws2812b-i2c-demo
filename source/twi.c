/*
 * twi.c
 */
#define __TWI_C_c8855ccc975748f0bccf6cdfc9e62e7a	1

#include "twi.h"

EXTFUNC_void(int8_t, twi_initialize) {
  if (twi_hasAction()) twi_release(0) ;  
  TWAR = ((TWI_DEFAULTADDRESS) << 1);
  TWCR = (1*_BV(TWEN)) | (1*_BV(TWEA)) | (0*_BV(TWSTA)) | (0*_BV(TWSTO));
  return 0;
}

EXTFUNC_void(int8_t, twi_finalize) {
  //TODO: stop the TWI
  return 0;
}

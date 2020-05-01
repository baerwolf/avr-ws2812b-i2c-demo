/*
 * defines.h
 */
#ifndef __DEFINES_H_94b054518dfc475a9d5b532207a60b61
#define __DEFINES_H_94b054518dfc475a9d5b532207a60b61 1

#ifndef SYSLED
#   define SYSLED       B,0
#endif

#ifndef BUTTONPROG
#   define BUTTONPROG   D,6
#endif

#ifdef CPUCONTEXTINCLUDEDEFINES
#   include <stdint.h>
 /* our WS2812B timing driver needs registers r2 to r7 exclusively for its own */
 
 /* tell the compiler to block the registers and avoid generating code with them */
 register uint8_t __register2 asm ("r2");
 register uint8_t __register3 asm ("r3");
 register uint8_t __register4 asm ("r4");
 register uint8_t __register5 asm ("r5");
 register uint8_t __register6 asm ("r6");
 register uint8_t __register7 asm ("r7");
 
 /* inform cpucontext library NOT to switch it between contexts */
#   define CONFIG_CPUCONTEXT_NO_R2
#   define CONFIG_CPUCONTEXT_NO_R3
#   define CONFIG_CPUCONTEXT_NO_R4
#   define CONFIG_CPUCONTEXT_NO_R5
#   define CONFIG_CPUCONTEXT_NO_R6
#   define CONFIG_CPUCONTEXT_NO_R7
#endif

#endif

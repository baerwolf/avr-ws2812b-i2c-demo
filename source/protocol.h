/*
 * protocol.h
 */
#ifndef __PROTOCOL_H_a31a1f168b584d09b11c547754a5c5cb
#define __PROTOCOL_H_a31a1f168b584d09b11c547754a5c5cb	1

#include "defines.h"
#include "libraries/avrlibs-baerwolf/include/extfunc.h"

#include <stdint.h>

#define PROTOCOL_VERSION        (0xff)

#define PROTOCOL_CMD_DOSIMPLEAUTOFRAME    (0x7F)
#define PROTOCOL_CMD_DOFRAME              (0x80)
#define PROTOCOL_CMD_SETPIXEL             (0x81)
#define PROTOCOL_CMD_RORPIXELS            (0xA0)

#ifdef __PROTOCOL_C_a31a1f168b584d09b11c547754a5c5cb
#	define	PROTOCOLPUBLIC
#else
#	define	PROTOCOLPUBLIC		extern
#endif

struct __protocol_ledcolor_t {
    union {
        uint8_t _b[4];
        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t w;
        } __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));
typedef struct __protocol_ledcolor_t protocol_ledcolor_t;


struct __protocol_packetdata_t {
    uint8_t                 version; /* should be 0xff for now */
    uint8_t                 command;
    uint16_t                operand;
    protocol_ledcolor_t     color;
} __attribute__((packed));
typedef struct __protocol_packetdata_t protocol_packetdata_t;


struct __protocol_packet_t {
    uint8_t                 rcvflags;
    protocol_packetdata_t   payload;
} __attribute__((packed));
typedef struct __protocol_packet_t protocol_packet_t;


PROTOCOLPUBLIC EXTFUNC_voidhead(int8_t, protocol_initialize);
PROTOCOLPUBLIC EXTFUNC_voidhead(int8_t, protocol_finalize);

PROTOCOLPUBLIC EXTFUNC_voidhead(protocol_packet_t*, protocol_poll);

#endif

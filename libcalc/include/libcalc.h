#include <stdint.h>

#define DEC128_W5			17
#define DEC128_T			110

#define DEC128_TDECLETS		11
#define DEC128_P			34		// Precision of decimal 128 
#define DEC128_BCDBYTES		17		// DEC128_P / 2

#define NAN_SHIFT			0
#define NAN_SIGNAL_SHIFT	1
#define INFINITY_SHIFT		2

// Store big endian
typedef struct decimal128 {
	uint8_t data[16];
} decimal128;

// Expanded version of decimal128
typedef struct dec128 {
	uint8_t sign;
	uint8_t flags;							// isNaN
	uint16_t exponent;
	uint8_t digits[DEC128_BCDBYTES];		//34 decimal digits
} dec128;

int unpack_decimal128(decimal128* src, dec128* dst);
int pack_decimal128(dec128* src, decimal128* dst);

int decode_dpd(uint16_t dpd, uint8_t* x, uint8_t* y, uint8_t* z);

// lookup table utils

#define DPDXM		0b110000
#define DPDYM		0b001100
#define DPDZM		0b000011

#define DPDX8		0b000000
#define DPDX1		0b010000

#define DPDY8		0b000000
#define DPDY1		0b000100
#define DPDY2		0b001000

#define DPDZ8		0b000000
#define	DPDZ1		0b000001
#define	DPDZ2		0b000010
#define DPDZ3		0b000011

const uint8_t dpd_lookup[8] = {
	0,
	0,
	0,
};

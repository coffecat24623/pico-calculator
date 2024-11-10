#ifndef __CALC_DECIMAL_GUARD__
#define __CALC_DECIMAL_GUARD__

#include <stdint.h>

#define DEC128_W5			17
#define DEC128_T			110
#define DEC128_BIAS			6176

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
	int16_t exponent;
	uint8_t digits[DEC128_BCDBYTES];		//34 decimal digits
} dec128;

int unpack_decimal128(decimal128* src, dec128* dst);
int pack_decimal128(dec128* src, decimal128* dst);

int decode_dpd(uint16_t dpd, uint8_t* x, uint8_t* y, uint8_t* z);
uint16_t encode_dpd(uint8_t x, uint8_t y, uint8_t z);

// lookup table utils

#endif

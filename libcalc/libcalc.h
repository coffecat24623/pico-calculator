#include <stdint.h>

#define DEC128_W5	17
#define DEC128_T	110
// Store big endian
typedef struct decimal128 {
	uint8_t data[16];
} decimal128;

// Expanded version of decimal128
typedef struct dec128 {
	uint8_t sign;
	uint16_t exponent;
	uint8_t digits[17];		//34 decimal digits
} dec128;

int unpack_decimal128(decimal128* src, dec128* dst);
int pack_decimal128(dec128* src, decimal128* dst);


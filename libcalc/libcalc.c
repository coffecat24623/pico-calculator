#include <stdint.h>
#include "libcalc.h"

int unpack_decimal128(decimal128 * src, dec128* dst) {
	uint32_t combined;
	//11 in trailing and one in our combined field
	uint16_t declets[12];
	uint8_t a = src->data[0];
	uint8_t b = src->data[1];
	uint8_t c = src->data[2];
	combined = ((a & 0x7f) << 16) | (b << 8) | c;

	dst->sign = (a >> 7);

	// trailing significand is made out of 11 declets = 110 bits
	// each i look at two bytes required
	
	// loop has to consume the first none trailing bits, this value of stored causes it to do that
	// and record a degenerate declet as the first one, but we replace this with the implied digit
	uint8_t stored = 10 - ((DEC128_W5 + 1) >> 3);
	uint16_t declet = 0;
	uint8_t declets_parsed = 0;

	// parse trailing declets
	for (int i = 2; i < 16; i++) {
		uint8_t cur = src->data[i];
		uint8_t remaining = 10 - stored;
		if (remaining < 8) {
			uint8_t p1, p2;
			p1 = cur >> (8 - remaining);
			p2 = cur & (8 - remaining);
			declet = (declet << remaining) | p1;

			// push this onto our declet stack
			declets[declets_parsed] = declet;
			declets_parsed += 1;

			// reset
			declet = p2;
			stored = (8 - remaining);
		} else {
			declet = (declet << 8) | cur;
			stored += 8;
			if (stored == 10) {
				// push this onto our declet stack
				declets[declets_parsed] = declet;
				declets_parsed += 1;

				stored = 0;
				declet = 0;
			}
		}
	}

	return 0;
}

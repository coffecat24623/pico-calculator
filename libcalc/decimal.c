#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "decimal.h"


int unpack_decimal128(decimal128 * src, dec128* dst) {
	uint32_t combined;

	// 11 in trailing and one in our combined field
	uint16_t declets[DEC128_TDECLETS];

	// these cover the combined field
	uint8_t a = src->data[0];
	uint8_t b = src->data[1];
	uint8_t c = src->data[2];

	uint8_t metadata = 0;
	uint8_t flags = 0;
	uint8_t digit_trailing;

	// digits for declet decoding process temporary storage
	uint8_t digit_triplets[3];

	//s55555EE|EEEEEEEE|EE......|

	// trailing significand is made out of 11 declets = 110 bits
	// each i look at two bytes required
	
	// loop has to consume the first none trailing bits, this value of stored causes it to do that
	// and record a degenerate declet so a bool will turn it off
	
	// TODO: add logic for detecting if this field and sign forms is actually a multiple of eight
	bool ignore_declet = true;
	uint8_t declets_stored = 10 - ((DEC128_W5 + 1) % 8);
	uint8_t declets_parsed = 0;
	uint16_t declet = 0;
	uint16_t exponent_leading;
	uint16_t exponent_trailing;
	uint16_t exponent;

	// For the goofy BCD
	uint8_t digits_stored = 0;

	// initialize memory
	for (int i = 0; i < DEC128_BCDBYTES; i++) {
		dst->digits[i] = 0;
	}
	dst->exponent=0;
	dst->flags=0;
	dst->sign = (a >> 7);

	// get trailing exponent
	exponent_trailing = ((a & 0b11) << 10) | (b << 8) | (c >> 6);

	// parse trailing declets
	for (int i = 2; i < 16; i++) {
		uint8_t cur = src->data[i];
		uint8_t remaining = 10 - declets_stored;
		if (remaining < 8) {
			uint8_t p1, p2;
			uint8_t p2_length = 8 - remaining;

			p1 = cur >> (p2_length);
			p2 = cur & ((1 << p2_length) - 1);

			declet = (declet << remaining) | p1;

			if (ignore_declet) {
				ignore_declet = false;
			} else {
				// push this onto our declet stack if it is not the degenerate declet
				assert(declets_parsed < DEC128_TDECLETS);
				declets[declets_parsed] = declet;
				declets_parsed += 1;
			}

			// reset
			declet = p2;
			declets_stored = (p2_length);
		} else {
			declet = (declet << 8) | cur;
			declets_stored += 8;
			if (declets_stored == 10) {
				// push this onto our declet stack
				if (ignore_declet) {
					ignore_declet = false;
				} else {
					assert(declets_parsed < DEC128_TDECLETS);
					declets[declets_parsed] = declet;
					declets_parsed += 1;
				}

				declets_stored = 0;
				declet = 0;
			}
		}
	}
	// Parse trailing exponents and leading digit
	// sFFFFxxx
	metadata = ((a >> 3) & 0b1111);

	// NaN / inf
	if (metadata == 0b1111) {
		uint8_t naninf = (a >> 2) & 1;
		uint8_t signal = (a >> 1) & 1;

		flags = flags | (naninf << NAN_SHIFT);
		flags = flags | (!naninf << INFINITY_SHIFT);
		flags = flags | (signal << NAN_SIGNAL_SHIFT);

		// I don't think specs describes if these are neeed so just get rid of
		exponent_leading = 0;
		digit_trailing = 0;
	} else if ((metadata & 0b1100) == 0b1100) {
		//s11xxcEE
		digit_trailing = 8 + ((a >> 2) & 1);
		exponent_leading = ((a >> 3) & 0b11);
	} else {
		//sxxcccEE
		digit_trailing = ((a >> 2) & 0b111);
		exponent_leading = ((a >> 5) & 0b11);
	}

	// This is the biased exponent
	exponent = (exponent_leading << 12) | exponent_trailing;
	dst->exponent = ((int16_t) exponent) - DEC128_BIAS;

	dst->flags = flags;

	//start storing to BCD
	dst->digits[0] |= (digit_trailing << 4);
	digits_stored += 1;

	for (int i = 0; i < DEC128_TDECLETS; i++) {
		decode_dpd(declets[i], &digit_triplets[0], 
							   &digit_triplets[1],
							   &digit_triplets[2]);
		for (int j = 0; j < 3; j++) {

			uint8_t index = digits_stored >> 1;
			assert(index < DEC128_BCDBYTES);
			if (digits_stored % 2) {
				dst->digits[index] |= digit_triplets[j];
			} else {
				dst->digits[index] |= (digit_triplets[j] << 4);
			}
			digits_stored += 1;
		}
	}

	return 0;
}

// https://en.wikipedia.org/wiki/Densely_packed_decimal
int decode_dpd(uint16_t dpd, uint8_t* px, uint8_t* py, uint8_t* pz) {
	uint8_t x = (dpd >> 7) & 1;
	uint8_t y = (dpd >> 4) & 1;
	uint8_t z = (dpd >> 0) & 1;
	if (dpd & 0b1000) {
		switch ((dpd & 0b1110) >> 1) {
			case 0b100:
				x = (dpd & (0b111 << 7)) >> 7;
				y = (dpd & (0b111 << 4)) >> 4;
				z = z | 0b1000;
				break;
			case 0b101:
				x = (dpd & 0b1110000000) >> 7;
				y = y | 0b1000;
				z = z | ((dpd & (0b11 << 5)) >> 4);
				break;
			case 0b110:
				x = x | 0b1000;
				y = (dpd & (0b111 << 4)) >> 4;
				z = z | ((dpd & (0b11 << 8)) >> 7);
				break;
			case 0b111:
				switch ((dpd & (0b11 << 5)) >> 5) {
					case 0b00:
						x = x | 0b1000;
						y = y | 0b1000;
						z = z | ((dpd & (0b11 << 8)) >> 7);
						break;
					case 0b01:
						x = x | 0b1000;
						y = y | ((dpd & (0b11 << 8)) >> 7);
						z = z | 0b1000;
						break;
					case 0b10:
						x = (dpd & (0b111 << 7)) >> 7;
						y = 0b1000 | y;
						z = 0b1000 | z;
						break;
					case 0b11:
						x = x | 0b1000;
						y = y | 0b1000;
						z = z | 0b1000;
						break;
					default:
						return -2;
				}
				break;
			default:
				return -1;
			
		}
	} else {
		x = (dpd & (0b111 << 7)) >> 7;
		y = (dpd & (0b111 << 4)) >> 4;
		z = (dpd & 0b111);
	}
	*px = x;
	*py = y;
	*pz = z;

	return 0;
}

uint16_t encode_dpd(uint8_t x, uint8_t y, uint8_t z) {
	uint16_t dpd = ((x & 1) << 7) |
				   ((y & 1) << 4) |
				   (z & 1);

	uint8_t type = ((x & 0b1000) >> 1) |
				   ((y & 0b1000) >> 2) |
				   ((z & 0b1000) >> 3);
	type = type & 7;
	switch (type) {
		case (0b000):
			dpd |= ((x & 7) << 7) |
				  ((y & 7) << 4) |
				  ((z & 7));
			return dpd;
		case (0b001):
			dpd |= ((x & 7) << 7) |
				   ((y & 7) << 4) |
				   0b1000;
			return dpd;
		case (0b010):
			dpd |= ((x & 7) << 7) |
				   ((z & 0b110) << 4) |
				   0b1010;
			return dpd;
		case (0b100):
			dpd |= ((z & 0b110) << 7) |
				   ((y & 7) << 4) |
				   0b1100;
			return dpd;
		case (0b110):
			dpd |= ((z & 0b110) << 7) |
				   0b0001110;
			return dpd;
		case (0b101):
			dpd |= ((y & 0b110) << 7) |
				   0b0101110;
			return dpd;
		case (0b011):
			dpd |= ((x & 0b110) << 7) |
				   0b1001110;
			return dpd;

		// serves case where all digits > 8
		default:
			return dpd | 0b1101110;
	}
}




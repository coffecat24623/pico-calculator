#include "arithmetic.h"


int bcd_shl(const uint8_t* src, uint8_t* dst, uint32_t length, uint32_t shift) {
	// annoying
	// aabbc cddeeff
	uint32_t start_index = shift >> 1;
	uint8_t offset = shift & 1;
	if (start_index >= length) {
		return -1;
	}

	if (offset) {
		uint8_t previous_byte = src[start_index];
		dst[start_index] = 0;

		for (int i = 1; i < length - start_index; i++) {
			uint8_t current_byte = src[start_index + i];
			uint8_t store = (previous_byte << 4) | (current_byte >> 4);

			dst[start_index + i] = 0;
			dst[i] = store;

			previous_byte = current_byte;
		}

		dst[length - start_index] = previous_byte << 4;

	} else {
		// chunk starting at start index move to beginning
		
		for (int i = 0; i < length - start_index; i++) {
			// 
			uint8_t v = dst[start_index + i];
			dst[start_index + i] = 0;
			dst[i] = v;
		}
	}
	return 0;
}

/*
int bcd_shr(uint8_t* src, uint8_t* dst, uint32_t length, uint32_t shift);

int bcd_add(uint8_t* a, uint8_t* b, uint8_t* dst, uint32_t length);
int bcd_sub(uint8_t* a, uint8_t* b, uint8_t* dst, uint32_t length);
*/

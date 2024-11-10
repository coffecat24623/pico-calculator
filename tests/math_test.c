#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libcalc.h"

void describe_dec128(const dec128 * src) {
	printf("Value: ");
	if (src->sign) {
		putchar('-');
	}
	for (int i = 0; i < DEC128_BCDBYTES; i++) {
		uint8_t a, b;
		a = src->digits[i] >> 4;
		b = src->digits[i] & 0b1111;
		putchar('0' + a);
		putchar('0' + b);
	}
	printf(" * 10^%d [%d]\n", src->exponent, src->flags);

}

void u32tb(uint32_t num, char* arr) {
	for (int i = 0; i < 32; i++) {
		int j = (32 - 1) - i;
		if (num & (1 << i)) {
			arr[j] = '1';
		} else {
			arr[j] = '0';
		}
	}
}


void describe_decimal128(const decimal128* src) {
	uint8_t a, b, c;
	char buf[33] = {0};
	buf[32] = 0;
	a = src->data[0];
	b = src->data[1];
	c = src->data[2];
	printf("sign: %d, ", a >> 7);
	uint32_t combined;
	combined = (a & 0b1111111) << 10;
	combined = combined | (b << 2);
	combined = combined | (c >> 6);
	u32tb(combined, buf);
	printf("combined: %s\n", buf + 15);
}

void u16tb(uint16_t num, char* arr) {
	for (int i = 0; i < 16; i++) {
		int j = (16 - 1) - i;
		if (num & (1 << i)) {
			arr[j] = '1';
		} else {
			arr[j] = '0';
		}
	}
}

int test_packing(const dec128* original) {
	decimal128 packed;
	dec128 new;

	pack_decimal128(original, &packed);
	unpack_decimal128(&packed, &new);


	if (cmp_dec128(original, &new)) {
		printf("[!] packing a number and unpacking gave wrong numger\n");
		describe_dec128(original);
		describe_decimal128(&packed);
		describe_dec128(&new);
		return -1;
	}
	return 0;
}

int main(int argc, char** argv) {
	char bs[17];
	bs[16] = 0x00;
	int dpd_pass = 1;

	printf("[*] Testing DPD\n");
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k =0; k < 10; k++) {
				uint16_t dpd = encode_dpd(i, j, k);
				u16tb(dpd, bs);
				uint8_t a, b, c;
				decode_dpd(dpd, &a, &b, &c);
				if ((a != i) || (b != j) || (c != k)) {
					uint8_t type = ((i & 0b1000) >> 1) |
								   ((j & 0b1000) >> 2) |
								   ((k & 0b1000) >> 3);

					printf("[!] Result (%d, %d, %d) -> %s of type %d\n", i, j, k, bs + 6, type);
					printf("[!] Got back (%d, %d, %d)\n", a, b, c);
					dpd_pass = 0;

				}
			}
		}
	}
	if (dpd_pass) {
		printf("[*] DPD passed!\n");
	}

	printf("[*] Testing dec encodings\n");
	dec128 pi = {
		.sign = 0,
		.exponent = -33,
		.flags = 0,
		.digits = {0x31, 0x41, 0x59, 0x26, 0x53, 0x58, 0x97, 0x93, 0x23, 0x84, 0x62, 0x64, 0x33, 0x83, 0x27, 0x95, 0x02}
	};

	test_packing(&pi);

	return 0;
}



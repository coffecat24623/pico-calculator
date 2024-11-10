#include <stdio.h>
#include <stdint.h>
#include "libcalc.h"

void describe_dec128(dec128 * src) {
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

void describe_decimal128(decimal128* src) {
	uint8_t a, b, c;
	char buf[33] = {0};
	buf[32] = 0;
	a = src->data[0];
	b = src->data[1];
	c = src->data[2];
	printf("sign: %d\n", a >> 7);
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
	describe_dec128(&pi);
	dec128 big = {
		.sign = 0,
		.exponent = 0,
		.flags = 0,
		.digits = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99}
	};
	describe_dec128(&big);

	decimal128 bigpi;
	dec128 newpi;

	pack_decimal128(&pi, &bigpi);
	unpack_decimal128(&bigpi, &newpi);

	describe_decimal128(&bigpi);
	describe_dec128(&newpi);


	return 0;
}



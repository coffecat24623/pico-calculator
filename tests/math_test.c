#include <stdio.h>
#include <stdint.h>
#include "libcalc.h"

void uint16_tbs(uint16_t num, char* arr, int length) {
	for (int i = 0; i < 16; i++) {
		int j = (length - 1) - i;
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
				uint16_tbs(dpd, bs, 16);
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


	return 0;
}



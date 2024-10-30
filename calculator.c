#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

const uint LED_PIN = 25;

int main() {
	bi_decl(bi_program_description("This is a super cool calculator"));

	stdio_init_all();

	gpio_init_mask(0x0001ffff);
	for (int i = 0; i < 17; i++) {
		gpio_pull_up(i);
	}

	// First 17 pins


	while (1) {
		for (int i = 0; i < 16; i++) {
			for (int j = i + 1; j < 17; j++) {
				//read pin always registers on multiple i's for some reason, assuming because switching i from OUT to IN takes longer then IN to OUT
				gpio_set_dir(j, GPIO_IN);
				gpio_set_dir(i, GPIO_OUT);

				
				gpio_put(i, 0);

				bool v = gpio_get(j);
				gpio_set_dir(i, GPIO_IN);
				if (!v) {
					//avoid flooding screen smfh
					sleep_ms(50);
					printf("Hit! at %d, %d\n", i,  j);
				}

			}
		}
	}
}
	

#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	if (!bcm2835_init()) {
		// Assert that we actually got the pins
		return 1;
	}
	bcm2835_spi_begin();
	int k = 0;
	while (k < 100) {
		char tbuf[4] = {0x00, 0x00, 0x00, 0x00};
		uint32_t len = 4;

		bcm2835_spi_transfern(tbuf, len);
		printlengthfourarray(tbuf);

		int32_t *temp = tbuf;
		int32_t value_times_4;
    		float actual;
		value_times_4 = *temp >> 18;
		// Divide by 4 to make it a float.
		actual = value_times_4/4.0f;
		//printf("It's %f degrees now. \n", actual);

		sleep(1);
		k += 10;
	}
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}

void printlengthfourarray(char *arr) {
	for (int k = 0; k < 4; k++) {
		printf("%02X ", arr[k]);
	}
	printf("\n");
}

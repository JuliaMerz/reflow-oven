#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

main() {
    
}

void init() {
    bcm2835_spi_begin();
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
    //BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192,    ///< 8192 = 32.768us = 30/51757813kHz
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8192);
    

	//	BCM2835_SPI_MODE0 = 0,  // CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
	//	BCM2835_SPI_MODE1 = 1,  // CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	//	BCM2835_SPI_MODE2 = 2,  // CPOL = 1, CPHA = 0, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	//	BCM2835_SPI_MODE3 = 3,  // CPOL = 1, CPHA = 1, Clock idle low, data is clocked in on rising, edge output data (change) on falling edge
    
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);    
}

/**
 * Returns 4 times the actual temperature, because screw it.
 */
float getThermoCoupleTemp() {
    uint32_t DATALEN = 4;
    char retbytes[10];
    char tbuf = {0xFF, 0xFF, 0xFF, 0xFF};
    
    int32_t *temp = retbytes;
    int32_t value_times_4;
    float actual;

    bcm2835_spi_transfernb(tbuf, retbytes, DATALEN);

    // Get the temperature from the first 4 chars, then right shift so only the
    // temperature remains.
    value_times_4 = *temp >> 18;
    // Divide by 4 to make it a float.
    actual = value_times_4/4.0f;

    return actual;
}

void cleanup() {
    bcm2835_spi_end();
}

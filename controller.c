#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

int main() {
    int n = 100;
    int TEMP = 1000;
    if (!init()) {
        //TODO Print error message failed on init code
        return 1;
    }
    
    //Testing loop
    while (n > 0) {
        printf("Current temperature: %f\n" % getThermoCoupleTemp());
        if (set_to_temp(TEMP)) {
            n--;
        }
    }
    
    if (!cleanup()) {
        //todo raise error
    } else {
        return 0
    }
}

int init() {
	if (!bcm2835_init()) {
		return 1;
	}
    bcm2835_spi_begin();
    
    //BCM2835_SPI_CS0 = Chip Select 0 // 0 = Active (CS low when TX/RX)
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
    
    //BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192, // 8192 = 32.768us = 30.51757813kHz
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8192);
    
    //BCM2835_SPI_MODE0 = 0,  // CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
}

/**
 * @return The thermocouple temperature as a float.
 */
float getThermoCoupleTemp() {
    uint32_t DATALEN = 4;
    char tbuf = {0x00, 0x00, 0x00, 0x00};
    bcm2835_spi_transfern(tbuf, len); //data is now loaded into tbuf

    uint16_t res = tbuf[0] << 8 | (tbuf[1] & 0xFF);
    float tc_temp = (res >> 2) / 4.00f; //Does not support negative temperatures TODO future release
  
    //Error Checking
    if (tbuf[1] & 0x02 || tbuf[3] & 0x08) {
        //programming error dummy (reserved bits high)
    } else if (tbuf[1] & 0x01) {
        //General error, let's probe more
        if (tbuf[3] & 0x04) {
            //print short to vcc
        }
        if (tbuf[3] & 0x02) {
            //print short to gnd
        }
        if (tbuf[3] & 0x01) {
            //print open circuit
        }
        return -1f;
    }
    return tc_temp;
}

int set_to_temp(int temp) {
    if (temp < getThermoCoupleTemp() { 
        bcm2835_gpio_clr(RPI_V2_GPIO_P1_11);
        return 0;
    } else {
        bcm2835_gpio_set(RPI_V2_GPIO_P1_11);
        return 1;
    }
}

void cleanup() {
    return bcm2835_spi_end();
}

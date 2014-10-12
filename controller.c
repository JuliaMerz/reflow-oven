#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

double getThermoCoupleTemp();

int main(int argc, char **argv) {
    printf("Got here.\n");
    int n = 1000;
    int TEMP = 1000;
    init();
    while(n > 0){
     //printf("Current temperature: %f\n",  getThermoCoupleTemp());
     //if(set_to_temp()){
	//printf("Reading a byte %02X\n", get());
        get();
	n--;
     //}
    }
    cleanup();
    return 0;
}
void get() {
	uint8_t data = bcm2835_spi_transfer(0xFF);
	printf("getting a fucking %02X\n", data);
	printf(data);
}

 void init() {
     if(!bcm2835_init()){
        return 1;
     }
     bcm2835_spi_begin();
     //bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 1);
     //BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192,    ///< 8192 = 32.768us = 30/51757813kHz
     //bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8192);
     
 
	// //	BCM2835_SPI_MODE0 = 0,  // CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
	// //	BCM2835_SPI_MODE1 = 1,  // CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	// //	BCM2835_SPI_MODE2 = 2,  // CPOL = 1, CPHA = 0, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	// //	BCM2835_SPI_MODE3 = 3,  // CPOL = 1, CPHA = 1, Clock idle low, data is clocked in on rising, edge output data (change) on falling edge
     
     //bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);    }
 
 /**
  * Returns 4 times the actual temperature, because screw it.
  */
 double getThermoCoupleTemp() {
     uint32_t DATALEN = 4;
     char retbytes[10] = {0};
     char tbuf[4] = {0x00, 0x00, 0x00, 0x00};
     
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
 
 int set_to_temp(int temp){
     if(temp < getThermoCoupleTemp()){
         bcm2835_gpio_clr(RPI_V2_GPIO_P1_11);
         return 0;
     }else{
         bcm2835_gpio_set(RPI_V2_GPIO_P1_11);
         return 1;
     }
 }
 
 void cleanup() {
     bcm2835_spi_end();
     bcm2835_close();
 }

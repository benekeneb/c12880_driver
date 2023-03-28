#include "ads1115.h"
#include "main.h"
#include "stm32f3xx_hal_i2c.h"

extern I2C_HandleTypeDef hi2c3;
extern uint16_t adc_data_rate;
extern uint8_t adc_mode;

int16_t adcRead()
{
	uint8_t first_conf_byte = 0b11000001;
	switch(adc_mode){
		case 0: 	first_conf_byte = 0b11000000; break; //continuous conversion
		case 1: 	first_conf_byte = 0b11000001; break; //single shot
	}

	uint8_t sec_conf_byte = 0b10000011;
	switch(adc_data_rate){
		case 8: 	sec_conf_byte = 0b00000011; break;
		case 16: 	sec_conf_byte = 0b00100011; break;
		case 32: 	sec_conf_byte = 0b01000011; break;
		case 64: 	sec_conf_byte = 0b01100011; break;
		case 128: 	sec_conf_byte = 0b10000011; break;
		case 250: 	sec_conf_byte = 0b10100011; break;
		case 475: 	sec_conf_byte = 0b11000011; break;
		case 860: 	sec_conf_byte = 0b11100011; break;
	}


    unsigned char ADSwrite[6];
    uint8_t address = 0b1001000;
    ADSwrite[0] = 0x01;

    // Channel 0
    ADSwrite[1] = first_conf_byte;
    ADSwrite[2] = sec_conf_byte;
    HAL_I2C_Master_Transmit(&hi2c3, address << 1, ADSwrite, 3, 100);

    ADSwrite[0] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c3, address << 1, ADSwrite, 1, 100);

    HAL_I2C_Master_Receive(&hi2c3, address << 1, ADSwrite, 2, 100);
    return (ADSwrite[0] << 8 | ADSwrite[1]);
}

int16_t adcReadVoltage(){
	int16_t adc_res = adcRead();
	int16_t voltage = adc_res * 6144 * 2 / 65536;
	return voltage;
}

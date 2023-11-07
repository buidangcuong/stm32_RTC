#ifndef _RTC_C
#define _RTC_C

#include "stm32f1xx_hal.h"

#include "rtc.h"

int8_t RTC_Init(I2C_HandleTypeDef hi)						// The function of initialization RTC
{
	uint8_t temp;
	
#ifdef	RTC_DS3231	
	// Clock start	
	if(RTC_Read(hi,0x0E,&temp)) return -1;
	temp &= ~(1 << 7);
	if(RTC_Write(hi,0x0E,temp)) return -1;
	
	// Enable 1 Hz signal on SQW output
	if(RTC_Read(hi,0x0E,&temp)) return -1;
	temp &= ~((1 << 4) | (1 << 3) | ( 1 << 2));				
	if(RTC_Write(hi,0x0E,temp)) return -1;
#endif
	
#ifdef	RTC_DS1307
	// Clock start	
	if(RTC_Read(0x00,&temp)) return -1;
	temp &= ~(1 << 7);
	if(RTC_Write(0x00,temp)) return -1;
	
	// Enable 1 Hz signal on SQW output	
	if(RTC_Read(0x07,&temp)) return -1;
	temp &= ~((1 << 1) | (1 << 0));
	temp |= (1<<4);
	if(RTC_Write(0x07,temp)) return -1;
#endif
	return 0;
} // RTC_Init

int8_t RTC_Write(I2C_HandleTypeDef hi, uint8_t address, uint8_t data)		// The function of writing data to RTC
{
	uint8_t TxBuffer[2];
	TxBuffer[0]=address;
	TxBuffer[1]=data;

	if(HAL_I2C_Master_Transmit(&hi,(RTC_ADDR), TxBuffer, 2, 100) !=HAL_OK)	return -1;
	
	return 0;
} // RTC_Write

int8_t RTC_Read(I2C_HandleTypeDef hi, uint8_t address, uint8_t *data)		// The function of reading data from RTC
{
	uint8_t i;
	i = address;

	if(HAL_I2C_Master_Transmit(&hi,(RTC_ADDR), &i, 1, 100) != HAL_OK)	return -1;
	if(HAL_I2C_Master_Receive(&hi,(RTC_ADDR), data, 1, 100) != HAL_OK)	return -1;
	
	return 0;
} // RTC_Read

#endif // RTC

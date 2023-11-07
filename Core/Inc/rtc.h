#ifndef _RTC_H
#define _RTC_H

// #define	RTC_DS1307						// Uncomment this line for DS3107
#define		RTC_DS3231						// Comment this line for DS3107

#include "stm32f1xx_hal.h"

#define RTC_ADDR		(0x68<<1)
int8_t RTC_Init(I2C_HandleTypeDef hi);					// The function of initialization RTC
int8_t RTC_Read(I2C_HandleTypeDef hi, uint8_t address, uint8_t *data);	// The function of reading data from RTC
int8_t RTC_Write(I2C_HandleTypeDef hi, uint8_t address, uint8_t data);	// The function of writing data to RTC

#endif

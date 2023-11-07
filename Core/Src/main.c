/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_16x2.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern I2C_HandleTypeDef hi2c1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char dateTimeBuffer[50]; 
#define DS1307_I2C_ADDRESS 0xD0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//uint8_t dectobcd(int val){

//   return(uint8_t)(val/10*16)+(val%10);
//}
//int bcd_to_decimal(uint8_t val){
//   return (int)(val/16*10)+(val%10);
// 
//}
//typedef struct{
//uint8_t hours ;
//uint8_t minutes ;
//uint8_t seccond ;
//uint8_t date ;
//uint8_t month ;
//uint8_t year;
//} TIME;
//TIME time ;

//void Set_time(uint8_t sec ,uint8_t min, uint8_t hour,uint8_t dow, uint8_t dom,uint8_t mouth,uint8_t year)
//{
//  uint8_t set_time[7];
//	set_time[0] = dectobcd(sec);
//	set_time[1] = dectobcd(min);
//	set_time[2] = dectobcd(hour);
//	set_time[3] = dectobcd(dow);
//	set_time[4] = dectobcd(dom);
//	set_time[5] = dectobcd(mouth);
//  set_time[6] = dectobcd(year);
//	
//HAL_I2C_Mem_Write(&hi2c1,0x68<<1,0x00,1,set_time,7,1000);
//}
//void Get_Time(void){

//   uint8_t get_time[7];
//	HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x00,1,get_time,7,1000);
//  time.seccond = bcd_to_decimal(get_time[0]);
//	time.minutes = bcd_to_decimal(get_time[1]);
//	time.hours = bcd_to_decimal(get_time[2]);
//	time.month =bcd_to_decimal(get_time[5]);
//	time.year = bcd_to_decimal(get_time[6]);
//}
//char buffer[10];
uint8_t bcdToDec(uint8_t value)
{
  return ((value / 16 * 10) + (value % 16));
}
uint8_t decToBcd(uint8_t val)
{
  return ((val / 10 * 16) + (val % 10));
}
void readTime(uint8_t *hour, uint8_t *minute, uint8_t *second)
{
  uint8_t buffer[3];
  

  HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, buffer, 3, HAL_MAX_DELAY);
  
 
  *second = bcdToDec(buffer[0] & 0x7F);
  *minute = bcdToDec(buffer[1]);
  *hour = bcdToDec(buffer[2] & 0x3F);
}

void SetTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  uint8_t timeData[3];
  
  timeData[0] = decToBcd(second);
  timeData[1] = decToBcd(minute);
  timeData[2] = decToBcd(hour);
  
  HAL_I2C_Mem_Write(&hi2c1, DS1307_I2C_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, timeData, 3, HAL_MAX_DELAY);
}
void readDate(uint8_t *day, uint8_t *month, uint8_t *year)
{
  uint8_t buffer[3];
  
 	 HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDRESS, 0x04, I2C_MEMADD_SIZE_8BIT, buffer, 3, HAL_MAX_DELAY);
  *day = bcdToDec(buffer[0]);
  *month = bcdToDec(buffer[1]);
  *year = bcdToDec(buffer[2]);

  
}
void SetDate(uint8_t day, uint8_t month, uint8_t year)
{
  uint8_t dateData[3];
  
  dateData[0] = decToBcd(day);
  dateData[1] = decToBcd(month);
  dateData[2] = decToBcd(year);
  
  HAL_I2C_Mem_Write(&hi2c1, DS1307_I2C_ADDRESS, 0x04, I2C_MEMADD_SIZE_8BIT, dateData, 3, HAL_MAX_DELAY);
}
void displayTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  char timeString[13];
  
  
  sprintf(timeString, "Time:%02d:%02d:%02d", hour, minute, second);

  Lcd_gotoxy(1,1);
 Lcd_write_string(timeString);

}
void displayDate(uint8_t day, uint8_t month, uint8_t year)
{
  char dateString[13];
  
  sprintf(dateString, "Date:%02d/%02d/%02d", day, month, year);

  Lcd_gotoxy(0, 0);
  Lcd_write_string(dateString);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

 Lcd_Init();
 HAL_I2C_Init(&hi2c1);
  SetTime(22,37,10); 
	SetDate(11,3,23);
// Set_time(0x23,0x59,0x50,5,3,1,20);
//  HAL_I2C_Mem_Write(&hi2c1,0x68<<1,0x00,1,&seccond,1,50);
//	HAL_I2C_Mem_Write(&hi2c1,0x68<<1,0x01,1,&minutes,1,50);
//  HAL_I2C_Mem_Write(&hi2c1,0x68<<1,0x02,1,&hours,1,50);
//		
//		HAL_I2C_Mem_Write(&hi2c1,0x68<<1,0x04,1,&date,1,50);
//		HAL_I2C_Mem_Write(&hi2c1,0x68<<1,0x05,1,&month,1,50);
//		HAL_I2C_Mem_Write(&hi2c1,0x68<<1,0x06,1,&year,1,50);

//		
//		sprintf((char*)time, "Time:%02d:%02d:%02d\r\n",seccond,minutes,hours);
//    Lcd_write_string((char*)time); 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
//    HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x00,1,&seccond,1,1000);
//	  HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x01,1,&minutes,1,50);
//    HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x02,1,&hours,1,50);
//		
//		HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x04,1,&date,1,50);
//		HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x05,1,&month,1,50);
//		HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x06,1,&year,1,50);

//		Lcd_gotoxy(0,0);
//		sprintf((char*)time, "Time:%02d:%02d:%02d\r\n",hours,minutes,seccond);
//    Lcd_write_string((char*)time); 
//		Get_Time();
//		
//		sprintf(buffer,"%02d:%02d:%02d",time.hours,time.minutes,time.seccond);
//		Lcd_gotoxy(0,0);
//		Lcd_write_string(buffer);
//		HAL_Delay(500);
		uint8_t hour, minute, second;
    
   
    readTime(&hour, &minute, &second);
    
  
    displayTime(hour, minute, second);
		
		uint8_t day, month, year;
    
   readDate(&day, &month, &year);
    
   displayDate(day, month, year);
  sprintf(dateTimeBuffer, "Date:%02d/%02d/%02d\n Time:%02d:%02d:%02d", day, month, year, hour, minute, second);


HAL_UART_Transmit(&huart1, (uint8_t*)dateTimeBuffer, strlen(dateTimeBuffer), HAL_MAX_DELAY);
    HAL_Delay(1000); 
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "BSP_BMP280.h"
#include "BSP_BMP280_Ex.h"
#include "BSP_LED.h"
#include <stdarg.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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
BMP280_HandleTypedef bmp280;

float pressure, temperature, humidity;

uint16_t size;
uint8_t Data[256];

int LOG_INFO (const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char buf[256];
	size_t len = vsnprintf (buf, sizeof buf, format, ap);
	va_end(ap);
	if (len > sizeof(buf) - 1)
		len = sizeof(buf) - 1;

	if (HAL_OK == HAL_UART_Transmit (&huart1, buf, len, 1000))
	{
		return len;
	}
	return -1;
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_I2C1_CLK_ENABLE();
  __HAL_RCC_I2C2_CLK_ENABLE();

  MX_GPIO_Init();
//  MX_I2C1_Init();
//  MX_I2C2_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	LOG_INFO("[I2C1] Reading PIN I2C1 SCL %d \r\n", HAL_GPIO_ReadPin(GPIOB, I2C_SCL_Pin));
	LOG_INFO("[I2C1] Reading PIN I2C1 SDA %d \r\n", HAL_GPIO_ReadPin(GPIOB, I2C_SDA_Pin));



//	if (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c2, BMP280_I2C_ADDRESS_0 << 1 , 10, 100))
//	{
//		bmp280_init_default_params(&bmp280.params);
//		bmp280.addr = BMP280_I2C_ADDRESS_0;
//		bmp280.i2c = &hi2c2;
//		LOG_INFO("Device 0 Ready\r\n");
//	}
//	else if (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c2, BMP280_I2C_ADDRESS_1 << 1 , 10, 100))
//	{
//		bmp280_init_default_params(&bmp280.params);
//		bmp280.addr = BMP280_I2C_ADDRESS_1;
//		bmp280.i2c = &hi2c2;
//		LOG_INFO("Device 1 Ready\r\n");
//	}else{
//		LOG_INFO("Device Failure\r\n");
//
//	}



//	for(int i=1; i<=127; i++)
//	{
//		LOG_INFO("[I2C1] Scanning %d \r\n", i);
//
//		if (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c1, i , 10, 100))
//			{
//
//				LOG_INFO("[I2C1] Device %d Ready\r\n", i);
//			}
//	}
//
//	for(int i=1; i<=127; i++)
//	{
//		LOG_INFO("[I2C2] Scanning %d \r\n", i);
//
//		if (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c2, i , 10, 100))
//			{
//
//			LOG_INFO("[I2C2] Device %d Ready\r\n", i);
//			}
//	}

//
//	while (!bmp280_init(&bmp280, &bmp280.params)) {
//		size = sprintf((char *)Data, "BMP280 initialization failed\n");
//		HAL_UART_Transmit(&huart1, Data, size, 1000);
//		HAL_Delay(2000);
//	}
//	bool bme280p = bmp280.id == BME280_CHIP_ID;
//	size = sprintf((char *)Data, "BMP280: found %s\n", bme280p ? "BME280" : "BMP280");
//	HAL_UART_Transmit(&huart1, Data, size, 1000);
//	BMP280_Init();
//	LOG_INFO("Init Done");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		HAL_Delay(500);
//		while (!bmp280_read_float(&bmp280, &temperature, &pressure, &humidity)) {
//			size = sprintf((char *)Data,
//					"Temperature/pressure reading failed\n");
//			HAL_UART_Transmit(&huart1, Data, size, 1000);
//			HAL_Delay(100);
//		}
//
//		size = sprintf((char *)Data,"Pressure: %.2f Pa, Temperature: %.2f C", pressure, temperature);
//		HAL_UART_Transmit(&huart1, Data, size, 1000);
//		if (bme280p) {
//			size = sprintf((char *)Data,", Humidity: %.2f\n", humidity);
//			HAL_UART_Transmit(&huart1, Data, size, 1000);
//		}
//
//		else {
//			size = sprintf((char *)Data, "\n");
//			HAL_UART_Transmit(&huart1, Data, size, 1000);
//		}
//
//		LOG_INFO("Looping");
//
//		long temperature = 0;
//		long pressure = 0;
//
//		bmp280Convert(&temperature, &pressure);
//		LOG_INFO("Temperature: %d, Pressure: %d\r\n", (int)temperature/10, (int)pressure);

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
	__disable_irq ();
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


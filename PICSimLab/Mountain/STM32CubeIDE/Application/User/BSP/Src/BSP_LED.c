/*
 * BSP_LED.c
 *
 *  Created on: Nov 14, 2022
 *      Author: may
 */


#include "BSP_LED.h"

#include "main.h"
#include "gpio.h"

#define _GPIO(f) f##_GPIO_Port, f##_Pin

void BSP_LED_Start()
{
	MX_GPIO_Init();
}
void BSP_LED_On()
{
	HAL_GPIO_WritePin(_GPIO(LED), GPIO_PIN_SET);
}
void BSP_LED_Off()
{
	HAL_GPIO_WritePin(_GPIO(LED), GPIO_PIN_RESET);

}
void BSP_LED_Toggle()
{
	GPIO_PinState LED_Status = HAL_GPIO_ReadPin(_GPIO(LED));
	HAL_GPIO_WritePin(_GPIO(LED), LED_Status == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET );

}

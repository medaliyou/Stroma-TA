/*
 * BSP_I2C.h
 *
 *  Created on: Nov 14, 2022
 *      Author: may
 */

#ifndef APPLICATION_USER_BSP_INC_BSP_I2C_H_
#define APPLICATION_USER_BSP_INC_BSP_I2C_H_

#include "main.h"

HAL_StatusTypeDef BSP_I2C_Start (void);

HAL_StatusTypeDef BSP_I2C_Write (uint16_t Device, const uint8_t *pWrData,
																 uint16_t nWrSize);

HAL_StatusTypeDef BSP_I2C_Read (uint16_t Device, uint8_t *pRdData,
																uint16_t nRdSize);

HAL_StatusTypeDef BSP_I2C_WriteRead (uint16_t Device, const uint8_t *pWrData,
																		 uint16_t nWrSize, uint8_t *pRdData,
																		 uint16_t nRdSize);

HAL_StatusTypeDef BSP_I2C_ReadRegister16 (uint16_t Device, uint8_t RegAddr,
																					uint16_t *pRdData);

HAL_StatusTypeDef BSP_I2C_ReadData (uint16_t Device, uint8_t RegAddr,
																		uint16_t *pRdData, uint16_t nRdSize);

HAL_StatusTypeDef BSP_I2C_WriteRegister8 (uint16_t Device, uint8_t RegAddr,
																					const uint8_t *pWrData);

#endif /* APPLICATION_USER_BSP_INC_BSP_I2C_H_ */

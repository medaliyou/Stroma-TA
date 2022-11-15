/*
 * BSP_I2C.c
 *
 *  Created on: Nov 14, 2022
 *      Author: may
 */

#include "BSP_I2C.h"
#include "i2c.h"

#define ARRAY_REF(x) x, sizeof(x)

extern I2C_HandleTypeDef hi2c1;

volatile uint32_t v_I2C_Wait = 0;
volatile HAL_StatusTypeDef v_I2C_Status = HAL_ERROR;

HAL_StatusTypeDef BSP_I2C_Wait (void)
{

	while (!v_I2C_Wait)
	{
	}

	v_I2C_Wait = 0;
	return v_I2C_Status;
}

void BSP_I2C_Notify (HAL_StatusTypeDef tStatus)
{
	v_I2C_Status = tStatus;
	v_I2C_Wait++;
}

//HAL_StatusTypeDef BSP_I2C_ReadRegister16 (uint16_t Device, uint8_t RegAddr,
//																					uint16_t *pRdData)
//{
//
//	HAL_StatusTypeDef tStatus = HAL_ERROR;
//
//	if (NULL == pRdData)
//		return HAL_ERROR;
//
//	uint8_t rx_buff[2] = { 0 };
//
//	tStatus = HAL_I2C_Mem_Read_IT (&hi2c1, Device, RegAddr, 1, pRdData, 2);
//	if (HAL_OK == tStatus)
//	{
//		tStatus = BSP_I2C_Wait ();
//	}
//	return tStatus;
//}

HAL_StatusTypeDef BSP_I2C_ReadRegister16 (uint16_t Device, uint8_t RegAddr,
																					uint16_t *pRdData)
{

	HAL_StatusTypeDef tStatus = HAL_ERROR;

	if (NULL == pRdData)
		return HAL_ERROR;

	uint8_t rx_buff[2] = { 0 };

	tStatus = HAL_I2C_Mem_Read (&hi2c1, Device, RegAddr, 1, pRdData, 2, 1000);
//	if (HAL_OK == tStatus)
//	{
//		tStatus = BSP_I2C_Wait ();
//	}
	return tStatus;
}

//HAL_StatusTypeDef BSP_I2C_ReadData (uint16_t Device, uint8_t RegAddr,
//																		uint16_t *pRdData, uint16_t nRdSize)
//{
//
//	HAL_StatusTypeDef tStatus = HAL_ERROR;
//
//	if ((nRdSize > 0) && (NULL == pRdData))
//		return HAL_ERROR;
//
//	uint8_t rx_buff[2] = { 0 };
//
//	tStatus = HAL_I2C_Mem_Read_IT (&hi2c1, Device, RegAddr, 1, pRdData, nRdSize);
//	if (HAL_OK == tStatus)
//	{
//		tStatus = BSP_I2C_Wait ();
//	}
//	return tStatus;
//}

HAL_StatusTypeDef BSP_I2C_ReadData (uint16_t Device, uint8_t RegAddr,
																		uint16_t *pRdData, uint16_t nRdSize)
{

	HAL_StatusTypeDef tStatus = HAL_ERROR;

	if ((nRdSize > 0) && (NULL == pRdData))
		return HAL_ERROR;

	uint8_t rx_buff[2] = { 0 };

	tStatus = HAL_I2C_Mem_Read (&hi2c1, Device, RegAddr, 1, pRdData, nRdSize, 1000);

	return tStatus;
}

//HAL_StatusTypeDef BSP_I2C_WriteRegister8 (uint16_t Device, uint8_t RegAddr,
//																					const uint8_t *pWrData)
//{
//
//	HAL_StatusTypeDef tStatus = HAL_ERROR;
//
//	if (NULL == pWrData)
//		return HAL_ERROR;
//
//	uint8_t rx_buff[2] = { 0 };
//
//	tStatus = HAL_I2C_Mem_Write_IT (&hi2c1, Device, RegAddr, 1, pWrData, 1);
//	if (HAL_OK == tStatus)
//	{
//		tStatus = BSP_I2C_Wait ();
//	}
//	return tStatus;
//}

HAL_StatusTypeDef BSP_I2C_WriteRegister8 (uint16_t Device, uint8_t RegAddr,
																					const uint8_t *pWrData)
{

	HAL_StatusTypeDef tStatus = HAL_ERROR;

	if (NULL == pWrData)
		return HAL_ERROR;

	uint8_t rx_buff[2] = { 0 };

	tStatus = HAL_I2C_Mem_Write_IT (&hi2c1, Device, RegAddr, 1, pWrData, 1);

	return tStatus;
}


HAL_StatusTypeDef BSP_I2C_Write (uint16_t Device, const uint8_t *pWrData,
																 uint16_t nWrSize)
{
	HAL_StatusTypeDef tStatus;

	if ((nWrSize > 0) && (NULL == pWrData))
		return HAL_ERROR;

	tStatus = HAL_I2C_Master_Transmit_IT (&hi2c1, Device, (uint8_t*) pWrData,
																				nWrSize);
	if (HAL_OK == tStatus)
	{
		tStatus = BSP_I2C_Wait ();
	}
	return tStatus;
}

HAL_StatusTypeDef BSP_I2C_Read (uint16_t Device, uint8_t *pRdData,
																uint16_t nRdSize)
{
	HAL_StatusTypeDef tStatus;
	if ((nRdSize > 0) && (NULL == pRdData))
		return HAL_ERROR;

	tStatus = HAL_I2C_Master_Receive_IT (&hi2c1, Device, pRdData, nRdSize);
	if (HAL_OK == tStatus)
	{
		tStatus = BSP_I2C_Wait ();
	}

	return tStatus;
}

HAL_StatusTypeDef BSP_I2C_WriteRead (uint16_t Device, const uint8_t *pWrData,
																		 uint16_t nWrSize, uint8_t *pRdData,
																		 uint16_t nRdSize)
{
	HAL_StatusTypeDef tStatus;

	if ((nWrSize > 0) && (NULL == pWrData))
		return HAL_ERROR;

	if ((nRdSize > 0) && (NULL == pRdData))
		return HAL_ERROR;

	tStatus = HAL_I2C_Master_Transmit_IT (&hi2c1, Device, (uint8_t*) pWrData,
																				nWrSize);
	if (HAL_OK == tStatus)
	{
		tStatus = BSP_I2C_Wait ();
		if (HAL_OK == tStatus)
		{
			tStatus = HAL_I2C_Master_Receive_IT (&hi2c1, Device, pRdData, nRdSize);
			if (HAL_OK == tStatus)
			{
				tStatus = BSP_I2C_Wait ();
			}
		}
	}

	return tStatus;
}

void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef *hi2c)
{
	BSP_I2C_Notify (HAL_OK);
}

void HAL_I2C_MasterRxCpltCallback (I2C_HandleTypeDef *hi2c)
{
	BSP_I2C_Notify (HAL_OK);
}

void HAL_I2C_ErrorCallback (I2C_HandleTypeDef *hi2c)
{
	BSP_I2C_Notify (HAL_ERROR);
}

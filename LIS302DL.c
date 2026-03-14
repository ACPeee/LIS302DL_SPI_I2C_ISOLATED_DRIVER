#include "LIS3DSHTR.h"

LIS_Handle lis;
LIS_value lis3dsh;

float sensitivity;

HAL_StatusTypeDef LIS_WriteReg(LIS_Handle *dev,uint8_t reg,uint8_t data,uint8_t data_size)
{
	if(dev->interface == LIS_I2C)
	{
		return HAL_I2C_Mem_Write(dev->hi2c,dev->address,reg,I2C_MEMADD_SIZE_8BIT,&data,data_size,HAL_MAX_DELAY);
	}
	else
	{
		uint8_t addr;

		addr = reg & 0x3F;

		if(data_size > 1)
		addr |= 0x40;

		uint8_t tx[2];

		tx[0] = addr;
		tx[1] = data;

		HAL_GPIO_WritePin(dev->cs_port,dev->cs_pin,GPIO_PIN_RESET);
		HAL_SPI_Transmit(dev->hspi,tx,2,HAL_MAX_DELAY);
		HAL_GPIO_WritePin(dev->cs_port,dev->cs_pin,GPIO_PIN_SET);

		return HAL_OK;
	}
}


HAL_StatusTypeDef LIS_ReadReg(LIS_Handle *dev,uint8_t reg,uint8_t *data,uint8_t data_size)
{
	if(dev->interface == LIS_I2C)
	{
		return HAL_I2C_Mem_Read(dev->hi2c,dev->address,reg,I2C_MEMADD_SIZE_8BIT,data,data_size,HAL_MAX_DELAY);
	}
	else
	{
		uint8_t addr;

		addr = reg | 0x80;

		if(data_size > 1)
		addr |= 0x40;

		HAL_GPIO_WritePin(dev->cs_port,dev->cs_pin,GPIO_PIN_RESET);

		HAL_SPI_Transmit(dev->hspi,&addr,1,HAL_MAX_DELAY);
		HAL_SPI_Receive(dev->hspi,data,data_size,HAL_MAX_DELAY);

		HAL_GPIO_WritePin(dev->cs_port,dev->cs_pin,GPIO_PIN_SET);

		return HAL_OK;
	}
}


uint8_t LIS_WHO_AM_I(LIS_Handle *dev)
{
	uint8_t id;

	LIS_ReadReg(dev,WHO_AM_I,&id,1);

	return id;
}


HAL_StatusTypeDef LIS_Init(LIS_Handle *dev,ODR_Value odr,LIS_Scale_Value scale)
{
	switch(scale)
	{
		case LIS_SCALE_2G:
		sensitivity = 0.018f;
		break;

		case LIS_SCALE_8G:
		sensitivity = 0.072f;
		break;
	}

	LIS_WriteReg(dev,CTRL_REG1,odr | 0x07,1);

	return HAL_OK;
}


HAL_StatusTypeDef LIS_ReadXYZ(LIS_Handle *dev)
{
	uint8_t x,y,z;

	LIS_ReadReg(dev,OUT_X,&x,1);
	LIS_ReadReg(dev,OUT_Y,&y,1);
	LIS_ReadReg(dev,OUT_Z,&z,1);

	lis3dsh.accel_x = (int8_t)x;
	lis3dsh.accel_y = (int8_t)y;
	lis3dsh.accel_z = (int8_t)z;

	return HAL_OK;
}


void LIS_AccelToG(float *x,float *y,float *z)
{
	*x = lis3dsh.accel_x * sensitivity;
	*y = lis3dsh.accel_y * sensitivity;
	*z = lis3dsh.accel_z * sensitivity;
}


void LIS_AccelTo_ms2(float *x,float *y,float *z)
{
	*x = lis3dsh.accel_x * sensitivity * 9.80665f;
	*y = lis3dsh.accel_y * sensitivity * 9.80665f;
	*z = lis3dsh.accel_z * sensitivity * 9.80665f;
}

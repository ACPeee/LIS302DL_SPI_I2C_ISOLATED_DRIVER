#ifndef __LIS302DL_H
#define __LIS302DL_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "math.h"

#define WHO_AM_I 0x0F

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22

#define OUT_X 0x29
#define OUT_Y 0x2B
#define OUT_Z 0x2D

typedef enum
{
    LIS_I2C,
    LIS_SPI
}LIS_Interface;

typedef struct{
	LIS_Interface interface;
	I2C_HandleTypeDef *hi2c;
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	uint8_t address;
}LIS_Handle;

typedef enum{
  LIS_100_HZ_Freq = 0x47,
  LIS_400_HZ_Freq = 0xC7
} ODR_Value;

typedef struct{
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
}LIS_value;

typedef enum{
	LIS_SCALE_2G  = 0x00,
	LIS_SCALE_8G  = 0x20
}LIS_Scale_Value;

extern LIS_Handle lis;
extern LIS_value lis3dsh;

HAL_StatusTypeDef LIS_WriteReg(LIS_Handle *dev,uint8_t reg,uint8_t data,uint8_t data_size);
HAL_StatusTypeDef LIS_ReadReg(LIS_Handle *dev,uint8_t reg,uint8_t *data,uint8_t data_size);

uint8_t LIS_WHO_AM_I(LIS_Handle *dev);

HAL_StatusTypeDef LIS_Init(LIS_Handle *dev,ODR_Value odr,LIS_Scale_Value scale);

HAL_StatusTypeDef LIS_ReadXYZ(LIS_Handle *dev);

void LIS_AccelToG(float *x,float *y,float *z);
void LIS_AccelTo_ms2(float *x,float *y,float *z);

#endif

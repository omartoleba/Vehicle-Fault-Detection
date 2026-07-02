/******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.h
 *
 * Description: Source file for the AVR ADC driver
 *
 * Author: Omar Toleba
 *
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                              Definitions                           		   *
 *******************************************************************************/

#define ADC_REF_VOLT_VALUE					5
#define ADC_MAXIMUM_VALUE					1023


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	DIVISION_FACTOR_2 = 1 , DIVISION_FACTOR_4 , DIVISION_FACTOR_8 \
	, DIVISION_FACTOR_16 , DIVISION_FACTOR_32 , DIVISION_FACTOR_64 \
	, DIVISION_FACTOR_128
}ADC_PrescalerType;

typedef enum
{
	EXTERNAL_VREF , AVCC , INTERNAL_VREF = 3
}ADC_VREF;

typedef struct
{
	ADC_PrescalerType factor;
	ADC_VREF vref;
}ADC_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void ADC_init(ADC_ConfigType *Config_ptr);

uint16 ADC_readChannel(uint8 Ch_num);

#endif /* ADC_H_ */

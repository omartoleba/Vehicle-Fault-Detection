/******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the AVR ADC driver
 *
 * Author: Omar Toleba
 *
 *******************************************************************************/

#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h" /* To use the IO Ports Registers */
#include "adc.h"

void ADC_init(ADC_ConfigType *Config_ptr)
{
	/* Voltage Reference select*/

	ADMUX = (ADMUX & 0x3F) | ((Config_ptr -> vref) << 6);


	CLEAR_BIT(ADMUX,MUX4);
	CLEAR_BIT(ADMUX,MUX3);
	CLEAR_BIT(ADMUX,MUX2);
	CLEAR_BIT(ADMUX,MUX1);
	CLEAR_BIT(ADMUX,MUX0);

	/* Enable ADC*/

	SET_BIT(ADCSRA,ADEN);

	/*Disable Interrupt*/

	CLEAR_BIT(ADCSRA,ADIE);

	/*Choose the prescaler 128*/

	ADCSRA = (ADCSRA & 0xF8) | (Config_ptr -> factor);
}


uint16 ADC_readChannel(uint8 Ch_num)
{
	/*insert channel number*/
	ADMUX = (ADMUX & 0xE0) | (Ch_num & 0x07);
	/*Start conversion*/
	SET_BIT(ADCSRA,ADSC);
	/*Polling until ADC finish conversion*/
	while(BIT_IS_CLEAR(ADCSRA,ADIF));
	/*Clear interrupt flag*/
	SET_BIT(ADCSRA,ADIF);

	return ADC;
}


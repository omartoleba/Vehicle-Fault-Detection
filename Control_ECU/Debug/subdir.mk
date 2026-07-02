################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Vehicle_Fault_Detection_Control_ECU.c \
../adc.c \
../dc_motor.c \
../external_eeprom.c \
../gpio.c \
../icu.c \
../lm35_sensor.c \
../pwm.c \
../twi.c \
../uart.c \
../ultrasonic.c 

OBJS += \
./Vehicle_Fault_Detection_Control_ECU.o \
./adc.o \
./dc_motor.o \
./external_eeprom.o \
./gpio.o \
./icu.o \
./lm35_sensor.o \
./pwm.o \
./twi.o \
./uart.o \
./ultrasonic.o 

C_DEPS += \
./Vehicle_Fault_Detection_Control_ECU.d \
./adc.d \
./dc_motor.d \
./external_eeprom.d \
./gpio.d \
./icu.d \
./lm35_sensor.d \
./pwm.d \
./twi.d \
./uart.d \
./ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



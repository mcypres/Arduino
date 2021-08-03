################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility/twi.c 

C_DEPS += \
./Libraries/Wire/utility/twi.c.d 

LINK_OBJ += \
./Libraries/Wire/utility/twi.c.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Wire/utility/twi.c.o: /home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility/twi.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-gcc" -c -g -Os -ffunction-sections -fdata-sections -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/cores/arduino" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/variants/mega" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/SoftwareSerial" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility" -I"/home/marc/Arduino/libraries/Adafruit_BMP085_Library" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '



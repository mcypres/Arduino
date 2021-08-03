################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../YachtComputer/Sensor.cpp \
../YachtComputer/SensorBmp183.cpp 

INO_SRCS += \
../YachtComputer/YachtComputer.ino 

INO_DEPS += \
./YachtComputer/YachtComputer.ino.d 

CPP_DEPS += \
./YachtComputer/Sensor.cpp.d \
./YachtComputer/SensorBmp183.cpp.d 

LINK_OBJ += \
./YachtComputer/Sensor.cpp.o \
./YachtComputer/SensorBmp183.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
YachtComputer/Sensor.cpp.o: ../YachtComputer/Sensor.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/cores/arduino" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/variants/mega" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/SoftwareSerial" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility" -I"/home/marc/Arduino/libraries/Adafruit_BMP085_Library" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

YachtComputer/SensorBmp183.cpp.o: ../YachtComputer/SensorBmp183.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/cores/arduino" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/variants/mega" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/SoftwareSerial" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility" -I"/home/marc/Arduino/libraries/Adafruit_BMP085_Library" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

YachtComputer/YachtComputer.o: ../YachtComputer/YachtComputer.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/cores/arduino" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/variants/mega" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/SoftwareSerial" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility" -I"/home/marc/Arduino/libraries/Adafruit_BMP085_Library" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '



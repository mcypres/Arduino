################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sensor.cpp \
../SensorBmp183.cpp \
../YachtComputer.cpp 

CPP_DEPS += \
./Sensor.cpp.d \
./SensorBmp183.cpp.d \
./YachtComputer.cpp.d 

LINK_OBJ += \
./Sensor.cpp.o \
./SensorBmp183.cpp.o \
./YachtComputer.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
Sensor.cpp.o: ../Sensor.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10604 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/cores/arduino" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/variants/mega" -I"/home/marc/Arduino/libraries/Adafruit_BMP085_Library" -I"/home/marc/Arduino/libraries/Adafruit_HMC5883_Unified" -I"/home/marc/Arduino/libraries/Adafruit_Sensor-master" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/SoftwareSerial" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

SensorBmp183.cpp.o: ../SensorBmp183.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10604 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/cores/arduino" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/variants/mega" -I"/home/marc/Arduino/libraries/Adafruit_BMP085_Library" -I"/home/marc/Arduino/libraries/Adafruit_HMC5883_Unified" -I"/home/marc/Arduino/libraries/Adafruit_Sensor-master" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/SoftwareSerial" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

YachtComputer.cpp.o: ../YachtComputer.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10604 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/cores/arduino" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/variants/mega" -I"/home/marc/Arduino/libraries/Adafruit_BMP085_Library" -I"/home/marc/Arduino/libraries/Adafruit_HMC5883_Unified" -I"/home/marc/Arduino/libraries/Adafruit_Sensor-master" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/Wire/utility" -I"/home/marc/workspace/arduino/arduino-1.6.4/hardware/arduino/avr/libraries/SoftwareSerial" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '



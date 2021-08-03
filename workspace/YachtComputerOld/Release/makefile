################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include arduino/core/subdir.mk
-include YachtComputer/subdir.mk
-include Libraries/Wire/utility/subdir.mk
-include Libraries/Wire/subdir.mk
-include Libraries/SoftwareSerial/subdir.mk
-include Libraries/Adafruit_BMP085_Library/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
arduino.ar \

ELF += \
YachtComputerOld.elf \

EEP += \
YachtComputerOld.eep \

FLASH_IMAGE += \
YachtComputerOld.hex \


# All Target
all: YachtComputerOld

# Tool invocations
YachtComputerOld: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-size" -A "/home/marc/Arduino/workspace/YachtComputerOld/Release/YachtComputerOld.elf"
	@echo 'Finished building target: $@'
	@echo ' '

arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-ar" rcs  "/home/marc/Arduino/workspace/YachtComputerOld/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

YachtComputerOld.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega2560  -o "/home/marc/Arduino/workspace/YachtComputerOld/Release/YachtComputerOld.elf"   $(LINK_OBJ) $(AR)  "/home/marc/Arduino/workspace/YachtComputerOld/Release/arduino.ar" "-L/home/marc/Arduino/workspace/YachtComputerOld/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

YachtComputerOld.eep: $(ELF)
	@echo 'Create eeprom image'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "/home/marc/Arduino/workspace/YachtComputerOld/Release/YachtComputerOld.elf" "/home/marc/Arduino/workspace/YachtComputerOld/Release/YachtComputerOld.eep"
	@echo 'Finished building: $@'
	@echo ' '

YachtComputerOld.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"/home/marc/workspace/arduino/arduino-1.6.4/hardware/tools/avr/bin/avr-objcopy" -O ihex -R .eeprom  "/home/marc/Arduino/workspace/YachtComputerOld/Release/YachtComputerOld.elf" "/home/marc/Arduino/workspace/YachtComputerOld/Release/YachtComputerOld.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_DEPS)$(AR)$(FLASH_IMAGE)$(EEP)$(CXX_DEPS)$(S_UPPER_DEPS)$(ELF)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(INO_DEPS)$(AR_OBJ)$(CPP_DEPS)$(LINK_OBJ)$(C_UPPER_DEPS)$(SIZEDUMMY) YachtComputerOld
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

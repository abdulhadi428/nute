################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sd/kl_ini.cpp \
../sd/sd.cpp 

C_SRCS += \
../sd/ff.c 

OBJS += \
./sd/ff.o \
./sd/kl_ini.o \
./sd/sd.o 

C_DEPS += \
./sd/ff.d 

CPP_DEPS += \
./sd/kl_ini.d \
./sd/sd.d 


# Each subdirectory must supply rules for building sources it contributes
sd/%.o: ../sd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -DSTM32F10X_HD -DVECT_TAB_FLASH -DUSE_STDPERIPH_DRIVER -I../inc -I../sd -I../lib -I../lib/CoreSupport -I../lib/inc -I../lib/startup -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -funsigned-bitfields -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sd/%.o: ../sd/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C++ Compiler'
	arm-none-eabi-g++ -DSTM32F10X_HD -DVECT_TAB_FLASH -DUSE_STDPERIPH_DRIVER -I../inc -I../sd -I../lib -I../lib/CoreSupport -I../lib/inc -I../lib/startup -Os -ffunction-sections -fdata-sections -Wall -std=gnu++0x -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -funsigned-bitfields -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/CoreSupport/core_cm3.c 

OBJS += \
./lib/CoreSupport/core_cm3.o 

C_DEPS += \
./lib/CoreSupport/core_cm3.d 


# Each subdirectory must supply rules for building sources it contributes
lib/CoreSupport/%.o: ../lib/CoreSupport/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -DSTM32F10X_HD -DVECT_TAB_FLASH -DUSE_STDPERIPH_DRIVER -I../inc -I../sd -I../lib -I../lib/CoreSupport -I../lib/inc -I../lib/startup -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -funsigned-bitfields -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



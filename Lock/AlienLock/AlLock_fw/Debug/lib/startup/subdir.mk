################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../lib/startup/startup_stm32f10x_hd.asm 

OBJS += \
./lib/startup/startup_stm32f10x_hd.o 

ASM_DEPS += \
./lib/startup/startup_stm32f10x_hd.d 


# Each subdirectory must supply rules for building sources it contributes
lib/startup/%.o: ../lib/startup/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -DSTM32F10X_HD -DVECT_TAB_FLASH -DUSE_STDPERIPH_DRIVER -I../inc -I../sd -I../lib -I../lib/CoreSupport -I../lib/inc -I../lib/startup -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



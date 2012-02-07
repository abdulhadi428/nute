################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/src/misc.c \
../lib/src/stm32f10x_adc.c \
../lib/src/stm32f10x_dma.c \
../lib/src/stm32f10x_gpio.c \
../lib/src/stm32f10x_i2c.c \
../lib/src/stm32f10x_rcc.c \
../lib/src/stm32f10x_sdio.c \
../lib/src/stm32f10x_spi.c \
../lib/src/stm32f10x_tim.c \
../lib/src/stm32f10x_usart.c 

OBJS += \
./lib/src/misc.o \
./lib/src/stm32f10x_adc.o \
./lib/src/stm32f10x_dma.o \
./lib/src/stm32f10x_gpio.o \
./lib/src/stm32f10x_i2c.o \
./lib/src/stm32f10x_rcc.o \
./lib/src/stm32f10x_sdio.o \
./lib/src/stm32f10x_spi.o \
./lib/src/stm32f10x_tim.o \
./lib/src/stm32f10x_usart.o 

C_DEPS += \
./lib/src/misc.d \
./lib/src/stm32f10x_adc.d \
./lib/src/stm32f10x_dma.d \
./lib/src/stm32f10x_gpio.d \
./lib/src/stm32f10x_i2c.d \
./lib/src/stm32f10x_rcc.d \
./lib/src/stm32f10x_sdio.d \
./lib/src/stm32f10x_spi.d \
./lib/src/stm32f10x_tim.d \
./lib/src/stm32f10x_usart.d 


# Each subdirectory must supply rules for building sources it contributes
lib/src/%.o: ../lib/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -DSTM32F10X_HD -DVECT_TAB_FLASH -DUSE_STDPERIPH_DRIVER -I../inc -I../sd -I../lib -I../lib/CoreSupport -I../lib/inc -I../lib/startup -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -funsigned-bitfields -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cc2500.c \
../src/olwen.c \
../src/time_utils.c 

OBJS += \
./src/cc2500.o \
./src/olwen.o \
./src/time_utils.o 

C_DEPS += \
./src/cc2500.d \
./src/olwen.d \
./src/time_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\nute\Calmari\NeldeCalma\mielofon\firmware_eclipse\inc" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega88 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



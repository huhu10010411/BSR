################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Device/SIM.c \
../Core/Src/Device/ds3231.c 

OBJS += \
./Core/Src/Device/SIM.o \
./Core/Src/Device/ds3231.o 

C_DEPS += \
./Core/Src/Device/SIM.d \
./Core/Src/Device/ds3231.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Device/%.o Core/Src/Device/%.su: ../Core/Src/Device/%.c Core/Src/Device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/blue_pill_pj/Station/Core/Inc" -I"D:/blue_pill_pj/Station/Core/Inc/Application" -I"D:/blue_pill_pj/Station/Core/Inc/Communication" -I"D:/blue_pill_pj/Station/Core/Inc/Device" -I"D:/blue_pill_pj/Station/Core/Inc/Protocol" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Device

clean-Core-2f-Src-2f-Device:
	-$(RM) ./Core/Src/Device/SIM.d ./Core/Src/Device/SIM.o ./Core/Src/Device/SIM.su ./Core/Src/Device/ds3231.d ./Core/Src/Device/ds3231.o ./Core/Src/Device/ds3231.su

.PHONY: clean-Core-2f-Src-2f-Device


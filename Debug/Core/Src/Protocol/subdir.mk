################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Protocol/Lora.c \
../Core/Src/Protocol/MQTT.c 

OBJS += \
./Core/Src/Protocol/Lora.o \
./Core/Src/Protocol/MQTT.o 

C_DEPS += \
./Core/Src/Protocol/Lora.d \
./Core/Src/Protocol/MQTT.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Protocol/%.o Core/Src/Protocol/%.su: ../Core/Src/Protocol/%.c Core/Src/Protocol/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/blue_pill_pj/Station/Core/Inc" -I"D:/blue_pill_pj/Station/Core/Inc/Application" -I"D:/blue_pill_pj/Station/Core/Inc/Communication" -I"D:/blue_pill_pj/Station/Core/Inc/Device" -I"D:/blue_pill_pj/Station/Core/Inc/Protocol" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Protocol

clean-Core-2f-Src-2f-Protocol:
	-$(RM) ./Core/Src/Protocol/Lora.d ./Core/Src/Protocol/Lora.o ./Core/Src/Protocol/Lora.su ./Core/Src/Protocol/MQTT.d ./Core/Src/Protocol/MQTT.o ./Core/Src/Protocol/MQTT.su

.PHONY: clean-Core-2f-Src-2f-Protocol


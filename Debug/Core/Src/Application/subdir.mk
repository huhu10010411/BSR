################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/App_MQTT.c 

OBJS += \
./Core/Src/Application/App_MQTT.o 

C_DEPS += \
./Core/Src/Application/App_MQTT.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/%.o Core/Src/Application/%.su: ../Core/Src/Application/%.c Core/Src/Application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/blue_pill_pj/uart_dma/Core/Inc" -I"D:/blue_pill_pj/uart_dma/Core/Inc/Application" -I"D:/blue_pill_pj/uart_dma/Core/Inc/Communication" -I"D:/blue_pill_pj/uart_dma/Core/Inc/Device" -I"D:/blue_pill_pj/uart_dma/Core/Inc/Protocol" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application

clean-Core-2f-Src-2f-Application:
	-$(RM) ./Core/Src/Application/App_MQTT.d ./Core/Src/Application/App_MQTT.o ./Core/Src/Application/App_MQTT.su

.PHONY: clean-Core-2f-Src-2f-Application


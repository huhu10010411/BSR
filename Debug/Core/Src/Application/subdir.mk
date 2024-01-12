################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/App_Display.c \
../Core/Src/Application/App_MBA_stepmor.c \
../Core/Src/Application/App_MCU.c \
../Core/Src/Application/App_MQTT.c \
../Core/Src/Application/App_SMS.c \
../Core/Src/Application/App_Serial.c 

OBJS += \
./Core/Src/Application/App_Display.o \
./Core/Src/Application/App_MBA_stepmor.o \
./Core/Src/Application/App_MCU.o \
./Core/Src/Application/App_MQTT.o \
./Core/Src/Application/App_SMS.o \
./Core/Src/Application/App_Serial.o 

C_DEPS += \
./Core/Src/Application/App_Display.d \
./Core/Src/Application/App_MBA_stepmor.d \
./Core/Src/Application/App_MCU.d \
./Core/Src/Application/App_MQTT.d \
./Core/Src/Application/App_SMS.d \
./Core/Src/Application/App_Serial.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/%.o Core/Src/Application/%.su: ../Core/Src/Application/%.c Core/Src/Application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/blue_pill_pj/Station/Core/Inc" -I"D:/blue_pill_pj/Station/Core/Inc/Application" -I"D:/blue_pill_pj/Station/Core/Inc/Communication" -I"D:/blue_pill_pj/Station/Core/Inc/Device" -I"D:/blue_pill_pj/Station/Core/Inc/Protocol" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application

clean-Core-2f-Src-2f-Application:
	-$(RM) ./Core/Src/Application/App_Display.d ./Core/Src/Application/App_Display.o ./Core/Src/Application/App_Display.su ./Core/Src/Application/App_MBA_stepmor.d ./Core/Src/Application/App_MBA_stepmor.o ./Core/Src/Application/App_MBA_stepmor.su ./Core/Src/Application/App_MCU.d ./Core/Src/Application/App_MCU.o ./Core/Src/Application/App_MCU.su ./Core/Src/Application/App_MQTT.d ./Core/Src/Application/App_MQTT.o ./Core/Src/Application/App_MQTT.su ./Core/Src/Application/App_SMS.d ./Core/Src/Application/App_SMS.o ./Core/Src/Application/App_SMS.su ./Core/Src/Application/App_Serial.d ./Core/Src/Application/App_Serial.o ./Core/Src/Application/App_Serial.su

.PHONY: clean-Core-2f-Src-2f-Application


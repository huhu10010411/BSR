################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Communication/Serial_CFG.c \
../Core/Src/Communication/Serial_log.c \
../Core/Src/Communication/ServerMessage.c \
../Core/Src/Communication/stationCMD.c 

OBJS += \
./Core/Src/Communication/Serial_CFG.o \
./Core/Src/Communication/Serial_log.o \
./Core/Src/Communication/ServerMessage.o \
./Core/Src/Communication/stationCMD.o 

C_DEPS += \
./Core/Src/Communication/Serial_CFG.d \
./Core/Src/Communication/Serial_log.d \
./Core/Src/Communication/ServerMessage.d \
./Core/Src/Communication/stationCMD.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Communication/%.o Core/Src/Communication/%.su: ../Core/Src/Communication/%.c Core/Src/Communication/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/blue_pill_pj/Station/Core/Inc" -I"D:/blue_pill_pj/Station/Core/Inc/Application" -I"D:/blue_pill_pj/Station/Core/Inc/Communication" -I"D:/blue_pill_pj/Station/Core/Inc/Device" -I"D:/blue_pill_pj/Station/Core/Inc/Protocol" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Communication

clean-Core-2f-Src-2f-Communication:
	-$(RM) ./Core/Src/Communication/Serial_CFG.d ./Core/Src/Communication/Serial_CFG.o ./Core/Src/Communication/Serial_CFG.su ./Core/Src/Communication/Serial_log.d ./Core/Src/Communication/Serial_log.o ./Core/Src/Communication/Serial_log.su ./Core/Src/Communication/ServerMessage.d ./Core/Src/Communication/ServerMessage.o ./Core/Src/Communication/ServerMessage.su ./Core/Src/Communication/stationCMD.d ./Core/Src/Communication/stationCMD.o ./Core/Src/Communication/stationCMD.su

.PHONY: clean-Core-2f-Src-2f-Communication


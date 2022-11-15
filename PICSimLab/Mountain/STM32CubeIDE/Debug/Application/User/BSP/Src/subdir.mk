################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/BSP/Src/BSP_BMP280.c \
../Application/User/BSP/Src/BSP_BMP280_Ex.c \
../Application/User/BSP/Src/BSP_I2C.c \
../Application/User/BSP/Src/BSP_LED.c 

OBJS += \
./Application/User/BSP/Src/BSP_BMP280.o \
./Application/User/BSP/Src/BSP_BMP280_Ex.o \
./Application/User/BSP/Src/BSP_I2C.o \
./Application/User/BSP/Src/BSP_LED.o 

C_DEPS += \
./Application/User/BSP/Src/BSP_BMP280.d \
./Application/User/BSP/Src/BSP_BMP280_Ex.d \
./Application/User/BSP/Src/BSP_I2C.d \
./Application/User/BSP/Src/BSP_LED.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/BSP/Src/%.o: ../Application/User/BSP/Src/%.c Application/User/BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Core/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -I"/home/may/STM32CubeIDE/my_try/Mountain/STM32CubeIDE/Application/User/BSP/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


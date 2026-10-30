################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DEV_Config.c \
../Core/Src/GUI_Paint.c \
../Core/Src/LCD_1inch28.c \
../Core/Src/W25Q128.c \
../Core/Src/font20.c \
../Core/Src/font24.c \
../Core/Src/main.c \
../Core/Src/phase.c \
../Core/Src/programming.c \
../Core/Src/screen_process.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/temperature.c 

OBJS += \
./Core/Src/DEV_Config.o \
./Core/Src/GUI_Paint.o \
./Core/Src/LCD_1inch28.o \
./Core/Src/W25Q128.o \
./Core/Src/font20.o \
./Core/Src/font24.o \
./Core/Src/main.o \
./Core/Src/phase.o \
./Core/Src/programming.o \
./Core/Src/screen_process.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/temperature.o 

C_DEPS += \
./Core/Src/DEV_Config.d \
./Core/Src/GUI_Paint.d \
./Core/Src/LCD_1inch28.d \
./Core/Src/W25Q128.d \
./Core/Src/font20.d \
./Core/Src/font24.d \
./Core/Src/main.d \
./Core/Src/phase.d \
./Core/Src/programming.d \
./Core/Src/screen_process.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/temperature.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DEV_Config.cyclo ./Core/Src/DEV_Config.d ./Core/Src/DEV_Config.o ./Core/Src/DEV_Config.su ./Core/Src/GUI_Paint.cyclo ./Core/Src/GUI_Paint.d ./Core/Src/GUI_Paint.o ./Core/Src/GUI_Paint.su ./Core/Src/LCD_1inch28.cyclo ./Core/Src/LCD_1inch28.d ./Core/Src/LCD_1inch28.o ./Core/Src/LCD_1inch28.su ./Core/Src/W25Q128.cyclo ./Core/Src/W25Q128.d ./Core/Src/W25Q128.o ./Core/Src/W25Q128.su ./Core/Src/font20.cyclo ./Core/Src/font20.d ./Core/Src/font20.o ./Core/Src/font20.su ./Core/Src/font24.cyclo ./Core/Src/font24.d ./Core/Src/font24.o ./Core/Src/font24.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/phase.cyclo ./Core/Src/phase.d ./Core/Src/phase.o ./Core/Src/phase.su ./Core/Src/programming.cyclo ./Core/Src/programming.d ./Core/Src/programming.o ./Core/Src/programming.su ./Core/Src/screen_process.cyclo ./Core/Src/screen_process.d ./Core/Src/screen_process.o ./Core/Src/screen_process.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/temperature.cyclo ./Core/Src/temperature.d ./Core/Src/temperature.o ./Core/Src/temperature.su

.PHONY: clean-Core-2f-Src


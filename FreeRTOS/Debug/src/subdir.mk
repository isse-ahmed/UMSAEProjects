################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/croutine.c \
../src/event_groups.c \
../src/heap_4.c \
../src/led.c \
../src/list.c \
../src/main.c \
../src/port.c \
../src/queue.c \
../src/stream_buffer.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/tasks.c \
../src/timers.c 

OBJS += \
./src/croutine.o \
./src/event_groups.o \
./src/heap_4.o \
./src/led.o \
./src/list.o \
./src/main.o \
./src/port.o \
./src/queue.o \
./src/stream_buffer.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/tasks.o \
./src/timers.o 

C_DEPS += \
./src/croutine.d \
./src/event_groups.d \
./src/heap_4.d \
./src/led.d \
./src/list.d \
./src/main.d \
./src/port.d \
./src/queue.d \
./src/stream_buffer.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/tasks.d \
./src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -I"D:/Documents/Notes/embed/UMSAEST32Tutorial/FreeRTOS/StdPeriph_Driver/inc" -I"D:/Documents/Notes/embed/UMSAEST32Tutorial/FreeRTOS/inc" -I"D:/Documents/Notes/embed/UMSAEST32Tutorial/FreeRTOS/CMSIS/device" -I"D:/Documents/Notes/embed/UMSAEST32Tutorial/FreeRTOS/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



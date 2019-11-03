################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.c" \

C_SRCS += \
../SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.c \

OBJS_OS_FORMAT += \
./SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.o \

C_DEPS_QUOTED += \
"./SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.d" \

OBJS += \
./SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.o \

OBJS_QUOTED += \
"./SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.o" \

C_DEPS += \
./SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.o: ../SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.c
	@echo 'Building file: $<'
	@echo 'Executing target #71 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/heap_2.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



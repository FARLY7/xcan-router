################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.c" \
"../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.c" \

S_SRCS_QUOTED += \
"../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.s" \

S_SRCS += \
../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.s \

C_SRCS += \
../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.c \
../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.c \

OBJS_OS_FORMAT += \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.o \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.o \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.o \

C_DEPS_QUOTED += \
"./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.d" \
"./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.d" \

OBJS += \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.o \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.o \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.o \

OBJS_QUOTED += \
"./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.o" \
"./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.o" \
"./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.o" \

C_DEPS += \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.d \
./SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.o: ../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.c
	@echo 'Building file: $<'
	@echo 'Executing target #68 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/port.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.o: ../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.s
	@echo 'Building file: $<'
	@echo 'Executing target #69 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.args" -c -o "SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/portasm.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.o: ../SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.c
	@echo 'Building file: $<'
	@echo 'Executing target #70 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/porttimer.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/rtos/FreeRTOS_C55/Source/croutine.c" \
"../SDK/rtos/FreeRTOS_C55/Source/event_groups.c" \
"../SDK/rtos/FreeRTOS_C55/Source/list.c" \
"../SDK/rtos/FreeRTOS_C55/Source/queue.c" \
"../SDK/rtos/FreeRTOS_C55/Source/tasks.c" \
"../SDK/rtos/FreeRTOS_C55/Source/timers.c" \

C_SRCS += \
../SDK/rtos/FreeRTOS_C55/Source/croutine.c \
../SDK/rtos/FreeRTOS_C55/Source/event_groups.c \
../SDK/rtos/FreeRTOS_C55/Source/list.c \
../SDK/rtos/FreeRTOS_C55/Source/queue.c \
../SDK/rtos/FreeRTOS_C55/Source/tasks.c \
../SDK/rtos/FreeRTOS_C55/Source/timers.c \

OBJS_OS_FORMAT += \
./SDK/rtos/FreeRTOS_C55/Source/croutine.o \
./SDK/rtos/FreeRTOS_C55/Source/event_groups.o \
./SDK/rtos/FreeRTOS_C55/Source/list.o \
./SDK/rtos/FreeRTOS_C55/Source/queue.o \
./SDK/rtos/FreeRTOS_C55/Source/tasks.o \
./SDK/rtos/FreeRTOS_C55/Source/timers.o \

C_DEPS_QUOTED += \
"./SDK/rtos/FreeRTOS_C55/Source/croutine.d" \
"./SDK/rtos/FreeRTOS_C55/Source/event_groups.d" \
"./SDK/rtos/FreeRTOS_C55/Source/list.d" \
"./SDK/rtos/FreeRTOS_C55/Source/queue.d" \
"./SDK/rtos/FreeRTOS_C55/Source/tasks.d" \
"./SDK/rtos/FreeRTOS_C55/Source/timers.d" \

OBJS += \
./SDK/rtos/FreeRTOS_C55/Source/croutine.o \
./SDK/rtos/FreeRTOS_C55/Source/event_groups.o \
./SDK/rtos/FreeRTOS_C55/Source/list.o \
./SDK/rtos/FreeRTOS_C55/Source/queue.o \
./SDK/rtos/FreeRTOS_C55/Source/tasks.o \
./SDK/rtos/FreeRTOS_C55/Source/timers.o \

OBJS_QUOTED += \
"./SDK/rtos/FreeRTOS_C55/Source/croutine.o" \
"./SDK/rtos/FreeRTOS_C55/Source/event_groups.o" \
"./SDK/rtos/FreeRTOS_C55/Source/list.o" \
"./SDK/rtos/FreeRTOS_C55/Source/queue.o" \
"./SDK/rtos/FreeRTOS_C55/Source/tasks.o" \
"./SDK/rtos/FreeRTOS_C55/Source/timers.o" \

C_DEPS += \
./SDK/rtos/FreeRTOS_C55/Source/croutine.d \
./SDK/rtos/FreeRTOS_C55/Source/event_groups.d \
./SDK/rtos/FreeRTOS_C55/Source/list.d \
./SDK/rtos/FreeRTOS_C55/Source/queue.d \
./SDK/rtos/FreeRTOS_C55/Source/tasks.d \
./SDK/rtos/FreeRTOS_C55/Source/timers.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/FreeRTOS_C55/Source/croutine.o: ../SDK/rtos/FreeRTOS_C55/Source/croutine.c
	@echo 'Building file: $<'
	@echo 'Executing target #62 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/croutine.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/croutine.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/rtos/FreeRTOS_C55/Source/event_groups.o: ../SDK/rtos/FreeRTOS_C55/Source/event_groups.c
	@echo 'Building file: $<'
	@echo 'Executing target #63 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/event_groups.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/event_groups.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/rtos/FreeRTOS_C55/Source/list.o: ../SDK/rtos/FreeRTOS_C55/Source/list.c
	@echo 'Building file: $<'
	@echo 'Executing target #64 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/list.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/list.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/rtos/FreeRTOS_C55/Source/queue.o: ../SDK/rtos/FreeRTOS_C55/Source/queue.c
	@echo 'Building file: $<'
	@echo 'Executing target #65 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/queue.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/queue.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/rtos/FreeRTOS_C55/Source/tasks.o: ../SDK/rtos/FreeRTOS_C55/Source/tasks.c
	@echo 'Building file: $<'
	@echo 'Executing target #66 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/tasks.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/tasks.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/rtos/FreeRTOS_C55/Source/timers.o: ../SDK/rtos/FreeRTOS_C55/Source/timers.c
	@echo 'Building file: $<'
	@echo 'Executing target #67 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/FreeRTOS_C55/Source/timers.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/FreeRTOS_C55/Source/timers.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



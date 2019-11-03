################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/rtos/osif/osif_freertos.c" \

C_SRCS += \
../SDK/rtos/osif/osif_freertos.c \

OBJS_OS_FORMAT += \
./SDK/rtos/osif/osif_freertos.o \

C_DEPS_QUOTED += \
"./SDK/rtos/osif/osif_freertos.d" \

OBJS += \
./SDK/rtos/osif/osif_freertos.o \

OBJS_QUOTED += \
"./SDK/rtos/osif/osif_freertos.o" \

C_DEPS += \
./SDK/rtos/osif/osif_freertos.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/osif/osif_freertos.o: ../SDK/rtos/osif/osif_freertos.c
	@echo 'Building file: $<'
	@echo 'Executing target #72 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/osif/osif_freertos.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/osif/osif_freertos.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



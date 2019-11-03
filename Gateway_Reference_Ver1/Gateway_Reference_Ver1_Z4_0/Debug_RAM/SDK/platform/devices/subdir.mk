################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/platform/devices/startup.c" \

C_SRCS += \
../SDK/platform/devices/startup.c \

OBJS_OS_FORMAT += \
./SDK/platform/devices/startup.o \

C_DEPS_QUOTED += \
"./SDK/platform/devices/startup.d" \

OBJS += \
./SDK/platform/devices/startup.o \

OBJS_QUOTED += \
"./SDK/platform/devices/startup.o" \

C_DEPS += \
./SDK/platform/devices/startup.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/devices/startup.o: ../SDK/platform/devices/startup.c
	@echo 'Building file: $<'
	@echo 'Executing target #35 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/devices/startup.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/devices/startup.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



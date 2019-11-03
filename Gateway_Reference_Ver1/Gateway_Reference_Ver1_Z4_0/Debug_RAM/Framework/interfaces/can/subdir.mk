################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/interfaces/can/can.c" \

C_SRCS += \
../Framework/interfaces/can/can.c \

OBJS_OS_FORMAT += \
./Framework/interfaces/can/can.o \

C_DEPS_QUOTED += \
"./Framework/interfaces/can/can.d" \

OBJS += \
./Framework/interfaces/can/can.o \

OBJS_QUOTED += \
"./Framework/interfaces/can/can.o" \

C_DEPS += \
./Framework/interfaces/can/can.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/interfaces/can/can.o: ../Framework/interfaces/can/can.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/interfaces/can/can.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/interfaces/can/can.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



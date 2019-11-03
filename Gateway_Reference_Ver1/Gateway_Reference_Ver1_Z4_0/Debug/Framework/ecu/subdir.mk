################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/ecu/ecu.c" \

C_SRCS += \
../Framework/ecu/ecu.c \

OBJS_OS_FORMAT += \
./Framework/ecu/ecu.o \

C_DEPS_QUOTED += \
"./Framework/ecu/ecu.d" \

OBJS += \
./Framework/ecu/ecu.o \

OBJS_QUOTED += \
"./Framework/ecu/ecu.o" \

C_DEPS += \
./Framework/ecu/ecu.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/ecu/ecu.o: ../Framework/ecu/ecu.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/ecu/ecu.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/ecu/ecu.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



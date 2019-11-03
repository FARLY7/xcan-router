################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/gateway/gateway.c" \

C_SRCS += \
../Framework/gateway/gateway.c \

OBJS_OS_FORMAT += \
./Framework/gateway/gateway.o \

C_DEPS_QUOTED += \
"./Framework/gateway/gateway.d" \

OBJS += \
./Framework/gateway/gateway.o \

OBJS_QUOTED += \
"./Framework/gateway/gateway.o" \

C_DEPS += \
./Framework/gateway/gateway.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/gateway/gateway.o: ../Framework/gateway/gateway.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/gateway/gateway.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/gateway/gateway.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



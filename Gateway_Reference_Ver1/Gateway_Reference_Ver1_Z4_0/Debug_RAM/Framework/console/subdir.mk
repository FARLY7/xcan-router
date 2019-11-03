################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/console/console.c" \

C_SRCS += \
../Framework/console/console.c \

OBJS_OS_FORMAT += \
./Framework/console/console.o \

C_DEPS_QUOTED += \
"./Framework/console/console.d" \

OBJS += \
./Framework/console/console.o \

OBJS_QUOTED += \
"./Framework/console/console.o" \

C_DEPS += \
./Framework/console/console.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/console/console.o: ../Framework/console/console.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/console/console.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/console/console.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



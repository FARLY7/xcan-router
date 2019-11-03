################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/interfaces/ethernet/ethernet.c" \

C_SRCS += \
../Framework/interfaces/ethernet/ethernet.c \

OBJS_OS_FORMAT += \
./Framework/interfaces/ethernet/ethernet.o \

C_DEPS_QUOTED += \
"./Framework/interfaces/ethernet/ethernet.d" \

OBJS += \
./Framework/interfaces/ethernet/ethernet.o \

OBJS_QUOTED += \
"./Framework/interfaces/ethernet/ethernet.o" \

C_DEPS += \
./Framework/interfaces/ethernet/ethernet.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/interfaces/ethernet/ethernet.o: ../Framework/interfaces/ethernet/ethernet.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/interfaces/ethernet/ethernet.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/interfaces/ethernet/ethernet.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



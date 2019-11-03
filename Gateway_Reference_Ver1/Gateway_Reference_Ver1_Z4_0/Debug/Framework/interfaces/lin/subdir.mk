################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/interfaces/lin/lin.c" \

C_SRCS += \
../Framework/interfaces/lin/lin.c \

OBJS_OS_FORMAT += \
./Framework/interfaces/lin/lin.o \

C_DEPS_QUOTED += \
"./Framework/interfaces/lin/lin.d" \

OBJS += \
./Framework/interfaces/lin/lin.o \

OBJS_QUOTED += \
"./Framework/interfaces/lin/lin.o" \

C_DEPS += \
./Framework/interfaces/lin/lin.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/interfaces/lin/lin.o: ../Framework/interfaces/lin/lin.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/interfaces/lin/lin.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/interfaces/lin/lin.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



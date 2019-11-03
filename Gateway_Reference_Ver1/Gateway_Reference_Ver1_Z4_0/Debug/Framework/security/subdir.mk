################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/security/hsm_she.c" \

C_SRCS += \
../Framework/security/hsm_she.c \

OBJS_OS_FORMAT += \
./Framework/security/hsm_she.o \

C_DEPS_QUOTED += \
"./Framework/security/hsm_she.d" \

OBJS += \
./Framework/security/hsm_she.o \

OBJS_QUOTED += \
"./Framework/security/hsm_she.o" \

C_DEPS += \
./Framework/security/hsm_she.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/security/hsm_she.o: ../Framework/security/hsm_she.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/security/hsm_she.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/security/hsm_she.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



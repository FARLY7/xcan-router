################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/interfaces/sdhc/sdhc.c" \

C_SRCS += \
../Framework/interfaces/sdhc/sdhc.c \

OBJS_OS_FORMAT += \
./Framework/interfaces/sdhc/sdhc.o \

C_DEPS_QUOTED += \
"./Framework/interfaces/sdhc/sdhc.d" \

OBJS += \
./Framework/interfaces/sdhc/sdhc.o \

OBJS_QUOTED += \
"./Framework/interfaces/sdhc/sdhc.o" \

C_DEPS += \
./Framework/interfaces/sdhc/sdhc.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/interfaces/sdhc/sdhc.o: ../Framework/interfaces/sdhc/sdhc.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/interfaces/sdhc/sdhc.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/interfaces/sdhc/sdhc.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



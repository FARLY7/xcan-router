################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/platform/drivers/src/bctu/bctu_driver.c" \

C_SRCS += \
../SDK/platform/drivers/src/bctu/bctu_driver.c \

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/bctu/bctu_driver.o \

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/bctu/bctu_driver.d" \

OBJS += \
./SDK/platform/drivers/src/bctu/bctu_driver.o \

OBJS_QUOTED += \
"./SDK/platform/drivers/src/bctu/bctu_driver.o" \

C_DEPS += \
./SDK/platform/drivers/src/bctu/bctu_driver.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/bctu/bctu_driver.o: ../SDK/platform/drivers/src/bctu/bctu_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/bctu/bctu_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/bctu/bctu_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



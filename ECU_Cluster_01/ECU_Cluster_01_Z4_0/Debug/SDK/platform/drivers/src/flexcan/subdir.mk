################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/platform/drivers/src/flexcan/flexcan_driver.c" \
"../SDK/platform/drivers/src/flexcan/flexcan_hw_access.c" \
"../SDK/platform/drivers/src/flexcan/flexcan_irq.c" \

C_SRCS += \
../SDK/platform/drivers/src/flexcan/flexcan_driver.c \
../SDK/platform/drivers/src/flexcan/flexcan_hw_access.c \
../SDK/platform/drivers/src/flexcan/flexcan_irq.c \

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/flexcan/flexcan_driver.o \
./SDK/platform/drivers/src/flexcan/flexcan_hw_access.o \
./SDK/platform/drivers/src/flexcan/flexcan_irq.o \

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/flexcan/flexcan_driver.d" \
"./SDK/platform/drivers/src/flexcan/flexcan_hw_access.d" \
"./SDK/platform/drivers/src/flexcan/flexcan_irq.d" \

OBJS += \
./SDK/platform/drivers/src/flexcan/flexcan_driver.o \
./SDK/platform/drivers/src/flexcan/flexcan_hw_access.o \
./SDK/platform/drivers/src/flexcan/flexcan_irq.o \

OBJS_QUOTED += \
"./SDK/platform/drivers/src/flexcan/flexcan_driver.o" \
"./SDK/platform/drivers/src/flexcan/flexcan_hw_access.o" \
"./SDK/platform/drivers/src/flexcan/flexcan_irq.o" \

C_DEPS += \
./SDK/platform/drivers/src/flexcan/flexcan_driver.d \
./SDK/platform/drivers/src/flexcan/flexcan_hw_access.d \
./SDK/platform/drivers/src/flexcan/flexcan_irq.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/flexcan/flexcan_driver.o: ../SDK/platform/drivers/src/flexcan/flexcan_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/flexcan/flexcan_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/flexcan/flexcan_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/flexcan/flexcan_hw_access.o: ../SDK/platform/drivers/src/flexcan/flexcan_hw_access.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/flexcan/flexcan_hw_access.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/flexcan/flexcan_hw_access.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/flexcan/flexcan_irq.o: ../SDK/platform/drivers/src/flexcan/flexcan_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/flexcan/flexcan_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/flexcan/flexcan_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



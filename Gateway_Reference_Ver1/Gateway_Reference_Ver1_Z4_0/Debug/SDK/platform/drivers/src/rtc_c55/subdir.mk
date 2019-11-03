################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.c" \
"../SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.c" \
"../SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.c" \

C_SRCS += \
../SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.c \
../SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.c \
../SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.c \

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o \

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.d" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.d" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.d" \

OBJS += \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o \

OBJS_QUOTED += \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o" \

C_DEPS += \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.d \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.d \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o: ../SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #53 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o: ../SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.c
	@echo 'Building file: $<'
	@echo 'Executing target #54 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o: ../SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #55 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



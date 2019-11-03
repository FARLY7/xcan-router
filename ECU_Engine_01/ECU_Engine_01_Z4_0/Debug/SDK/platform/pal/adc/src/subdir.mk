################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/platform/pal/adc/src/adc_irq.c" \
"../SDK/platform/pal/adc/src/adc_pal.c" \

C_SRCS += \
../SDK/platform/pal/adc/src/adc_irq.c \
../SDK/platform/pal/adc/src/adc_pal.c \

OBJS_OS_FORMAT += \
./SDK/platform/pal/adc/src/adc_irq.o \
./SDK/platform/pal/adc/src/adc_pal.o \

C_DEPS_QUOTED += \
"./SDK/platform/pal/adc/src/adc_irq.d" \
"./SDK/platform/pal/adc/src/adc_pal.d" \

OBJS += \
./SDK/platform/pal/adc/src/adc_irq.o \
./SDK/platform/pal/adc/src/adc_pal.o \

OBJS_QUOTED += \
"./SDK/platform/pal/adc/src/adc_irq.o" \
"./SDK/platform/pal/adc/src/adc_pal.o" \

C_DEPS += \
./SDK/platform/pal/adc/src/adc_irq.d \
./SDK/platform/pal/adc/src/adc_pal.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/pal/adc/src/adc_irq.o: ../SDK/platform/pal/adc/src/adc_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/pal/adc/src/adc_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/pal/adc/src/adc_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/pal/adc/src/adc_pal.o: ../SDK/platform/pal/adc/src/adc_pal.c
	@echo 'Building file: $<'
	@echo 'Executing target #26 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/pal/adc/src/adc_pal.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/pal/adc/src/adc_pal.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



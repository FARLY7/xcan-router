################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/adc/src/adc_irq.c" \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/adc/src/adc_pal.c" \

C_SRCS += \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/adc/src/adc_irq.c \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/adc/src/adc_pal.c \

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
SDK/platform/pal/adc/src/adc_irq.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/adc/src/adc_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/pal/adc/src/adc_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/pal/adc/src/adc_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/pal/adc/src/adc_pal.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/adc/src/adc_pal.c
	@echo 'Building file: $<'
	@echo 'Executing target #26 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/pal/adc/src/adc_pal.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/pal/adc/src/adc_pal.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



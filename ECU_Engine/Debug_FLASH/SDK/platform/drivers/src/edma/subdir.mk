################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_driver.c" \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_hw_access.c" \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_irq.c" \

C_SRCS += \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_driver.c \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_hw_access.c \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_irq.c \

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/edma/edma_driver.o \
./SDK/platform/drivers/src/edma/edma_hw_access.o \
./SDK/platform/drivers/src/edma/edma_irq.o \

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/edma/edma_driver.d" \
"./SDK/platform/drivers/src/edma/edma_hw_access.d" \
"./SDK/platform/drivers/src/edma/edma_irq.d" \

OBJS += \
./SDK/platform/drivers/src/edma/edma_driver.o \
./SDK/platform/drivers/src/edma/edma_hw_access.o \
./SDK/platform/drivers/src/edma/edma_irq.o \

OBJS_QUOTED += \
"./SDK/platform/drivers/src/edma/edma_driver.o" \
"./SDK/platform/drivers/src/edma/edma_hw_access.o" \
"./SDK/platform/drivers/src/edma/edma_irq.o" \

C_DEPS += \
./SDK/platform/drivers/src/edma/edma_driver.d \
./SDK/platform/drivers/src/edma/edma_hw_access.d \
./SDK/platform/drivers/src/edma/edma_irq.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/edma/edma_driver.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/edma/edma_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/edma/edma_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/edma/edma_hw_access.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_hw_access.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/edma/edma_hw_access.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/edma/edma_hw_access.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/edma/edma_irq.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/drivers/src/edma/edma_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/edma/edma_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/edma/edma_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



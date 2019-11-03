################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/can/src/can_pal.c" \

C_SRCS += \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/can/src/can_pal.c \

OBJS_OS_FORMAT += \
./SDK/platform/pal/can/src/can_pal.o \

C_DEPS_QUOTED += \
"./SDK/platform/pal/can/src/can_pal.d" \

OBJS += \
./SDK/platform/pal/can/src/can_pal.o \

OBJS_QUOTED += \
"./SDK/platform/pal/can/src/can_pal.o" \

C_DEPS += \
./SDK/platform/pal/can/src/can_pal.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/pal/can/src/can_pal.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/pal/can/src/can_pal.c
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/pal/can/src/can_pal.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/pal/can/src/can_pal.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



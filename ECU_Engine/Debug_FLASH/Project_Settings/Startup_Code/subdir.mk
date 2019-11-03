################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS_QUOTED += \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/startup_MPC5748G.S" \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/core0_intc_sw_handlers.S" \
"C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/interrupt_vectors.S" \

S_UPPER_SRCS += \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/startup_MPC5748G.S \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/core0_intc_sw_handlers.S \
C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/interrupt_vectors.S \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/startup_MPC5748G.o \
./Project_Settings/Startup_Code/core0_intc_sw_handlers.o \
./Project_Settings/Startup_Code/interrupt_vectors.o \

OBJS += \
./Project_Settings/Startup_Code/startup_MPC5748G.o \
./Project_Settings/Startup_Code/core0_intc_sw_handlers.o \
./Project_Settings/Startup_Code/interrupt_vectors.o \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/startup_MPC5748G.o" \
"./Project_Settings/Startup_Code/core0_intc_sw_handlers.o" \
"./Project_Settings/Startup_Code/interrupt_vectors.o" \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/startup_MPC5748G.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/startup_MPC5748G.S
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@Project_Settings/Startup_Code/Startup_MPC5748G.args" -c -o "Project_Settings/Startup_Code/startup_MPC5748G.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/core0_intc_sw_handlers.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/core0_intc_sw_handlers.S
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@Project_Settings/Startup_Code/core0_intc_sw_handlers.args" -c -o "Project_Settings/Startup_Code/core0_intc_sw_handlers.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/interrupt_vectors.o: C:/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_EAR_0.8.2/platform/devices/MPC5748G/startup/gcc/interrupt_vectors.S
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@Project_Settings/Startup_Code/interrupt_vectors.args" -c -o "Project_Settings/Startup_Code/interrupt_vectors.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



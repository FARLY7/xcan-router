################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/core0_intc_sw_handlers.S" \
"../Project_Settings/Startup_Code/interrupt_vectors.S" \
"../Project_Settings/Startup_Code/startup_MPC5748G.S" \

S_UPPER_SRCS += \
../Project_Settings/Startup_Code/core0_intc_sw_handlers.S \
../Project_Settings/Startup_Code/interrupt_vectors.S \
../Project_Settings/Startup_Code/startup_MPC5748G.S \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/core0_intc_sw_handlers.o \
./Project_Settings/Startup_Code/interrupt_vectors.o \
./Project_Settings/Startup_Code/startup_MPC5748G.o \

OBJS += \
./Project_Settings/Startup_Code/core0_intc_sw_handlers.o \
./Project_Settings/Startup_Code/interrupt_vectors.o \
./Project_Settings/Startup_Code/startup_MPC5748G.o \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/core0_intc_sw_handlers.o" \
"./Project_Settings/Startup_Code/interrupt_vectors.o" \
"./Project_Settings/Startup_Code/startup_MPC5748G.o" \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/core0_intc_sw_handlers.o: ../Project_Settings/Startup_Code/core0_intc_sw_handlers.S
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@Project_Settings/Startup_Code/core0_intc_sw_handlers.args" -c -o "Project_Settings/Startup_Code/core0_intc_sw_handlers.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/interrupt_vectors.o: ../Project_Settings/Startup_Code/interrupt_vectors.S
	@echo 'Building file: $<'
	@echo 'Executing target #23 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@Project_Settings/Startup_Code/interrupt_vectors.args" -c -o "Project_Settings/Startup_Code/interrupt_vectors.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/startup_MPC5748G.o: ../Project_Settings/Startup_Code/startup_MPC5748G.S
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@Project_Settings/Startup_Code/startup_MPC5748G.args" -c -o "Project_Settings/Startup_Code/startup_MPC5748G.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



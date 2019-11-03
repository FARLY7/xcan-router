################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/dbg_console/dbg_console.c" \

C_SRCS += \
../Framework/dbg_console/dbg_console.c \

OBJS_OS_FORMAT += \
./Framework/dbg_console/dbg_console.o \

C_DEPS_QUOTED += \
"./Framework/dbg_console/dbg_console.d" \

OBJS += \
./Framework/dbg_console/dbg_console.o \

OBJS_QUOTED += \
"./Framework/dbg_console/dbg_console.o" \

C_DEPS += \
./Framework/dbg_console/dbg_console.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/dbg_console/dbg_console.o: ../Framework/dbg_console/dbg_console.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/dbg_console/dbg_console.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/dbg_console/dbg_console.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



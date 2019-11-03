################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/can.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/can.c \
../Sources/main.c \

OBJS_OS_FORMAT += \
./Sources/can.o \
./Sources/main.o \

C_DEPS_QUOTED += \
"./Sources/can.d" \
"./Sources/main.d" \

OBJS += \
./Sources/can.o \
./Sources/main.o \

OBJS_QUOTED += \
"./Sources/can.o" \
"./Sources/main.o" \

C_DEPS += \
./Sources/can.d \
./Sources/main.d \


# Each subdirectory must supply rules for building sources it contributes
Sources/can.o: ../Sources/can.c
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/can.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/can.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



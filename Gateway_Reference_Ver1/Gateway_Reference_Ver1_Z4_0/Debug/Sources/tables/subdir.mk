################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/tables/can_filter.c" \
"../Sources/tables/ecu_table.c" \
"../Sources/tables/pdu_table.c" \

C_SRCS += \
../Sources/tables/can_filter.c \
../Sources/tables/ecu_table.c \
../Sources/tables/pdu_table.c \

OBJS_OS_FORMAT += \
./Sources/tables/can_filter.o \
./Sources/tables/ecu_table.o \
./Sources/tables/pdu_table.o \

C_DEPS_QUOTED += \
"./Sources/tables/can_filter.d" \
"./Sources/tables/ecu_table.d" \
"./Sources/tables/pdu_table.d" \

OBJS += \
./Sources/tables/can_filter.o \
./Sources/tables/ecu_table.o \
./Sources/tables/pdu_table.o \

OBJS_QUOTED += \
"./Sources/tables/can_filter.o" \
"./Sources/tables/ecu_table.o" \
"./Sources/tables/pdu_table.o" \

C_DEPS += \
./Sources/tables/can_filter.d \
./Sources/tables/ecu_table.d \
./Sources/tables/pdu_table.d \


# Each subdirectory must supply rules for building sources it contributes
Sources/tables/can_filter.o: ../Sources/tables/can_filter.c
	@echo 'Building file: $<'
	@echo 'Executing target #74 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/tables/can_filter.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/tables/can_filter.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/tables/ecu_table.o: ../Sources/tables/ecu_table.c
	@echo 'Building file: $<'
	@echo 'Executing target #75 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/tables/ecu_table.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/tables/ecu_table.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/tables/pdu_table.o: ../Sources/tables/pdu_table.c
	@echo 'Building file: $<'
	@echo 'Executing target #76 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/tables/pdu_table.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/tables/pdu_table.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



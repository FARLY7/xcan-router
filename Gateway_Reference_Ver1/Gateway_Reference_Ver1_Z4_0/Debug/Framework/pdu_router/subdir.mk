################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Framework/pdu_router/pdu_router.c" \

C_SRCS += \
../Framework/pdu_router/pdu_router.c \

OBJS_OS_FORMAT += \
./Framework/pdu_router/pdu_router.o \

C_DEPS_QUOTED += \
"./Framework/pdu_router/pdu_router.d" \

OBJS += \
./Framework/pdu_router/pdu_router.o \

OBJS_QUOTED += \
"./Framework/pdu_router/pdu_router.o" \

C_DEPS += \
./Framework/pdu_router/pdu_router.d \


# Each subdirectory must supply rules for building sources it contributes
Framework/pdu_router/pdu_router.o: ../Framework/pdu_router/pdu_router.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Framework/pdu_router/pdu_router.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Framework/pdu_router/pdu_router.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



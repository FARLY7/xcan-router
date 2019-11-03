################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Generated_Code/CAN0.c" \
"../Generated_Code/CAN1.c" \
"../Generated_Code/CAN2.c" \
"../Generated_Code/CAN3.c" \
"../Generated_Code/CONSOLE.c" \
"../Generated_Code/Cpu.c" \
"../Generated_Code/HSM.c" \
"../Generated_Code/clockMan1.c" \
"../Generated_Code/dmaController1.c" \
"../Generated_Code/pin_mux.c" \
"../Generated_Code/rtcTimer1.c" \
"../Generated_Code/usdhc1.c" \

C_SRCS += \
../Generated_Code/CAN0.c \
../Generated_Code/CAN1.c \
../Generated_Code/CAN2.c \
../Generated_Code/CAN3.c \
../Generated_Code/CONSOLE.c \
../Generated_Code/Cpu.c \
../Generated_Code/HSM.c \
../Generated_Code/clockMan1.c \
../Generated_Code/dmaController1.c \
../Generated_Code/pin_mux.c \
../Generated_Code/rtcTimer1.c \
../Generated_Code/usdhc1.c \

OBJS_OS_FORMAT += \
./Generated_Code/CAN0.o \
./Generated_Code/CAN1.o \
./Generated_Code/CAN2.o \
./Generated_Code/CAN3.o \
./Generated_Code/CONSOLE.o \
./Generated_Code/Cpu.o \
./Generated_Code/HSM.o \
./Generated_Code/clockMan1.o \
./Generated_Code/dmaController1.o \
./Generated_Code/pin_mux.o \
./Generated_Code/rtcTimer1.o \
./Generated_Code/usdhc1.o \

C_DEPS_QUOTED += \
"./Generated_Code/CAN0.d" \
"./Generated_Code/CAN1.d" \
"./Generated_Code/CAN2.d" \
"./Generated_Code/CAN3.d" \
"./Generated_Code/CONSOLE.d" \
"./Generated_Code/Cpu.d" \
"./Generated_Code/HSM.d" \
"./Generated_Code/clockMan1.d" \
"./Generated_Code/dmaController1.d" \
"./Generated_Code/pin_mux.d" \
"./Generated_Code/rtcTimer1.d" \
"./Generated_Code/usdhc1.d" \

OBJS += \
./Generated_Code/CAN0.o \
./Generated_Code/CAN1.o \
./Generated_Code/CAN2.o \
./Generated_Code/CAN3.o \
./Generated_Code/CONSOLE.o \
./Generated_Code/Cpu.o \
./Generated_Code/HSM.o \
./Generated_Code/clockMan1.o \
./Generated_Code/dmaController1.o \
./Generated_Code/pin_mux.o \
./Generated_Code/rtcTimer1.o \
./Generated_Code/usdhc1.o \

OBJS_QUOTED += \
"./Generated_Code/CAN0.o" \
"./Generated_Code/CAN1.o" \
"./Generated_Code/CAN2.o" \
"./Generated_Code/CAN3.o" \
"./Generated_Code/CONSOLE.o" \
"./Generated_Code/Cpu.o" \
"./Generated_Code/HSM.o" \
"./Generated_Code/clockMan1.o" \
"./Generated_Code/dmaController1.o" \
"./Generated_Code/pin_mux.o" \
"./Generated_Code/rtcTimer1.o" \
"./Generated_Code/usdhc1.o" \

C_DEPS += \
./Generated_Code/CAN0.d \
./Generated_Code/CAN1.d \
./Generated_Code/CAN2.d \
./Generated_Code/CAN3.d \
./Generated_Code/CONSOLE.d \
./Generated_Code/Cpu.d \
./Generated_Code/HSM.d \
./Generated_Code/clockMan1.d \
./Generated_Code/dmaController1.d \
./Generated_Code/pin_mux.d \
./Generated_Code/rtcTimer1.d \
./Generated_Code/usdhc1.d \


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/CAN0.o: ../Generated_Code/CAN0.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/CAN0.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/CAN0.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/CAN1.o: ../Generated_Code/CAN1.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/CAN1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/CAN1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/CAN2.o: ../Generated_Code/CAN2.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/CAN2.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/CAN2.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/CAN3.o: ../Generated_Code/CAN3.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/CAN3.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/CAN3.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/CONSOLE.o: ../Generated_Code/CONSOLE.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/CONSOLE.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/CONSOLE.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Cpu.o: ../Generated_Code/Cpu.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/Cpu.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/Cpu.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/HSM.o: ../Generated_Code/HSM.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/HSM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/HSM.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/clockMan1.o: ../Generated_Code/clockMan1.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/clockMan1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/clockMan1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/dmaController1.o: ../Generated_Code/dmaController1.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/dmaController1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/dmaController1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/pin_mux.o: ../Generated_Code/pin_mux.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/pin_mux.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/pin_mux.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/rtcTimer1.o: ../Generated_Code/rtcTimer1.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/rtcTimer1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/rtcTimer1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/usdhc1.o: ../Generated_Code/usdhc1.c
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/usdhc1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/usdhc1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



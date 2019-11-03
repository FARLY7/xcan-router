################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/lwip.c" \
"../Sources/main.c" \
"../Sources/tcp_sockets.c" \

C_SRCS += \
../Sources/lwip.c \
../Sources/main.c \
../Sources/tcp_sockets.c \

OBJS_OS_FORMAT += \
./Sources/lwip.o \
./Sources/main.o \
./Sources/tcp_sockets.o \

C_DEPS_QUOTED += \
"./Sources/lwip.d" \
"./Sources/main.d" \
"./Sources/tcp_sockets.d" \

OBJS += \
./Sources/lwip.o \
./Sources/main.o \
./Sources/tcp_sockets.o \

OBJS_QUOTED += \
"./Sources/lwip.o" \
"./Sources/main.o" \
"./Sources/tcp_sockets.o" \

C_DEPS += \
./Sources/lwip.d \
./Sources/main.d \
./Sources/tcp_sockets.d \


# Each subdirectory must supply rules for building sources it contributes
Sources/lwip.o: ../Sources/lwip.c
	@echo 'Building file: $<'
	@echo 'Executing target #71 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/lwip.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/lwip.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #72 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/tcp_sockets.o: ../Sources/tcp_sockets.c
	@echo 'Building file: $<'
	@echo 'Executing target #73 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/tcp_sockets.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/tcp_sockets.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



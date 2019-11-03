################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/platform/drivers/src/linflexd/linflexd_uart_driver.c" \
"../SDK/platform/drivers/src/linflexd/linflexd_uart_irq.c" \

C_SRCS += \
../SDK/platform/drivers/src/linflexd/linflexd_uart_driver.c \
../SDK/platform/drivers/src/linflexd/linflexd_uart_irq.c \

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/linflexd/linflexd_uart_driver.o \
./SDK/platform/drivers/src/linflexd/linflexd_uart_irq.o \

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/linflexd/linflexd_uart_driver.d" \
"./SDK/platform/drivers/src/linflexd/linflexd_uart_irq.d" \

OBJS += \
./SDK/platform/drivers/src/linflexd/linflexd_uart_driver.o \
./SDK/platform/drivers/src/linflexd/linflexd_uart_irq.o \

OBJS_QUOTED += \
"./SDK/platform/drivers/src/linflexd/linflexd_uart_driver.o" \
"./SDK/platform/drivers/src/linflexd/linflexd_uart_irq.o" \

C_DEPS += \
./SDK/platform/drivers/src/linflexd/linflexd_uart_driver.d \
./SDK/platform/drivers/src/linflexd/linflexd_uart_irq.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/linflexd/linflexd_uart_driver.o: ../SDK/platform/drivers/src/linflexd/linflexd_uart_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #47 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/linflexd/linflexd_uart_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/linflexd/linflexd_uart_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/linflexd/linflexd_uart_irq.o: ../SDK/platform/drivers/src/linflexd/linflexd_uart_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #48 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/linflexd/linflexd_uart_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/linflexd/linflexd_uart_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



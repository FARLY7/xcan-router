################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/middleware/fatfs/source/ff.c" \
"../SDK/middleware/fatfs/source/ffunicode.c" \

C_SRCS += \
../SDK/middleware/fatfs/source/ff.c \
../SDK/middleware/fatfs/source/ffunicode.c \

OBJS_OS_FORMAT += \
./SDK/middleware/fatfs/source/ff.o \
./SDK/middleware/fatfs/source/ffunicode.o \

C_DEPS_QUOTED += \
"./SDK/middleware/fatfs/source/ff.d" \
"./SDK/middleware/fatfs/source/ffunicode.d" \

OBJS += \
./SDK/middleware/fatfs/source/ff.o \
./SDK/middleware/fatfs/source/ffunicode.o \

OBJS_QUOTED += \
"./SDK/middleware/fatfs/source/ff.o" \
"./SDK/middleware/fatfs/source/ffunicode.o" \

C_DEPS += \
./SDK/middleware/fatfs/source/ff.d \
./SDK/middleware/fatfs/source/ffunicode.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/fatfs/source/ff.o: ../SDK/middleware/fatfs/source/ff.c
	@echo 'Building file: $<'
	@echo 'Executing target #29 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/fatfs/source/ff.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/fatfs/source/ff.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/middleware/fatfs/source/ffunicode.o: ../SDK/middleware/fatfs/source/ffunicode.c
	@echo 'Building file: $<'
	@echo 'Executing target #30 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/fatfs/source/ffunicode.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/fatfs/source/ffunicode.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



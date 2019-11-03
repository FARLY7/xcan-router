################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SDK/middleware/sdhc/sd/sd.c" \
"../SDK/middleware/sdhc/sd/sdmmc.c" \
"../SDK/middleware/sdhc/sd/sdspi.c" \

C_SRCS += \
../SDK/middleware/sdhc/sd/sd.c \
../SDK/middleware/sdhc/sd/sdmmc.c \
../SDK/middleware/sdhc/sd/sdspi.c \

OBJS_OS_FORMAT += \
./SDK/middleware/sdhc/sd/sd.o \
./SDK/middleware/sdhc/sd/sdmmc.o \
./SDK/middleware/sdhc/sd/sdspi.o \

C_DEPS_QUOTED += \
"./SDK/middleware/sdhc/sd/sd.d" \
"./SDK/middleware/sdhc/sd/sdmmc.d" \
"./SDK/middleware/sdhc/sd/sdspi.d" \

OBJS += \
./SDK/middleware/sdhc/sd/sd.o \
./SDK/middleware/sdhc/sd/sdmmc.o \
./SDK/middleware/sdhc/sd/sdspi.o \

OBJS_QUOTED += \
"./SDK/middleware/sdhc/sd/sd.o" \
"./SDK/middleware/sdhc/sd/sdmmc.o" \
"./SDK/middleware/sdhc/sd/sdspi.o" \

C_DEPS += \
./SDK/middleware/sdhc/sd/sd.d \
./SDK/middleware/sdhc/sd/sdmmc.d \
./SDK/middleware/sdhc/sd/sdspi.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/sdhc/sd/sd.o: ../SDK/middleware/sdhc/sd/sd.c
	@echo 'Building file: $<'
	@echo 'Executing target #32 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/sdhc/sd/sd.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/sdhc/sd/sd.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/middleware/sdhc/sd/sdmmc.o: ../SDK/middleware/sdhc/sd/sdmmc.c
	@echo 'Building file: $<'
	@echo 'Executing target #33 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/sdhc/sd/sdmmc.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/sdhc/sd/sdmmc.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/middleware/sdhc/sd/sdspi.o: ../SDK/middleware/sdhc/sd/sdspi.c
	@echo 'Building file: $<'
	@echo 'Executing target #34 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/sdhc/sd/sdspi.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/sdhc/sd/sdspi.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



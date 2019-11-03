################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 76
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Framework/dbg_console/subdir.mk
-include Framework/ecu/subdir.mk
-include Framework/gateway/subdir.mk
-include Framework/interfaces/can/subdir.mk
-include Framework/interfaces/ethernet/subdir.mk
-include Framework/interfaces/lin/subdir.mk
-include Framework/interfaces/sdhc/subdir.mk
-include Framework/pdu_router/subdir.mk
-include Framework/security/subdir.mk
-include Generated_Code/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include SDK/middleware/fatfs/common/subdir.mk
-include SDK/middleware/fatfs/rtc/subdir.mk
-include SDK/middleware/fatfs/source/subdir.mk
-include SDK/middleware/sdhc/fatfs/subdir.mk
-include SDK/middleware/sdhc/sd/subdir.mk
-include SDK/middleware/sdhc/usdhc/subdir.mk
-include SDK/platform/devices/MPC5748G/startup/subdir.mk
-include SDK/platform/devices/subdir.mk
-include SDK/platform/drivers/src/clock/MPC57xx/subdir.mk
-include SDK/platform/drivers/src/clock/subdir.mk
-include SDK/platform/drivers/src/edma/subdir.mk
-include SDK/platform/drivers/src/flexcan/subdir.mk
-include SDK/platform/drivers/src/hsm/subdir.mk
-include SDK/platform/drivers/src/interrupt/subdir.mk
-include SDK/platform/drivers/src/linflexd/subdir.mk
-include SDK/platform/drivers/src/pins/subdir.mk
-include SDK/platform/drivers/src/rtc_c55/subdir.mk
-include SDK/platform/drivers/src/usdhc/subdir.mk
-include SDK/platform/pal/can/src/subdir.mk
-include SDK/platform/pal/security/src/subdir.mk
-include SDK/platform/pal/uart/src/subdir.mk
-include SDK/rtos/FreeRTOS_C55/Source/subdir.mk
-include SDK/rtos/FreeRTOS_C55/Source/portable/GCC/PowerPC_Z4/subdir.mk
-include SDK/rtos/FreeRTOS_C55/Source/portable/MemMang/subdir.mk
-include SDK/rtos/osif/subdir.mk
-include Sources/subdir.mk
-include Sources/tables/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Gateway_Reference_Ver1_Z4_0.elf \

EXECUTABLES_QUOTED += \
"Gateway_Reference_Ver1_Z4_0.elf" \

EXECUTABLES_OS_FORMAT += \
Gateway_Reference_Ver1_Z4_0.elf \

SECONDARY_SIZE += \
Gateway_Reference_Ver1_Z4_0.siz \

SECONDARY_SIZE_QUOTED += \
"Gateway_Reference_Ver1_Z4_0.siz" \

SECONDARY_SIZE_OS_FORMAT += \
Gateway_Reference_Ver1_Z4_0.siz \


# All Target
all: warning-messages explicit-dependencies Gateway_Reference_Ver1_Z4_0.elf secondary-outputs

# Tool invocations
Gateway_Reference_Ver1_Z4_0.elf: $(OBJS) C:/Users/nxa20140/workspaceS32DS.Power.2017.R1/Gateway_Reference_Ver1/Gateway_Reference_Ver1_Z4_0/Project_Settings/Linker_Files/MPC5748G_flash.ld $(OSPT) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #77 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	powerpc-eabivle-gcc -o "Gateway_Reference_Ver1_Z4_0.elf" "@Gateway_Reference_Ver1_Z4_0.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) $(OSPT)
	@echo 'Finished building target: $@'
	@echo ' '

Gateway_Reference_Ver1_Z4_0.siz: $(EXECUTABLES)
	@echo 'Executing target #78 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	powerpc-eabivle-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*/*.d ./*/*.d ./*/*/*/*.d ./*/*/*/*/*/*/*.d ./*/*/*/*/*/*/*/*.d ./*/*/*/*/*.d ./*/*/*/*/*/*.d  ./*/*/*.o ./*/*.o ./*/*/*/*.o ./*/*/*/*/*/*/*.o ./*/*/*/*/*/*/*/*.o ./*/*/*/*/*.o ./*/*/*/*/*/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 


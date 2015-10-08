################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
MotorStateMachine/MotorStateMachine.obj: ../MotorStateMachine/MotorStateMachine.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7R4 --code_state=32 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/Users/Charanpreet/Documents/GitHub/buddy/firmware/Buddy_Firmware_Hercules/MotorStateMachine" --include_path="C:/Users/Charanpreet/Documents/GitHub/buddy/firmware/Buddy_Firmware_Hercules/include" -g --display_error_number --diag_warning=225 --diag_wrap=off --enum_type=packed --preproc_with_compile --preproc_dependency="MotorStateMachine/MotorStateMachine.pp" --obj_directory="MotorStateMachine" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '



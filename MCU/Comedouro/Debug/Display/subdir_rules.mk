################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Display/Display.obj: ../Display/Display.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv8/ccs_base/msp430/include" --include_path="C:/Users/gabri/Documents/UTFPR - Engenharia de Computação/8º Semestre/Desenvolvimento de aplicativos para dispositivos móveis/ddm_comedouro/MCU/Comedouro" --include_path="C:/Users/gabri/Documents/UTFPR - Engenharia de Computação/8º Semestre/Desenvolvimento de aplicativos para dispositivos móveis/ddm_comedouro/MCU/Comedouro/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="Display/Display.d_raw" --obj_directory="Display" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Display/StateMachine.obj: ../Display/StateMachine.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv8/ccs_base/msp430/include" --include_path="C:/Users/gabri/Documents/UTFPR - Engenharia de Computação/8º Semestre/Desenvolvimento de aplicativos para dispositivos móveis/ddm_comedouro/MCU/Comedouro" --include_path="C:/Users/gabri/Documents/UTFPR - Engenharia de Computação/8º Semestre/Desenvolvimento de aplicativos para dispositivos móveis/ddm_comedouro/MCU/Comedouro/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="Display/StateMachine.d_raw" --obj_directory="Display" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



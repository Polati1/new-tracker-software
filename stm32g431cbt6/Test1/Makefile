#---Compiler Tools---

#cross compiler
CC = arm-none-eabi-gcc
#inspect and disassembly
OBJDUMP = arm-none-eabi-objdump
#inspect binary size
SIZE    = arm-none-eabi-size


#---MCU Type and Model---

#family of MCU aka G4, F1, L4, F4
MCU_FAMILY = STM32G4
#specific MCU model name
MCU_DEF = STM32G431xx


#---File Names---

#linker .ld file name
LINKER_NAME = linker
#.elf name
BINARY_NAME = blink
#.c name
PROGRAM_NAME = main
#startup code .c name
STARTUP_NAME = startup
#system init file name, depends on family
SYSTEM_FILE_NAME = system_stm32g4xx


#---CMSIS Paths---

#CMSIS .h and system file .c paths
CMSIS_CORE = ./vendor/CMSIS_5/CMSIS/Core/Include
CMSIS_ST = ./vendor/CMSIS_5/Device/ST/$(MCU_FAMILY)/Include
CMSIS_SRC = ./vendor/CMSIS_5/Device/ST/$(MCU_FAMILY)/Source/Templates

#---Build Flags---

#build flags
CFLAGS = -mcpu=cortex-m4 \
         -mthumb \
         -nostdlib \
		 -Os \
         -g3 \
         -Wall \
         -Wextra \
		 -fno-builtin \

#include flags
CPPFLAGS = -D$(MCU_DEF) -I$(CMSIS_CORE) -I$(CMSIS_ST)
LDFLAGS = -T $(LINKER_NAME).ld


#---Programmer Configuration---

#programmer name and flags
PROGRAMMER = openocd
PROGRAMMER_FLAGS = -f interface/stlink.cfg -f target/stm32g4x.cfg



#---Build Process---

#Process of creating binary file
all: $(BINARY_NAME).elf

$(BINARY_NAME).elf: $(PROGRAM_NAME).o $(STARTUP_NAME).o $(SYSTEM_FILE_NAME).o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $(BINARY_NAME).elf

$(PROGRAM_NAME).o: $(PROGRAM_NAME).c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(STARTUP_NAME).o: $(STARTUP_NAME).c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(SYSTEM_FILE_NAME).o: $(CMSIS_SRC)/$(SYSTEM_FILE_NAME).c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@



#---Utility Commands---

# Display make commands with make help
help:
	@echo "Available targets:"
	@echo "  all       - Build the hex file (default)"
	@echo "  flash     - Upload code to MCU"
	@echo "  ping      - Check programmer connection"
	@echo "  disasm    - Generate assembly listing (.s file)"
	@echo "  size      - Show program size"
	@echo "  clean     - Remove build artifacts"

# Generate disassembly from ELF
disasm: $(BINARY_NAME).elf
	$(OBJDUMP) -d -S -M force-thumb $< > $(BINARY_NAME).s

# Upload the built program to MCU
flash: $(BINARY_NAME).elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program $(BINARY_NAME).elf verify reset exit"

# Ping the MCU to check if its connected
ping:
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "init; shutdown"

#View size in bytes of the binary file
size: $(BINARY_NAME).elf
	$(SIZE) -B $<

#remove build artifacts
clean:
	rm -rf *.o *.elf *.s


#ensure naming files these names does not overwrite the makefile commands
.PHONY: clean flash all disasm ping size


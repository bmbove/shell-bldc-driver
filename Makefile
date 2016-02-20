PROJECT		   = main
RUN_FROM_FLASH = 1

TRGT = arm-none-eabi-

CC	 = $(TRGT)gcc
CP	 = $(TRGT)objcopy
AS	 = $(TRGT)as
BIN  = $(CP) -S -O binary
GDB  = $(TRGT)gdb
 
MCU  = cortex-m4
 
# List all default C defines here, like -D_DEBUG=1
DEFS = -DSTM32L476xx

# List all default ASM defines here, like -D_DEBUG=1
ADEFS = 
 
LIBSDIR    = ./libraries

COREINC = $(LIBSDIR)/CMSIS/core/include
DEVINC = $(LIBSDIR)/CMSIS/device/include
OTHERINC = ./

DEVICE		 = ./device
STARTUP		 = $(DEVICE)/startup
LINKER		 = $(DEVICE)/linker

SRC  := $(wildcard *.c) $(wildcard $(DEVICE)/*.c)

ASRC := $(wildcard *.s) $(wildcard $(STARTUP)/*.s)


# Define optimisation level here
OPT = -Os
 
LDSCRIPT = $(LINKER)/STM32L476RG_FLASH.ld
FULL_PRJ = $(PROJECT)_rom
 
INCLUDES = $(COREINC) $(DEVINC) $(OTHERINC)
CPPFLAGS += -g $(foreach includedir,$(INCLUDES),-I$(includedir))

OBJS	= $(ASRC:.s=.o) $(SRC:.c=.o)
MCFLAGS = -mcpu=$(MCU)
 
ASFLAGS = $(MCFLAGS) -g -mlittle-endian $(ADEFS)
CFLAGS = $(MCFLAGS) $(OPT) -g -mthumb -Wall -c -mlittle-endian $(DEFS)
LDFLAGS = $(MCFLAGS) -g -mthumb -mlittle-endian -T$(LDSCRIPT) -Wl,--gc-sections $(LIBDIR)
 
 
all: $(OBJS) $(FULL_PRJ).elf $(FULL_PRJ).bin

%o : %c
	$(CC) -c $(CPFLAGS) $< -o $@
 
%o : %s
	$(AS) -c $(ASFLAGS) $< -o $@
 
%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
 
%bin: %elf
	$(BIN) $< $@
clean:
	-rm -f $(OBJS)
	-rm -f $(FULL_PRJ).elf
	-rm -f $(FULL_PRJ).map
	-rm -f $(FULL_PRJ).bin
	-rm -f $(SRC:.c=.lst)
	-rm -f $(ASRC:.s=.lst)
	-rm -fR .dep

.PHONY: flash
flash: $(FULL_PRJ).bin $(FULL_PRJ).elf
	st-flash --reset erase
	st-flash --reset write $(FULL_PRJ).bin 0x08000000

.PHONY: debug
debug:
	$(GDB) $(FULL_PRJ).elf -ex "target extended-remote :4242"

.PHONY: asm
asm: $(FULL_PRJ).bin $(FULL_PROJ).elf
	$(TRGT)objdump -d $(FULL_PROJ).elf > ./debug/main_asm.s


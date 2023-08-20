CC=/opt/riscv32i/bin/riscv32-unknown-elf-gcc
AS=/opt/riscv32i/riscv32-unknown-elf/bin/as --traditional-format -march=rv32i -mabi=ilp32 
LD=/opt/riscv32i/riscv32-unknown-elf/bin/ld -m elf32lriscv
OBJ=/opt/riscv32i/riscv32-unknown-elf/bin/objcopy 
DUMP=/opt/riscv32i/riscv32-unknown-elf/bin/objdump

LINK=RISKXVII.ld
CFLAGS=	-O1 -march=rv32i -mabi=ilp32 
TARGET=	$(patsubst %.c,%, $(wildcard */*.c)) 

OBJS=start.o $(patsubst %,%.o, $(TARGET))


all: $(TARGET)

# generate all targets, then create memory image file and human readable listing file.
$(TARGET): start.o $(OBJS) $(LINK)
	$(LD) start.o $@.o -o $@ -T$(LINK)
	$(OBJ) -O binary $@ $@.mi
	$(DUMP) -S $@ > $@.lst
	

# compile C program source code into assembly code.
%.s:	%.c
	$(CC) -S $(CFLAGS) $<

# create a binary .o from assembly source
%.o:	%.s
	$(AS) -o $@ $<

# rule for cleaning up all generated files
clean:
	-rm -f $(TARGET).lst $(OBJS) $(patsubst %,%.mi, $(TARGET)) $(patsubst %,%.lst, $(TARGET))

ASM=nasm
CC=gcc
LD=ld

# boot sector is raw 16-bit code
ASMFLAGS_BOOT=-f bin

# stage 2 is linked with C, so we need ELF object files
ASMFLAGS_ELF=-f elf32
CFLAGS= -Iinclude -m32 -ffreestanding -c -fno-pie -fno-stack-protector
LDFLAGS=-m elf_i386 -Ttext 0x7e00 --oformat binary

SRC_DIR=src
BUILD_DIR=build

BOOT_SRC=$(SRC_DIR)/boot.asm
STAGE2_SRC=$(SRC_DIR)/stage2.asm
KERNEL_SRC=$(shell find $(SRC_DIR) -name "*.c")
#need to assemble src/core/handle_idt.asm and link against kernel
IDT_SRC=$(SRC_DIR)/core/handle_idt.asm

BOOT_OBJ=$(BUILD_DIR)/boot.o
STAGE2_OBJ=$(BUILD_DIR)/stage2.o
KERNEL_OBJS=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRC))
KERNEL_OBJS += $(BUILD_DIR)/core/handle_idt.o
TARGET=$(BUILD_DIR)/boot.bin

.PHONY: all clean run debug

all: $(TARGET)

debug:
	@echo "KERNEL_SRC: $(KERNEL_SRC)"
	@echo "KERNEL_OBJS: $(KERNEL_OBJS)"

$(TARGET): $(BOOT_OBJ) $(STAGE2_OBJ) $(KERNEL_OBJS)
	$(LD) -m elf_i386 -T numsectors.ld --oformat binary -o $@ $^

$(BOOT_OBJ): $(BOOT_SRC)
	@mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(STAGE2_OBJ): $(STAGE2_SRC)
	@mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(BUILD_DIR)/core/handle_idt.o: $(IDT_SRC)
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	@./test_image.sh $(TARGET)

clean:
	rm -rf $(BUILD_DIR)

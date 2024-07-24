BUILD_DIR = build

# Ensure the build directory is created
all: $(BUILD_DIR) os-image.bin

# Rule to create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build kernel binary from object files
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Compile kernel entry object
$(BUILD_DIR)/kernel_entry.o: boot/kernel_entry.asm | $(BUILD_DIR)
	nasm $< -f elf -o $@

# Compile kernel object
$(BUILD_DIR)/kernel.o: kernel/kernel.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -c $< -o $@

# Disassemble kernel binary (optional)
$(BUILD_DIR)/kernel.dis: $(BUILD_DIR)/kernel.bin
	ndisasm -b 32 $< > $@

# Assemble boot sector
$(BUILD_DIR)/boot.bin: boot/boot.asm | $(BUILD_DIR)
	nasm -Iboot $< -f bin -o $@

# Concatenate boot sector and kernel into OS image
os-image.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

# Run the OS image with QEMU
run: os-image.bin
	qemu-system-i386 -fda $<

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) *.bin *.o *.dis

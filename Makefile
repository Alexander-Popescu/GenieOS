BUILD_DIR = build
SRC_DIR = kernel
DRIVERS_DIR = drivers
BOOT_DIR = boot
UTILS_DIR = util

# Ensure the build directory is created
all: $(BUILD_DIR) os-image.bin

# Rule to create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Find all .c files in the drivers and utils directories
DRIVER_SRCS = $(wildcard $(DRIVERS_DIR)/*.c)
DRIVER_OBJS = $(DRIVER_SRCS:$(DRIVERS_DIR)/%.c=$(BUILD_DIR)/%.o)

UTIL_SRCS = $(wildcard $(UTILS_DIR)/*.c)
UTIL_OBJS = $(UTIL_SRCS:$(UTILS_DIR)/%.c=$(BUILD_DIR)/%.o)

# Build kernel binary from object files
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(DRIVER_OBJS) $(UTIL_OBJS)
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Compile kernel entry object
$(BUILD_DIR)/kernel_entry.o: $(BOOT_DIR)/kernel_entry.asm | $(BUILD_DIR)
	nasm -I$(BOOT_DIR) $< -f elf -o $@

# Compile kernel object
$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(SRC_DIR) -I$(DRIVERS_DIR) -I$(UTILS_DIR) -g -c $< -o $@

# Compile driver objects
$(BUILD_DIR)/%.o: $(DRIVERS_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(DRIVERS_DIR) -g -c $< -o $@

# Compile util objects
$(BUILD_DIR)/%.o: $(UTILS_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(UTILS_DIR) -g -c $< -o $@

# Disassemble kernel binary (optional)
$(BUILD_DIR)/kernel.dis: $(BUILD_DIR)/kernel.bin
	ndisasm -b 32 $< > $@

# Assemble boot sector
$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/boot.asm | $(BUILD_DIR)
	nasm -I$(BOOT_DIR) $< -f bin -o $@

# Concatenate boot sector and kernel into OS image
os-image.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

# Run the OS image with QEMU
run: os-image.bin
	qemu-system-i386 -fda $<

# Build kernel.elf for debugging
$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(DRIVER_OBJS) $(UTIL_OBJS)
	i386-elf-ld -o $@ -Ttext 0x1000 $^

# Debug with GDB
debug: os-image.bin $(BUILD_DIR)/kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin &
	i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) *.bin *.o *.dis

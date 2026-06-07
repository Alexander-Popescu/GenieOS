BUILD_DIR = build
SRC_DIR = kernel
DRIVERS_DIR = drivers
BOOT_DIR = boot
UTILS_DIR = util
CPU_DIR = cpu
APPS_DIR = apps
TESTS_DIR = tests

all: $(BUILD_DIR) os-image.bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

DRIVER_SRCS = $(wildcard $(DRIVERS_DIR)/*.c)
DRIVER_OBJS = $(DRIVER_SRCS:$(DRIVERS_DIR)/%.c=$(BUILD_DIR)/%.o)

UTIL_SRCS = $(wildcard $(UTILS_DIR)/*.c)
UTIL_OBJS = $(UTIL_SRCS:$(UTILS_DIR)/%.c=$(BUILD_DIR)/%.o)

CPU_SRCS = $(wildcard $(CPU_DIR)/*.c)
CPU_OBJS = $(CPU_SRCS:$(CPU_DIR)/%.c=$(BUILD_DIR)/%.o)

KERNEL_SRCS = $(wildcard $(SRC_DIR)/*.c)
KERNEL_OBJS = $(KERNEL_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

APPS_SRCS = $(wildcard $(APPS_DIR)/*.c)
APPS_OBJS = $(APPS_SRCS:$(APPS_DIR)/%.c=$(BUILD_DIR)/%.o)

TESTS_SRCS = $(wildcard $(TESTS_DIR)/*.c)
TESTS_OBJS = $(TESTS_SRCS:$(TESTS_DIR)/%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(KERNEL_OBJS) $(DRIVER_OBJS) $(UTIL_OBJS) $(CPU_OBJS) $(APPS_OBJS) $(TESTS_OBJS) $(BUILD_DIR)/interrupt.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

$(BUILD_DIR)/kernel_entry.o: $(BOOT_DIR)/kernel_entry.asm | $(BUILD_DIR)
	nasm -I$(BOOT_DIR) $< -f elf -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(SRC_DIR) -I$(DRIVERS_DIR) -I$(UTILS_DIR) -I$(CPU_DIR) -I$(APPS_DIR) -I$(TESTS_DIR) -g -c $< -o $@

$(BUILD_DIR)/%.o: $(DRIVERS_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(DRIVERS_DIR) -g -c $< -o $@

$(BUILD_DIR)/%.o: $(UTILS_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(UTILS_DIR) -g -c $< -o $@

$(BUILD_DIR)/%.o: $(CPU_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(CPU_DIR) -g -c $< -o $@

$(BUILD_DIR)/%.o: $(APPS_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(APPS_DIR) -g -c $< -o $@

$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.c | $(BUILD_DIR)
	i386-elf-gcc -ffreestanding -I$(TESTS_DIR) -I$(UTILS_DIR) -I$(DRIVERS_DIR) -g -c $< -o $@

$(BUILD_DIR)/interrupt.o: $(CPU_DIR)/interrupt.asm | $(BUILD_DIR)
	nasm -I$(CPU_DIR) $< -f elf -o $@

$(BUILD_DIR)/kernel.dis: $(BUILD_DIR)/kernel.bin
	ndisasm -b 32 $< > $@

$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/boot.asm | $(BUILD_DIR)
	nasm -I$(BOOT_DIR) $< -f bin -o $@

# sector count must match 'mov dh, N' in boot.asm
BOOT_SECTORS = 40
STACK_LIMIT = 32256

os-image.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	@KSIZE=$$(stat -f%z $(BUILD_DIR)/kernel.bin); \
	 MAX=$$(($(BOOT_SECTORS) * 512)); \
	 if [ $$KSIZE -gt $$MAX ]; then \
	   echo "kernel.bin is $$KSIZE bytes, bootloader only loads $$MAX"; \
	   echo "  bump 'mov dh' in boot.asm to at least $$(( ($$KSIZE + 511) / 512 ))"; \
	   exit 1; \
	 fi; \
	 if [ $$KSIZE -gt $(STACK_LIMIT) ]; then \
	   echo "heads up: kernel.bin ($$KSIZE bytes) is getting close to the stack at 0x9000"; \
	 fi
	cat $^ > $@
	truncate -s 1474560 $@

run: os-image.bin
	qemu-system-i386 -fda os-image.bin -serial stdio

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel_entry.o $(KERNEL_OBJS) $(DRIVER_OBJS) $(UTIL_OBJS) $(CPU_OBJS) $(APPS_OBJS) $(TESTS_OBJS) $(BUILD_DIR)/interrupt.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^

debug: os-image.bin $(BUILD_DIR)/kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin &
	i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

clean:
	rm -rf $(BUILD_DIR) os-image.bin $(BUILD_DIR)/*.o $(BUILD_DIR)/*.dis $(BUILD_DIR)/*.elf

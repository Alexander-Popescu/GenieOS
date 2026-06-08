#include "VGAModes.h"

// Mode 13h (320x200x256 colors) register state
static uint8_t mode_13h_regs[VGA_NUM_REGS] = {
    /* MISC */
    0x63,
    /* SEQ */
    0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x8E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
};

// Text Mode 3 (80x25) register state
static uint8_t text_mode_3_regs[VGA_NUM_REGS] = {
    /* MISC */
    0x67,
    /* SEQ */
    0x03, 0x00, 0x03, 0x00, 0x02,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
    0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x0C, 0x00, 0x0F, 0x08, 0x00
};

void write_vga_registers(uint8_t *regs) {
    uint8_t i;

    // turn off display (AC)
    rIO8(VGA_INSTAT_READ);
    wIO8(VGA_AC_INDEX, 0x00);

    // Synchronous reset on (SEQ)
    wIO8(VGA_SEQ_INDEX, 0x00);
    wIO8(VGA_SEQ_DATA, 0x01);

    // write MISC
    wIO8(VGA_MISC_WRITE, *regs);
    regs++;

    // write SEQ
    for (i = 0; i < VGA_NUM_SEQ_REGS; i++) {
        wIO8(VGA_SEQ_INDEX, i);
        wIO8(VGA_SEQ_DATA, *regs);
        regs++;
    }

    // unlock CRTC explicitly
    wIO8(VGA_CRTC_INDEX, 0x03);
    wIO8(VGA_CRTC_DATA, rIO8(VGA_CRTC_DATA) | 0x80);
    wIO8(VGA_CRTC_INDEX, 0x11);
    wIO8(VGA_CRTC_DATA, rIO8(VGA_CRTC_DATA) & ~0x80);

    // make sure they remain unlocked in the array we're writing
    regs[0x03] |= 0x80;
    regs[0x11] &= ~0x80;

    // write CRTC
    for (i = 0; i < VGA_NUM_CRTC_REGS; i++) {
        wIO8(VGA_CRTC_INDEX, i);
        wIO8(VGA_CRTC_DATA, *regs);
        regs++;
    }

    // write GC
    for (i = 0; i < VGA_NUM_GC_REGS; i++) {
        wIO8(VGA_GC_INDEX, i);
        wIO8(VGA_GC_DATA, *regs);
        regs++;
    }

    // write AC
    for (i = 0; i < VGA_NUM_AC_REGS; i++) {
        rIO8(VGA_INSTAT_READ);
        wIO8(VGA_AC_INDEX, i);
        wIO8(VGA_AC_WRITE, *regs);
        regs++;
    }

    // Synchronous reset off
    wIO8(VGA_SEQ_INDEX, 0x00);
    wIO8(VGA_SEQ_DATA, 0x03);

    // lock 16-color palette and unblank display
    rIO8(VGA_INSTAT_READ);
    wIO8(VGA_AC_INDEX, 0x20);
}

void enter_graphics_mode() {
    write_vga_registers(mode_13h_regs);
}

void enter_text_mode() {
    write_vga_registers(text_mode_3_regs);
}

static const uint8_t standard_vga_palette[48] = {
    0x00, 0x00, 0x00, // 0: Black
    0x00, 0x00, 0x2A, // 1: Blue
    0x00, 0x2A, 0x00, // 2: Green
    0x00, 0x2A, 0x2A, // 3: Cyan
    0x2A, 0x00, 0x00, // 4: Red
    0x2A, 0x00, 0x2A, // 5: Magenta
    0x2A, 0x15, 0x00, // 6: Brown
    0x2A, 0x2A, 0x2A, // 7: Light Gray
    0x15, 0x15, 0x15, // 8: Dark Gray
    0x15, 0x15, 0x3F, // 9: Light Blue
    0x15, 0x3F, 0x15, // A: Light Green
    0x15, 0x3F, 0x3F, // B: Light Cyan
    0x3F, 0x15, 0x15, // C: Light Red
    0x3F, 0x15, 0x3F, // D: Light Magenta
    0x3F, 0x3F, 0x15, // E: Yellow
    0x3F, 0x3F, 0x3F  // F: White
};

void vga_restore_palette() {
    wIO8(0x3C6, 0xFF); // PEL Mask

    wIO8(VGA_DAC_WRITE_INDEX, 0);
    // Restore first 16 standard text colors
    for (int i = 0; i < 48; i++) {
        wIO8(VGA_DAC_DATA, standard_vga_palette[i]);
    }
    // Pad remaining DAC indices to black
    for (int i = 48; i < 768; i++) {
        wIO8(VGA_DAC_DATA, 0);
    }
}

static uint8_t font_buffer[16384];

void vga_save_font() {
    // Save current GC and SEQ state
    wIO8(VGA_GC_INDEX, 0x04);
    uint8_t old_read_map = rIO8(VGA_GC_DATA);
    wIO8(VGA_GC_INDEX, 0x05);
    uint8_t old_mode = rIO8(VGA_GC_DATA);
    wIO8(VGA_GC_INDEX, 0x06);
    uint8_t old_misc = rIO8(VGA_GC_DATA);
    wIO8(VGA_SEQ_INDEX, 0x04);
    uint8_t old_seq_mem = rIO8(VGA_SEQ_DATA);

    // Setup to read from Plane 2
    wIO8(VGA_GC_INDEX, 0x04);
    wIO8(VGA_GC_DATA, 0x02); // Read Map Select: Plane 2

    wIO8(VGA_GC_INDEX, 0x05);
    wIO8(VGA_GC_DATA, 0x00); // Read Mode 0

    wIO8(VGA_GC_INDEX, 0x06);
    wIO8(VGA_GC_DATA, 0x04); // Memory Map A0000-AFFFF

    wIO8(VGA_SEQ_INDEX, 0x04);
    wIO8(VGA_SEQ_DATA, 0x06); // Sequential memory

    // Copy 16KB from Plane 2
    uint8_t *vga_mem = (uint8_t *)0xA0000;
    for (int i = 0; i < 16384; i++) {
        font_buffer[i] = vga_mem[i];
    }

    // Restore GC and SEQ state
    wIO8(VGA_GC_INDEX, 0x04); wIO8(VGA_GC_DATA, old_read_map);
    wIO8(VGA_GC_INDEX, 0x05); wIO8(VGA_GC_DATA, old_mode);
    wIO8(VGA_GC_INDEX, 0x06); wIO8(VGA_GC_DATA, old_misc);
    wIO8(VGA_SEQ_INDEX, 0x04); wIO8(VGA_SEQ_DATA, old_seq_mem);
}

void vga_restore_font() {
    // Save current GC and SEQ state
    wIO8(VGA_SEQ_INDEX, 0x02);
    uint8_t old_map_mask = rIO8(VGA_SEQ_DATA);
    wIO8(VGA_SEQ_INDEX, 0x04);
    uint8_t old_seq_mem = rIO8(VGA_SEQ_DATA);
    wIO8(VGA_GC_INDEX, 0x05);
    uint8_t old_mode = rIO8(VGA_GC_DATA);
    wIO8(VGA_GC_INDEX, 0x06);
    uint8_t old_misc = rIO8(VGA_GC_DATA);

    // Setup to write to Plane 2
    wIO8(VGA_SEQ_INDEX, 0x02);
    wIO8(VGA_SEQ_DATA, 0x04); // Map Mask: Enable Plane 2

    wIO8(VGA_SEQ_INDEX, 0x04);
    wIO8(VGA_SEQ_DATA, 0x06); // Sequential memory

    wIO8(VGA_GC_INDEX, 0x05);
    wIO8(VGA_GC_DATA, 0x00); // Write Mode 0

    wIO8(VGA_GC_INDEX, 0x06);
    wIO8(VGA_GC_DATA, 0x04); // Memory Map A0000-AFFFF

    // Write 16KB back to Plane 2
    uint8_t *vga_mem = (uint8_t *)0xA0000;
    for (int i = 0; i < 16384; i++) {
        vga_mem[i] = font_buffer[i];
    }

    // Restore GC and SEQ state
    wIO8(VGA_SEQ_INDEX, 0x02); wIO8(VGA_SEQ_DATA, old_map_mask);
    wIO8(VGA_SEQ_INDEX, 0x04); wIO8(VGA_SEQ_DATA, old_seq_mem);
    wIO8(VGA_GC_INDEX, 0x05); wIO8(VGA_GC_DATA, old_mode);
    wIO8(VGA_GC_INDEX, 0x06); wIO8(VGA_GC_DATA, old_misc);
}

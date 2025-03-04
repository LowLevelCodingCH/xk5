/* Stole all this, do NOT expect anything beautiful or so */


struct idt_entry {
    uint16_t base_low;   // Lower 16 bits of the handler address
    uint16_t selector;   // Kernel segment selector
    uint8_t  always0;     // This must always be zero
    uint8_t  flags;     // Flags
    uint16_t base_high; // Upper 16 bits of the handler address
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

void set_idt_entry(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].selector = 0x08; // Kernel code segment
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;    // Present, ring 0, 32-bit interrupt gate
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void load_idt() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;
    __asm__ __volatile__("lidt (%0)" : : "r" (&idtp));
}

void default_exception() {
    kprintln("EXCEPTION ERROR");
	return;
}

void swi_80() {
	kprintln("Yay, i got called 0x80");
	return;
}

extern void defhandler();
extern void swi_80_handler();

void init_idt() {
	for (int i = 0; i < 256; i++) {
    	set_idt_entry(i, (uint32_t)defhandler);
	}

	set_idt_entry(0x80, (uint32_t)swi_80_handler);

	load_idt();

	klog_println("idt: IDT Loaded");
}

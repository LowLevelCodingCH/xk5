/* Basic portIO. Mario means MAsteR Input Output*/

void outw(WORD port, WORD val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

WORD inw(WORD port) {
    WORD ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(WORD port, BYTE val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

BYTE inb(WORD port) {
    BYTE ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outl(uint16_t port, uint32_t value) {
    asm("outl %%eax, %%dx" :: "d" (port), "a" (value));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm("inl %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}


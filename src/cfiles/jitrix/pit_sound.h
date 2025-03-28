void init_pit(uint32_t frequency) {
    uint32_t divisor = PIT_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36);  // Command byte: Channel 0, Access mode: lobyte/hibyte, Mode: 3, Binary mode
    outb(PIT_CHANNEL_0, (uint8_t)(divisor & 0xFF));       // Low byte of divisor
    outb(PIT_CHANNEL_0, (uint8_t)((divisor >> 8) & 0xFF));// High byte of divisor
}

void playSound(uint32_t nFrequence) {
    uint32_t Div;
    uint8_t tmp;
    // Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (Div) );
    outb(0x42, (uint8_t) (Div >> 8));
    // And play the sound using the PC speaker
    tmp = inb(0x61);
     if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

void shutup() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

void wait_ms(unsigned int milliseconds) {
    unsigned int count = 11932 * milliseconds; // PIT frequency is approximately 11932 Hz

    // Write initial count value
    outb(count & 0xFF, PIT_CHANNEL0_PORT); // Low byte
    outb((count >> 8) & 0xFF, PIT_CHANNEL0_PORT); // High byte

    // Wait until timer reaches 0
    while (count > 0) {
        // You may want to include some kind of yield or sleep mechanism here to avoid busy-waiting
        count--;
    }
}

void beep(uint32_t pitch, uint32_t ms) {
    playSound(pitch);
    wait_ms(ms);
    shutup();
}

void XK_Prefix(PlayS)(sound s) {
    beep(s.pitch, s.ms);
}

void XK_Prefix(PlayCS)(sound* sl, int len) {
    for(int i = 0; i < len; i++) {
        XK_PlayS(sl[i]);
    }
}

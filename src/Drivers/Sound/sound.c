#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43

void init_speaker() {
	outb(0x36, 0x43);
}

void speaker_off() {
	uint8_t tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}

void play_tone(uint32_t frequency) {
	uint32_t Div;
	uint8_t tmp;
	Div = 1193180 / frequency;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (Div) );
	outb(0x42, (uint8_t) (Div >> 8));
	tmp = inb(0x61);
 	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

void wait_ms(unsigned int milliseconds) {
	unsigned int count = 11932 * milliseconds;

	outb(count & 0xFF, PIT_CHANNEL0_PORT);
	outb((count >> 8) & 0xFF, PIT_CHANNEL0_PORT);

	while (count > 0) {
		count--;
	}
}

void beep(uint32_t pitch, uint32_t ms) {
	play_tone(pitch);
	wait_ms(ms);
	speaker_off();
}

void init_pit() {
	outb(0x36, PIT_COMMAND_PORT);
}

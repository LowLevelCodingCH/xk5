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

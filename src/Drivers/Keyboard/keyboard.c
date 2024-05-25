const unsigned char key_map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0xFF, '\t',
	'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n', 'C', 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '#', 'y', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0xFD, '*', 'A', ' ',
	0x77, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFE, 'n', '7',
	0x77, 0x00, '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	0x77, 0x00, 0xFB, 0xFC
};

const unsigned char key_map_shifted[256] = {
	0, 27, '!', '"', '$', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0xFF, '\t',
	'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '{', '}', '\n', 'C', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 0, '#', 'Y', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '?', 0xFD, '*', 'A', ' ',
	'C', 0XF1, 0XF2, 0XF3, 0XF4, 0XF5, 0XF6, 0XF7, 0XF8, 0XF9, 0XFA, 0xFE, 0xFE, '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	'A', 'B', 0XFB, 0XFC
};

char read_char() {
	char status;
	do {
		status = inb(0x64);
	} while ((status & 0x01) == 0);

	unsigned char scancode = inb(0x60);

	static int shift_pressed = 0;

	if (scancode == 0x2A) {
		shift_pressed = 1;
		return 0;
	} else if (scancode == 0xAA) {
		shift_pressed = 0;
		return 0;
	}

	return (scancode & 0x80) ? 0 : (shift_pressed ? key_map_shifted[scancode] : key_map[scancode]);
}


void read_word(char *buffer, const int buffer_size, int addedsz) {
	int index = 0;
	char c;

	while (index < buffer_size - 1) {
		c = read_char();

		if (c == '\n') {
			buffer[index] = '\0';
			kputln(ssize(buffer)+addedsz);
			return;
		} else if (c == 0xFF) {
			if (index > 0) {
				index--;
			}
		} else if (c >= ' ' && c <= '~') {
			buffer[index++] = c;
			kputc(c);
		}
	}

	buffer[index] = '\0';
	kputc('\n');
}

#include <Drivers/drivers.h>

const unsigned char key_map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '0', '2', 0x05, '\t',
	'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n', 0x02, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '#', 'y', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0xFD, '*', 0x03, ' ',
	0x77, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFE, 'n', '7',
	0x77, 0x00, '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	0x77, 0x00, 0xFB, 0xFC
};

// 0x03 = alt, 0x02 = ctrl

const unsigned char key_map_shifted[256] = {
	0, 27, '!', '"', '$', '$', '%', '^', '&', '(', ')', '=', '?', '+', 0x05, '\t',
	'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '{', '}', '\n', 0x02, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 0, '\'', 'Y', 'X', 'C', 'V',
	'B', 'N', 'M', ';', ':', '?', 0xFD, '*', 0x03, ' ',
	'C', 0XF1, 0XF2, 0XF3, 0XF4, 0XF5, 0XF6, 0XF7, 0XF8, 0XF9, 0XFA, 0xFE, 0xFE, '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	'A', 'B', 0XFB, 0XFC
};

char read_char(int* ctrlpressed) {
	char status;
	do {
		status = inb(0x64);
	} while ((status & 0x01) == 0);

	unsigned char scancode = inb(0x60);

	static int shift_pressed = 0;

	static int ctrl_pressed  = 0;

	if (scancode == 0x2A) {
		shift_pressed = 1;
		return 0;
	} else if (scancode == 0xAA) {
		shift_pressed = 0;
		return 0;
	}

	if (scancode == 0x02) {
		ctrl_pressed = 1;
	}

	*ctrlpressed = ctrl_pressed;

	return (scancode & 0x80) ? 0 : (shift_pressed ? key_map_shifted[scancode] : key_map[scancode]);
}


void read_word(char *buffer, const int buffer_size, int addedsz, int backspace_limit) {
	int index = 0;
	int in = index;
	char c;
	int ct;

	while (index < buffer_size - 1) {
		c = read_char(&ct);

		if (c == '\n') {
			buffer[index] = '\0';
			kputln(ssize(buffer)+addedsz);
			return;
		} else if (c == 0x05 && index > backspace_limit) {
			index--;
			sub_cursor();
			kputc(' ');
			sub_cursor();
		} else if (c >= ' ' && c <= '~') {
			buffer[index++] = c;
			kputc(c);
		}
	}

	buffer[index] = '\0';
	kputc('\n');
}

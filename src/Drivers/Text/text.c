#include <Drivers/drivers.h>

static int len = 80;

int offs = -32; // not 8x16 font called "font" use 0

static int cursor = 0;

void sub_cursor() {
  cursor -= 2;

  int position = (cursor / 2);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (position >> 8) & 0xFF);

  outb(0x3D4, 0x0F);
  outb(0x3D5, position & 0xFF);
}

int get_cursor(){return cursor;}

void set_cursor(int g){cursor = g;}

void update_cursor() {
    cursor += 2;
    int position = (cursor / 2);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (position >> 8) & 0xFF);

    outb(0x3D4, 0x0F);
    outb(0x3D5, position & 0xFF);
}

void reset_cursor() {
  cursor = 0;

  outb(0x3D4, 0x0E);
  outb(0x3D5, (0 >> 8) & 0xFF);

  outb(0x3D4, 0x0F);
  outb(0x3D5, 0 & 0xFF);
}

void clear() {
  reset_cursor();
  for (size_t y = 0; y < 1000; y++) {
    for (size_t x = 0; x < 800; x++) {
      const size_t index = y * 80 + x;
      if(index % 2 == 0) *(char*)(0xb8000 + index) = ' ' + offs;
      if(index % 2 == 0) *(char*)(0xb8000 + index - 1) = 15 | 1 << 4;
      if(index % 2 == 0) *(char*)(0xb8000 + index + 1) = 2;
    }
  }
  reset_cursor();
}

void kpanic(char* string, int blue)
{
    for(int i = 0; i < ssize(string); i++) {
        *(char*)(0xb8000 + cursor)     = string[i] + offs;
        *(char*)(0xb8000 + cursor + 1) = 7 | 4 << 4;
        if(blue == 1) *(char*)(0xb8000 + cursor + 1) = 4 | 1 << 4;
        update_cursor();
    }
}

void kpanicln(char* string, int blue)
{
    kpanic(string, blue);
    for(int i = 0; i < (len - ssize(string)); i++)
        update_cursor();
}

void kgreen(char* string)
{
    for(int i = 0; i < ssize(string); i++) {
        *(char*)(0xb8000 + cursor)     = string[i] + offs;
        *(char*)(0xb8000 + cursor + 1) = 10 | 1 << 4;
        update_cursor();
    }
}

void kgreenln(char* string)
{
    kgreen(string);
    for(int i = 0; i < (len - ssize(string)); i++)
        update_cursor();
}

void kclearcol(char bgc) {
  reset_cursor();
  for(int i = 0; i < 200; i++) {
    for(int i = 0; i < 320; i++)
      kpanic(" ", 0);
    kputln(320);
  }
  reset_cursor();
}

void kprintadd(char* string, int to_add) {
  char* s = 0x01;
  for(int i = 0; i < ssize(string); i++) {
    s[i] = string[i] + to_add;
  }
  s[ssize(string)] = 0x00;
  kprint(s);
}

int ssize(char* str) {
	int len = 0;
	while (str[len])
		len++;
	return len;
}

char* scpy(register char *to, register const char *from) {
	char *save = to;
	for (int k = 0; from[k] != '\0'; ++k) to[k] = from[k];
	return save;
}

char* scat(char* s1, char* s2) {
	  scpy(s1 + ssize (s1), s2);
	  return s1;
}

void cmdsplit(char* sys, char del, char result[][50], int* count) {
  int i = 0;
  int j = 0;
  int k = 0;

  while(sys[i] != 0) {
    if(sys[i] == del) {
      if(sys[i + 1] != del && sys[i + 1] != 0x00) {
        result[j][k] = 0;
        j++;
        k = 0;
      }
      if(j >= 10) {
        break;
      }
    } else {
      if(k < 50) {
        result[j][k] = sys[i];
        k++;
      }
    }
    i++;
  }
  result[j][k] = 0;
  *count = j+1;
}

int streq(const char *s1, const char *s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

void kputc(char c) {
    *(char*)(0xb8000 + cursor) = c + offs;
    update_cursor();
}

void kputln(int ssi) {
    for(int i = 0; i < (len - ssi); i++)
        update_cursor();
}

void kprint(char* string)
{
    for(int i = 0; i < ssize(string); i++) {
        *(char*)(0xb8000 + cursor) = string[i] + offs;
        update_cursor();
    }
}

void kreadme(char* string) {
    for(int i = 0; i < ssize(string); i++) {
        *(char*)(0xb8000 + cursor)     = string[i] + offs;
        *(char*)(0xb8000 + cursor + 1) = 14;
        *(char*)(0xb8000 + cursor + 1) = 14 | 1 << 4;
        update_cursor();
    }
}

void kprintln(char* string)
{
    kprint(string);
    for(int i = 0; i < (len - ssize(string)); i++)
        update_cursor();
}

void kreadmeln(char* string) {
    kreadme(string);
    for(int i = 0; i < (len - ssize(string)); i++)
        update_cursor();
}

char* int_to_string(int num) {
    char buffer[10];
    char* str = "";
    int i = 0, j = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    for (j = 0; j < i; j++) {
        str[j] = buffer[i - j - 1];
    }
    str[j] = '\0';
    return str;
}

void scroll_down(){

}

int atoi(char* str) {
    int num = 0;
    int sign = 1;

    // Skip leading whitespace
    while (*str == ' ') {
        str++;
    }



    // Handle optional sign
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Process digits
    while (*str >= '0' && *str <= '9') {
        num = num * 10 + (*str - '0');
        str++;
    }

    // Apply sign
    return sign * num;
}

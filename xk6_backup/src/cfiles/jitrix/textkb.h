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

void sub_cursor() {
  cursor -= 2;

  int position = (cursor / 2);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (position >> 8) & 0xFF);

  outb(0x3D4, 0x0F);
  outb(0x3D5, position & 0xFF);
}

int ssize(const char* str) {
    int len = 0;
    while (str[len])
        len++;
    return len;
}

int streq(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}


void kputc(char c) {
    *(char*)(0xb8000+cursor)=c;
    *(char*)(0xb8000+cursor+1)=_glob_color;
    update_cursor();
}

void kputln(int s) {
    for(int i = 0; i < (80-s); i++) kputc(' ');
}

void kprint(char *s) {
    int size = ssize(s);
    for(int i = 0; i < size; i++) {
        kputc(s[i]);
    }
}

void kprintln(char *s) {
    kprint(s);
    kputln(ssize(s));
}

void kprintbuf(char *buf, int len) {
    for(int i = 0; i < len; i++) {
        if(buf[i] < 32){kputc('.');}
        else{kputc(buf[i]);}
    }
}

void klog_putc(char c) {
    *(char*)(TTY_Begin+lcursor)=c;
    lcursor++;
}

void klog_putln(int s) {
    for(int i = 0; i < (80-s); i++) klog_putc(' ');
}

void klog_print(char *s) {
    int oldgc = _glob_color;
    _glob_color = VGA_COLOR_YELLOW;
    kprint("[kernel::log] -> ");
    _glob_color = oldgc;
    kprint(s);
}

void klog_println(char *s) {
    int oldgc = _glob_color;
    _glob_color = VGA_COLOR_YELLOW;
    kprint("[kernel::log] -> ");
    _glob_color = oldgc;
    kprint(s);
    kputln(ssize(s) + ssize("[kernel::log] -> "));
}

void XK_Prefix(FlushKLOGToStdout)() {
    kprintbuf((char*)TTY_Begin, TTY_End - TTY_Begin);
}

void clear(void) {
    reset_cursor();
    for(int i = 0; i < 80*100; i++)
        kputc(' ');
    reset_cursor();
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


int atoi(char* str) {
    int num = 0;
    int sign = 1;
    int tc = 0;

    while (*str == ' ') {
        str++;
    }

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        num = num * 10 + (*str - '0');
        str++;
        tc++;
    }

    if(tc < 1) return -1000;

    return sign * num;
}

char* itoa(int num) {
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


// JoshuaFarmer/NovaOS and JoshuaFarmer/OSA86 (make up like 50% of DEES NU- drivers. i mean drivers)
// MODIFIED
void printf(const char *format, ...) {
     va_list args;
     va_start(args, format);
     int i = 0;
     const char* p = format;
     while (*p) {
        if (*p == '\n') {
            p++;
            kputln(i);
            i = 0;
        }
        if (*p == '%' && *(p + 1)) {
               p++;
               switch (*p) {
                case 'f': {
                    uint32_t f = (uint32_t) va_arg(args, int);
                    _glob_color = f;
                }
                    case 'c': {
                         char c = (char) va_arg(args, int);
                    kputc(c);
                         i++;
                         break;
                    }
                    case 'd': {
                         int i = va_arg(args, int);
                         kprint(itoa(i));
                         i+=ssize(itoa(i));
                         break;
                    }
                    case 's': {
                         char* str = va_arg(args, char*);
                         kprint(str);
                         i+=ssize(str);
                         break;
                    }
                    default: {
                         kputc('%');
                         kputc(*p);
                         i++;
                    break;
                    }
               }
          } else {
               kputc(*p);
            i++;
          }
          p++;
     }

     va_end(args);
}

void memset(void* des, int value, size_t size) {
    if (des == NULL) return;
    for (size_t x=0; x<size; ++x)
        ((char*)des)[x]=value;
}

void clearBuffer(void * des, size_t size) {
    memset(des, 0, size);
}

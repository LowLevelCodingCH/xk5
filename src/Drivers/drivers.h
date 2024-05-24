#include <stdint.h>
#include <stddef.h>

// PortIO
static inline void outb(uint16_t port, uint8_t val);
static inline uint8_t inb(uint16_t port);

// VGA
void set_vga_mode_13h();
void draw_pixel(int x, int y, uint8_t color);

// Sound
void init_speaker();
void speaker_off();
void play_tone(uint32_t frequency);
void wait_ms(unsigned int milliseconds);
void beep(uint32_t pitch, uint32_t ms);
void init_pit();

// Text
void update_cursor();
int ssize(char* str);
void clear();
char* scpy(register char *to, register const char *from);
void reset_cursor();
char* scat(char* s1, char* s2);
void kputc(char c);
void kputln(int ssi);
void kprint(char* string);
void kprintln(char* string);
char* int_to_string(int num);
int streq(const char *s1, const char *s2);
void cmdsplit(char* sys, char del, char result[][50], int* count);

// Keyboard
char read_char();
void read_word(char *buffer, const int buffer_size, int addedsz);

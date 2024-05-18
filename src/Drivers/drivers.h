#include <stdint.h>

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

// Text
void update_cursor();
int ssize(char* str);
void kputc(char c);
void kputln(int ssi);
void kprint(char* string);
void kprintln(char* string);
char* int_to_string(int num);
int streq(const char *s1, const char *s2);

// Keyboard
char read_char();
void read_word(char *buffer, const int buffer_size, int addedsz);

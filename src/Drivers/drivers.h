#include <stddev.h>

// PortIO
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

// VGA
void set_vga_mode_13h();
void draw_pixel(int x, int y, uint8_t color);
void set_text_mode(int hi_res);
/*static*/ void write_font(unsigned char *buf, unsigned font_height);
void lidfk();
void write_pixel1(unsigned x, unsigned y, unsigned c);
/*static*/ unsigned get_fb_seg(void);
/*static*/ void vpokeb(unsigned off, unsigned val);
/*static*/ void font512(void);
void draw_char(char c, int x, int y, int Colour);
void draw_text(char* s, int x, int y, int Colour);
void vga_test();

// Sound
void init_speaker();
void speaker_off();
void play_tone(uint32_t frequency);
void wait_ms(unsigned int milliseconds);
void beep(uint32_t pitch, uint32_t ms);
void init_pit();

// Text
void sub_cursor();
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
void kgreen(char* string);
void kgreenln(char* string);
void kreadmeln(char* string);
void kreadme(char* string);
void kpanic(char* string, int blue);
void kpanicln(char* string, int blue);
char* int_to_string(int num);
int   atoi(char* str);
int streq(const char *s1, const char *s2);
void cmdsplit(char* sys, char del, char result[][50], int* count);
void scroll_down();
int get_cursor();
void kclearcol(char bgc);
void set_cursor(int g);
void kprintadd(char* string, int to_add);

// Keyboard
char read_char();
void read_word(char *buffer, const int buffer_size, int addedsz, int backspace_limit);

// Etcetera
void halt();
unsigned int balloc(unsigned int size, unsigned int address);



//void unlock(Spinlock* spinlock);
//void lock(Spinlock* spinlock);
//void spinlock(Spinlock* spinlock);

// Disk
void identify_ata(uint8_t drive);
void wait_BSY();
void wait_DRQ();
void insw(uint16_t port, void* addr, uint32_t count);
void ata_wait_bsy();
void ata_wait_drq();
void ata_read_sector(uint32_t lba, uint8_t* buffer);
uint32_t chs2lba(int c, int h, int s);
int  ATA_disk_write(const BYTE* buff, LBA_t sector, UINT count);
int disk_write (
    BYTE pdrv,
    const BYTE *buff,
    LBA_t sector,
    UINT count
);
int disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
);
int ATA_disk_initialize(void);
void ata_chs_read(uint16_t cylinder, uint8_t head, uint8_t sector, uint8_t sector_count, uint16_t* buffer);

// Programs/write.c
void write();

#ifndef JS
#define JS

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//typedef unsigned short uint16_t;
//typedef unsigned char uint8_t;
//typedef unsigned int uint32_t;

typedef struct Bytearray {
  uint32_t length;
  char*    begin;
} Bytearray;

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define vgabuff 0xA0000
#define videobuff 0xB8000
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define STATUS_BSY	  0x80
#define STATUS_RDY	  0x40
#define STATUS_DRQ	  0x08
#define STATUS_DF		0x20
#define STATUS_ERR	  0x01
#define read_sector(drive, sector, buffer) LBA28_read_sector(drive, sector, 1, buffer)
#define write_sector(drive, sector, data) LBA28_write_sector(drive, sector, 1, data)
#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define SECTOR_SIZE 512
#define ROOT_DIR_SECTOR 2
#define RESERVED_SECTORS 32
#define FAT_SECTORS 32
#define cli() asm("cli");
#define sti() asm("sti");

typedef uint32_t error_t;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef WORD WCHAR;
typedef uint32_t UINT;
typedef uint32_t LBA_t;
typedef uint32_t FSIZE_t;

#endif // JS

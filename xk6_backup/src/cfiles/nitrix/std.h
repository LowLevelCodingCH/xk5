typedef void* KNOB;
typedef unsigned long  FILE;
typedef unsigned int   size_t;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef unsigned long  QWORD;

typedef unsigned char  byte_t;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;

typedef char *va_list;
#define va_start(ap,parmn) (void)((ap) = (char*)(&(parmn) + 1))
#define va_end(ap) (void)((ap) = 0)
#define va_arg(ap, type) \
    (((type*)((ap) = ((ap) + sizeof(type))))[-1])

#define XK_Prefix(S)   XK_##S
#define Nx_Prefix(S)   Nx_##S

#define MOVE "Black: rook g7"

#define TTY_Begin      (0xc18000)
#define TTY_End        (0xc1b000)

#define OUTMEM 0xb8000

#define PAGESIZE       4096
#define PAGEAMOUNT     1024

#define TASK_EXIST_FLAG 121
#define TASK_RUNNING_FLAG 1
#define TASK_FOREVER_FLAG -1

/* Either 60816 or 26484*/

#define BASE_END_ALL 60816

#define MAX_TASK_AMOUNT 16

#define NULL           ((void*)(0))

#define vector(T,N) typedef struct N {T *values;size_t size;} N;
#define UNS(T) unsigned T
#define CALL(A) asm("call *%0\n\t" : : "r"(A));
#define JUMP(A) asm("jmp *%0\n\t" : : "r"(A));
#define POKE(A,V) * ( BYTE * ) ( A ) = V ;
#define PEEK(A) ( * ( BYTE * ) ( A ) )
#define POKED(A,V) * ( DWORD * ) ( A ) = V ;
#define PEEKD(A) ( * ( DWORD * ) ( A ) )

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT   0x60

#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43

#define PIT_FREQUENCY 1193182
#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND   0x43
#define PIC1_COMMAND  0x20
#define PIC1_DATA	 0x21
#define PIC2_COMMAND  0xA0
#define PIC2_DATA	 0xA1

#define INTERRUPT_GATE 0x8E

#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CTRL 0x3F6
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_REG_HDDEVSEL 0x06
#define STATUS_RDY 0x40
#define IRQ_MASK_ATA_PRIMARY 0x40 // Mask for IRQ 14 (Primary ATA)
#define IRQ_MASK_ATA_SECONDARY 0x80 // Mask for IRQ 15 (Secondary ATA)
#define ATA_PRIMARY_CONTROL_BASE 0x3F6
#define STATUS_BSY	  0x80
#define STATUS_RDY	  0x40
#define STATUS_DRQ	  0x08
#define STATUS_DF		0x20
#define STATUS_ERR	  0x01

//#define CoolLoadingBar
#define CoolLoadingBarDelay 3000

/* Time and date  copied from wiki.osdev.org */
#define CURRENT_YEAR        2025                            // Change this each year! (original: 2023)
int century_register = 0x00;                                // Set by ACPI table parsing code if possible

unsigned char century;
unsigned char curr_second;
unsigned char curr_minute;
unsigned char curr_hour;
unsigned char curr_day;
unsigned char curr_month;
unsigned char curr_year;
unsigned char curr_century;
unsigned char registerB;

unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;

#define in_byte(P) inb(P)
#define out_byte(P,V) outb(P,V)

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

enum vga_colour {
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
	VGA_COLOR_YELLOW = 14,
	VGA_COLOR_WHITE = 15,
};

typedef struct {
	KNOB adr;
	size_t pam;
	FILE fd;
} HANDLE;

typedef struct {
	int pitch;
	int ms;
} sound;

typedef struct bprgm {
	uint32_t begin_exec;
	uint32_t end_exec;
} bprgm;

typedef enum {
	SYS_WRITE,
	SYS_READ,
	SYS_DISK_READ,
	SYS_DISK_WRITE
} syscall;

typedef struct {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t eip;
} regs_t;

typedef struct {
	char running;
	char exists;
	int length;
	char hasupdate;
	int current_tick;
	char name[32];
	void (*update)(void);
	void (*init)(void);
	uint8_t stack[8192];
} Task;

typedef struct {
	char XKFS_Label[4];
	char XKFS_Version[2];
	char XKFS_DiskLabel;
	char XKFS_MaxNameSize;
	short XKFS_BlockSize;
	char XKFS_MaxFileSizeBlocks;
} XKFS_Header;

typedef struct {
	unsigned char files_amount;
} FST_Header;

typedef struct {
	unsigned char size[16];
	unsigned char blocks[16];
	unsigned char name[16];
} FST_Entry;

typedef struct {
	unsigned char name[16];
	unsigned char passcode[16];
} User;

Task *_glob_tasks[MAX_TASK_AMOUNT];
uint32_t _glob_tick = 0;
uint32_t _glob_amount = 0;
uint32_t _glob_color = VGA_COLOR_WHITE;

char buffer[50];
char cmdbuffer[16][50];
int cnt = 0;
char try_username[16] = {0};
char try_passcode[16] = {0};
char user[16] = {0};
int logged_in = 0;

unsigned char century;
unsigned char curr_second;
unsigned char curr_minute;
unsigned char curr_hour;
unsigned char curr_day;
unsigned char curr_month;
unsigned char curr_year;
unsigned char curr_century;
unsigned char registerB;

unsigned char century;
unsigned char last_second;
unsigned char last_minute;
unsigned char last_hour;
unsigned char last_day;
unsigned char last_month;
unsigned char last_year;
unsigned char last_century;
unsigned char registerB;

int cursor = 0;
int lcursor = 0;

FILE XK_Prefix(FileDescriptors)[128];
UNS(char) XK_Prefix(MemoryPool)[PAGESIZE*PAGEAMOUNT];
UNS(int) XK_Prefix(NextFreePage) = 0;

XKFS_Header XKFS;
FST_Entry FST_Entries[256];
FST_Header FST;
byte_t FileSysHeader[512] = {0};
byte_t FST_Blocks[256][512] = {0};
byte_t FileSegmentTableHeader[512] = {0};
byte_t FileSegmentTableEntries[512*8] = {0};

sound startup_beep = {
		.pitch = 440,
		.ms    = 500
};

const unsigned char key_map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '0', '2', 0x05, '\t',
	'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n', 0x02, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '#', 'y', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0xFD, '*', 0x03, ' ',
	0x77, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFE, 'n', '7',
	0x77, 0x00, '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	0x77, 0x00, 0xFB, 0xFC
};

const unsigned char key_map_shifted[256] = {
	0, 27, '!', '"', '$', '$', '%', '^', '&', '(', ')', '=', '?', '+', 0x05, '\t',
	'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '{', '}', '\n', 0x02, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 0, '\'', 'Y', 'X', 'C', 'V',
	'B', 'N', 'M', ';', ':', '?', 0xFD, '*', 0x03, ' ',
	'C', 0XF1, 0XF2, 0XF3, 0XF4, 0XF5, 0XF6, 0XF7, 0XF8, 0XF9, 0XFA, 0xFE, 0xFE, '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	'A', 'B', 0XFB, 0XFC
};

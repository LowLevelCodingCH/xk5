#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <Drivers/drivers.h>
#include <Executable/xke.h>
#include <Drivers/VGA/libfeuer.c>
#include <Kernel/keyval.h>

typedef enum {
  PERMISSION_USER,
  PERMISSION_DRIVER,
  PERMISSION_KERNEL
} PERMS;

typedef enum {
  HANDLE_WINDOW,
  HANDLE_SYSCALL,
  HANDLE_EXEC,
  HANDLE_FILE,
  HANDLE_ACCESS,
  HANDLE_KILL
} HTYPE;

int nextfreefd = 1;

typedef struct {
  int   fd;
  int   pid;
  HTYPE type;
  PERMS owner;
  void* memadr;
  size_t size;
  int   disklba;
} HANDLE;


typedef char BYTEH;

HANDLE __TYPE_FUNC_UNUSE_NONE_Create_Simple0_FilelX20_TYPE0_HANDLE(
                                                                   BYTEH csFileName[11],
                                                                   PERMS perm
);

HANDLE __TYPE_FUNC_UNUSE_NONE_Create_Simple0_WindowlX20_TYPE0_HANDLE(
                                                                     PERMS perm
);

void FreeHandle(HANDLE *handle) {
  if(handle->fd != 0) nextfreefd--;
  handle->fd = 0;
  handle->pid = 0;
  handle->type = HANDLE_KILL;
  handle->owner = PERMISSION_KERNEL;
  handle->memadr = NULL;
  handle->size = 0;
  handle->disklba = 0;
}

HANDLE __TYPE_FUNC_UNUSE_NONE_Create_Simple0_FilelX20_TYPE0_HANDLE(
                                                                   BYTEH csFileName[11],
                                                                   PERMS perm
) {
  // 7 first chars = file name
  // 1 char = .
  // 3 chars = extension

  char fname[7];
  fname[0] = csFileName[0];
  fname[1] = csFileName[1];
  fname[2] = csFileName[2];
  fname[3] = csFileName[3];
  fname[4] = csFileName[4];
  fname[5] = csFileName[5];
  fname[6] = csFileName[6];

  char dot;
  dot = csFileName[7];

  if(dot != '.') {
    kpanicln("FILE HAS NO '.' AT STRADR0~7", 1);
    kgreenln("CONTINUING");
  }

  char extension[3];
  extension[0] = csFileName[8];
  extension[1] = csFileName[9];
  extension[2] = csFileName[10];

  // Find the file SMHW

  HANDLE handle;
  handle.fd = nextfreefd;
  handle.pid = 0;
  handle.type = HANDLE_FILE;
  handle.owner = perm;
  handle.memadr = NULL;
  handle.size = 0;
  handle.disklba = 0;

  nextfreefd++;
  return handle;
}

HANDLE __TYPE_FUNC_UNUSE_NONE_Create_Simple0_WindowlX20_TYPE0_HANDLE(
                                                                     PERMS perm
) {
  HANDLE handle;

  handle.fd = nextfreefd;
  handle.pid = 0;
  handle.type = HANDLE_WINDOW;
  handle.owner = perm;
  handle.memadr = NULL;
  handle.size = 0;
  handle.disklba = 0;

  nextfreefd++;
  return handle;
}

void vga_init();
void vga_drawline_hori(int x, int y, char pixel);
//void vga_drawrect(int x, int y, int width, int height, char pixel);

unsigned char mouse_cycle=0;
char mouse_byte[3];
char mouse_x=0;
char mouse_y=0;

void mouse_handler() {
	switch(mouse_cycle) {
		case 0:
			mouse_byte[0] = inb(0x60);
			mouse_cycle++;
			break;
		case 1:
			mouse_byte[1] = inb(0x60);
			mouse_cycle++;
			break;
		case 2:
			mouse_byte[2] = inb(0x60);
			mouse_x = mouse_byte[1];
			mouse_y = mouse_byte[2];
			mouse_cycle = 0;
			break;
	}
}

void mouse_wait(unsigned char a_type) {
	unsigned int _time_out=100000;
	if(a_type==0) {
		while(_time_out--) {
			if((inb(0x64) & 1)==1) {
				return;
			}
		}
		return;
	}
	else {
		while(_time_out--) {
			if((inb(0x64) & 2) == 0) {
				return;
			}
		}
		return;
	}
}

void mouse_write(unsigned char a_write) {
	mouse_wait(1);
	outb(0x64, 0xD4);
	mouse_wait(1);
	outb(0x60, a_write);
}

unsigned char mouse_read() {
	mouse_wait(0);
	return inb(0x60);
}

void mouse_install() {
	unsigned char _status;
	mouse_wait(1);
	outb(0x64, 0xA8);
	mouse_wait(1);
	outb(0x64, 0x20);
	mouse_wait(0);
	_status=(inb(0x60) | 2);
	mouse_wait(1);
	outb(0x64, 0x60);
	mouse_wait(1);
	outb(0x60, _status);
	mouse_write(0xF6);
	mouse_read();
	mouse_write(0xF4);
	mouse_read();
}

#define new(X,Y) X Y = (X){0}

#define SPRITE_WIDTH  12
#define SPRITE_HEIGHT 12

typedef struct {
  char* a;
  int   b;
} tuple;
// Change tuple to add more fields and change the types (also need to change get_tuple_value and get_hmap_value)

int get_tuple_value(const tuple *t, char* i) {
  if(streq(i, t->a) == 0)
    return t->b;
  return 9999;
}

typedef struct {
  tuple map[10];
} hmap;

int get_hmap_value(const hmap *h, char* d) {
  for(int i = 0; i < 10; i++)
    if(get_tuple_value(&h->map[i], d) != 9999)
      return get_tuple_value(&h->map[i], d);
  return 9999;
}

#define INT_SIZE sizeof(int)
#define PAGESIZE 4096

extern int cursor;

#define unless(X) if(!(X))

int cfindex = 0;

void dont_do_fucking_anything_bitch(){
  return;
}

typedef struct {
  int  free;
  char data[PAGESIZE];
  int  addr;
} page;

int mem_begin = PAGESIZE * 0;
int mem_end   = PAGESIZE * 3;

// Begin: 0x0000, End: 0x3000

page page0 = {1, {0}, PAGESIZE * 0};
page page1 = {1, {0}, PAGESIZE * 1};
page page2 = {1, {0}, PAGESIZE * 2};
page page3 = {1, {0}, PAGESIZE * 3};

#define true  1
#define false 0

char* passcode = "";

page* palloc() {
  if(page0.free == 1) {
    page0.free = 0;
    return &page0;
  } else if(page1.free == 1) {
    page1.free = 0;
    return &page1;
  } else if(page2.free == 1) {
    page2.free = 0;
    return &page2;
  } else if(page3.free == 1) {
    page3.free = 0;
    return &page3;
  } else {
    kprintln("[ERROR]: tried to allocate allocated page!");
    return NULL;
  }
}

void pfree() {
  if(page0.free == 0) {
    page0.free = 1;
  } else if(page1.free == 0) {
    page1.free = 1;
  } else if(page2.free == 0) {
    page2.free = 1;
  } else if(page3.free == 0) {
    page3.free = 1;
  } else {
    kprintln("[ERROR]: tried to free freed page!");
  }
}

void pfreeall() {
  pfree();
  pfree();
  pfree();
  pfree();
}

void pallocall() {
  palloc();
  palloc();
  palloc();
  palloc();
}

void pinit() {
  page0.addr = PAGESIZE*0;
  page1.addr = PAGESIZE*1;
  page2.addr = PAGESIZE*2;
  page3.addr = PAGESIZE*3;

  page0.free = 1;
  page1.free = 1;
  page2.free = 1;
  page3.free = 1;
}

int padding0[100] = {0};

char* user = "DEFAULT.XK5.USER";

typedef char* str;

typedef struct {
  str name;
  str data;
} memf;

typedef enum {
  XBLACK,
  XBLUE,
  XGREEN,
  XCYAN,
  XRED,
  XPURPLE,
  XBROWN,
  XGRAY,
  XDARKGRAY,
  XLIGHTBLUE,
  XLIGHTGREEN,
  XLIGHTCYAN,
  XLIGHTRED,
  XLIGHTPURPLE,
  XYELLOW,
  XWHITE
} COLORS;

PERMS permissions;
int in_driver   = false;

void lscmd(void) {
    kprintln("LSCMD     XKB"); // XKB = X Kernel Builtin
    kprintln("RDISK     XKB");
    kprintln("EXEC      XKB");
    kprintln("VER       XKB");
    kprintln("ECHO      XKB");
    kprintln("PERM      XKB");
    kprintln("GMODE     XKB");
    kprintln("WRITE     XKB");
    kprintln("KENTER    XKB");
    kprintln("KLEAVE    XKB");
    kprintln("CMDPNT    XKB");
    kprintln("XKBASIC   XKB");
    kprintln("HALT      XKB");
    kprintln("BEEP      XKB");
    kprintln("CLEAR     XKB");
}

void kernel_panic(char* error, int graphical) {
  clear();
  reset_cursor();

  char* prom  = "* ------------------------------------";

  for(int i = 0; i < 10; i++) {
    kpanic(prom, 1);
    for(int i = 0; i < ssize(error); i++)
      kpanic("-", 1);
    kputln(ssize(error) + ssize(prom));
  }

  char* kke = "* -------------KRITICAL KERNEL ERROR: ";

  kpanic(kke, 1);
  kpanic(error, 1);
  kputln(ssize(error) + ssize(error) + ssize(kke));

  for(int i = 0; i < 10; i++) {
    for(int i = 0; i < ssize(error); i++)
      kpanic(" ", 1);
    kpanic(prom, 1);
    for(int i = 0; i < ssize(error); i++)
      kpanic("-", 1);
    kputln(ssize(error) * 2 + ssize(prom));
  }

  beep(700, 1000*10*10);
  if(graphical == 1){
    set_vga_mode_13h();

    for(int i = 0; i < 320*200; i++)
      *(char*)(0xA0000 + i) = 4;
  }
  while(1)beep(700, 1000*10*10);
}

memf writememf(char* name, char* data) {
  memf mf;

  mf.name = name;
  mf.data = data;

  return mf;
}

char* permissiontostr(PERMS p) {
  switch(p) {
    case PERMISSION_USER:   return "PERMISSION_USER";
    case PERMISSION_DRIVER: return "PERMISSION_DRIVER";
    case PERMISSION_KERNEL: return "PERMISSION_KERNEL";
    default: return "";
  }
}

void elevateperms(PERMS p) {
  permissions = p;
  unless(permissions == p) {
    kpanicln("[ERROR]: PERMISSION SWITCH DID NOT WORK!!", 1);
  } else {
    kprint("Set permissions to: ");
    kgreen(permissiontostr(permissions));
    kputln(ssize(permissiontostr(permissions)) + 20);
  }
  if(p == PERMISSION_KERNEL) {
    permissions = PERMISSION_KERNEL;
  }
}

typedef struct {
  union {
    char header;
    char upper;
    char lower;
    char footer;
    int ip;
  };
} IPv4;

char basicprog[10][40];

void xkbasic(char* cmd) {
  if(streq(cmd, "clear") != 0 && streq(cmd, "run") != 0 && streq(cmd, "list") != 0) {
    int line = (int)cmd[0] - '0';
    cmd += 2;
    kprintln(cmd);
    kputc((char)(line + '0'));
    kputln(1);
    for(int i = 0; i < 40; i++) {
      if(cmd[i] != 0x00)
        basicprog[line][i] = cmd[i];
    }
  }
  else if(streq(cmd, "list") == 0){
    for(int i = 0; i < 10; i++) {
      kprint(int_to_string(i));
      kputc(' ');
      kprint(basicprog[i]);
      if(i + 1 < 10)kputln(ssize(basicprog[i]) + 2);
    }
  }
  else if(streq(cmd, "clear") == 0){
    clear();
  }
  else {
    for(int i = 0; i < 10; i++) {
      //TODO: Parse every line in basicprog.
    }
  }

  cmd = "";
}

typedef struct {
  COLORS color;
} pixel;

typedef struct {
  pixel r[SPRITE_WIDTH];
} row;

typedef struct {
  row spr[SPRITE_HEIGHT];
} Sprite;

void draw_sprite(int x, int y, Sprite spr, int only) {
  if(only == 1) {
    for(int i = 0; i < 320*200; i++)
      *(char*)(0xa0000 + i) = 15;
  }

  int px = x;
  int py = y;
  for(int i = 0; i < SPRITE_HEIGHT; i++) {
    px = x;
    row r = spr.spr[i];
    for(int j = 0; j < SPRITE_WIDTH; j++) {
      pixel p = r.r[j];
      if(p.color != 15)draw_pixel(px, py, p.color);
      px++;
    }
    py++;
  }
}

Sprite create_sprite(pixel tex[SPRITE_WIDTH][SPRITE_HEIGHT]) {
  Sprite spr = {0};

  int x=0;
  int y=0;

  int index = 0;

  for(int i = 0; i < SPRITE_WIDTH; i++) {
    for(int j = 0; j < SPRITE_HEIGHT; j++) {
      x++;
      spr.spr[y].r[x].color = tex[y][x].color;
    }
    x = 0;
    y++;
  }
  kprintln(int_to_string(__LINE__));

  return spr;
}

pixel cursor_texture[SPRITE_WIDTH][SPRITE_HEIGHT] = {
  {00, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
  {00, 00, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
  {00, 63, 00, 15, 15, 15, 15, 15, 15, 15, 15, 15},
  {00, 63, 63, 00, 15, 15, 15, 15, 15, 15, 15, 15},
  {00, 63, 63, 63, 00, 15, 15, 15, 15, 15, 15, 15},
  {00, 63, 63, 63, 63, 00, 15, 15, 15, 15, 15, 15},
  {00, 00, 00, 63, 00, 00, 15, 15, 15, 15, 15, 15},
  {00, 15, 00, 63, 00, 15, 15, 15, 15, 15, 15, 15},
  {15, 15, 00, 63, 00, 15, 15, 15, 15, 15, 15, 15},
  {15, 15, 15, 00, 00, 15, 15, 15, 15, 15, 15, 15},
  {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
  {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15}
};


pixel letter_a[SPRITE_WIDTH][SPRITE_HEIGHT] = {
  {15, 15, 15, 15, 15, 00, 00, 15, 15, 15, 15, 15},
  {15, 15, 15, 15, 00, 15, 15, 00, 15, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 00, 00, 00, 00, 00, 15, 15, 15},
  {15, 15, 15, 00, 00, 00, 00, 00, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15}
};

pixel letter_b[SPRITE_WIDTH][SPRITE_HEIGHT] = {
  {15, 15, 15, 00, 00, 00, 00, 00, 15, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 00, 00, 00, 00, 15, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 15, 15, 15, 15, 00, 15, 15, 15},
  {15, 15, 15, 00, 00, 00, 00, 00, 15, 15, 15, 15}
};

typedef enum {
  CHAR_A,
  CHAR_B,
  CHAR_W,
  CHAR_S,
  CHAR_D,
} LETTER;

typedef struct {
  unsigned int width;
  unsigned int height;
} field_t;

typedef struct {
  uint32_t w,h,y,x;
} rect_t;

#define MAX_WINDOWS_CNT 3
#define MAX_ELEMENT_CNT 5

typedef struct {
  unsigned int x;
  unsigned int y;
  char exists; // false
} element_t;

typedef struct {
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
  int id;
  char exists;
  char* title;
  element_t elements[MAX_ELEMENT_CNT];
} window_t;

pixel cbtn[SPRITE_WIDTH][SPRITE_HEIGHT] = {
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04},
  {04, 04, 00, 04, 04, 04, 04, 04, 04, 00, 04, 04},
  {04, 04, 04, 00, 04, 04, 04, 04, 00, 04, 04, 04},
  {04, 04, 04, 04, 00, 04, 04, 00, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 00, 00, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 00, 00, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 00, 04, 04, 00, 04, 04, 04, 04},
  {04, 04, 04, 00, 04, 04, 04, 04, 00, 04, 04, 04},
  {04, 04, 00, 04, 04, 04, 04, 04, 04, 00, 04, 04},
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04}
};

void draw_rect(rect_t *rect, unsigned char color) {
  for(int i = 0; i < rect->h; i++) {
    for(int j = 0; j < rect->w; j++) {
      draw_pixel(j+rect->x, i+rect->y, color);
    }
  }
}

void draw_window(window_t *window) {
  Sprite close_btn = create_sprite(cbtn);
  unsigned int width  = window->w;
  unsigned int height = window->h;
  unsigned int x      = window->x;
  unsigned int y      = window->y;
  signed   int id     = window->id;

  field_t field;
  field.width  = width;
  field.height = height;

  // Draw content to the field
  for(int i = 0; i < field.height; i++) {
    for(int j = 0; j < field.width; j++) {
      draw_pixel(j+x, i+y, 7);
    }
  }

  draw_sprite(window->x, window->y, close_btn, 0);

  draw_text(window->title, window->x + SPRITE_WIDTH, window->y, 0);

  // ELEMENTS
  //
  for(int i = 0; i < MAX_ELEMENT_CNT; i++) {
    // DRAW_ELEMENT(ELEMENT);
  }
}

window_t windows[MAX_WINDOWS_CNT]; //TODO: make a ptr so shit is updatable

window_t create_xkwm_window(int width, int height, int x, int y, char *title){
  window_t window;
  window.w = width;
  window.h = height;
  window.x = x;
  window.y = y;
  window.exists = 1;
  window.title = title;
  return window;
} // XKWM

void gmode(void) {
  window_t window = create_xkwm_window(110, 50, 20, 8, "test #0");

  window_t window1 = create_xkwm_window(40, 80, 100, 30, "test #1");

  windows[0] = window;
  windows[1] = window1;

  kprintln(int_to_string(__LINE__));

  int character_point = 0;

  Sprite cur = create_sprite(cursor_texture);

  Sprite let_a = create_sprite(letter_a);
  Sprite let_b = create_sprite(letter_b);

  kprintln(int_to_string(__LINE__));

  LETTER lp;
  int x = 12;
  int y = 12;
  char cgr = 0x00;
  lidfk();
  for(int i = 0; i < 320*200; i++)
    *(char*)(0xa0000 + i) = 15;
  for(;;){
    draw_sprite(x, y, cur, 1);
    // other graphics
    for(int i = 0; i < MAX_WINDOWS_CNT; i++) {
      if(windows[i].exists != 0) {
        draw_window(&windows[i]);
      }
    }
    // here:
    //...
    draw_sprite(x, y, cur, 0);

    cgr = read_char();

    if(cgr == 'w' && y > 0)  {
      y--;
    }
    if(cgr == 's' && y < 200){
      y++;
    }
    if(cgr == 'a' && x > 0)  {
      x--;
    }
    if(cgr == 'd' && x < 320){
      x++;
    }
  }
}

void tpanic(char* error) {
    kclearcol(4);
    kpanic("ERROR: ", 0);
    kpanic(error, 0);
    halt();
}

memf memfiles[5];

void kprintlen(char* buf, int len, int to_add) {
  for(int i = 0; i < len; i++) {
    //kprintadd((char*)(buf + i), to_add);
    kprint((char*)(buf + i));
  }
}

void lookupata(int lba) {
  unsigned char* abuf;
  int ah = disk_read (
      0x0,
      abuf,
      lba,
      1
  );
  kprintlen(abuf, 512, 0);
  if(ah != 0) {
    tpanic("DISK FAULT: Critical Process DIED");
    halt();
  }
}

void intomem(int addr,int off,char val){*(char*)(addr+off)=val;}

unsigned char* abuf;

void callf(int adr) {
  kprint("Calling: ");
  kprint(int_to_string(adr));
  kputln(9 + ssize(int_to_string(adr)));
  __asm__ volatile (
    "jmp %0\n\n"
    :
    : "r"(adr)
    :
  );
}

void system(char cmd[10][50], int cnt) {
    if(streq(cmd[0], "lscmd") == 0 && cnt == 1) {
      lscmd();
    } else if(streq(cmd[0], "rdisk") == 0) {
      if(cnt == 4) {
        int ah = disk_read (
            atoi(cmd[1]),
            abuf,
            atoi(cmd[2]),
            atoi(cmd[3])
        );
        kprintlen(abuf, atoi(cmd[3])*512, 0);
      } else {
        kpanicln("RDISK, 3 args:", 1);
        kpanicln("pdrv", 1);
        kpanicln("lba", 1);
        kpanicln("count", 1);
      }
    } else if(streq(cmd[0], "exec") == 0) {
      if(cnt == 2) {
        callf(atoi(cmd[1]));
      } else {
        kpanicln("EXEC, 1 arg:", 1);
        kpanicln("load_address", 1);
      }
    } else if(streq(cmd[0], "ver") == 0 && cnt == 1) {
      kprintln("X-KERNEL V5");
    } else if(streq(cmd[0], "beep") == 0 && cnt == 1) {
      beep(650, 1000);
    } else if(streq(cmd[0], "gmode") == 0 && cnt == 1) {
      gmode();
    } else if(streq(cmd[0], "write") == 0 && cnt == 1) {
      write();
    } else if(streq(cmd[0], "perm") == 0 && cnt == 1) {
      kprintln(permissiontostr(permissions));
    } else if(streq(cmd[0], "halt") == 0 && cnt == 1) {
      if(permissions == PERMISSION_KERNEL) {
        halt();
      } else {
        kpanicln("[ERROR]: Too Few Permissions, Needed:", 1);
        kpanicln("         PERMISSION_KERNEL", 1);
      }
    } else if(streq(cmd[0], "kenter") == 0 && cnt == 2) {
      if(streq(cmd[1], passcode) == 0) {
        elevateperms(PERMISSION_KERNEL);
      } else {
        kpanicln("[ERROR]: WRONG KERNEL PASSWORD, CANNOT ENTER KERNEL MODE!!", 1);
      }
    } else if(streq(cmd[0], "kleave") == 0 && cnt == 1) {
      elevateperms(PERMISSION_USER);
    } else if(streq(cmd[0], "echo") == 0) {
      int major = 0;
      for(int i = 1; i < cnt; i++) {
        kprint(cmd[i]);
        kputc(' ');
        major += ssize(cmd[i]) + 1;
      }
      kputln(major);
    } else if(streq(cmd[0], "clear") == 0 && cnt == 1) {
      clear();
    } else if(streq(cmd[0], "cmdpnt") == 0 && cnt == 1) {
      kprintln(int_to_string(get_cursor()));
    } else if(streq(cmd[0], "") == 0 || streq(cmd[0], " ") == 0 || streq(cmd[0], "\n") == 0) {
      dont_do_fucking_anything_bitch();
    }
    //else {
    //  kpanicln("[ERROR]: Invalid Command");
    //}
}

void i16_to_8(uint16_t* bytes, char* out, int len) {
  for (size_t i = 0; i < len; ++i) {
    out[i * 2] = (char)(bytes[i] & 0xFF);
    out[i * 2 + 1] = (char)((bytes[i] >> 8) & 0xFF);
  }
}


#endif // SYSTEM_H_

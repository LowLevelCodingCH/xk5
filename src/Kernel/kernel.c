#include <Drivers/drivers.h>

#define PAGESIZE 4096

typedef struct {
  int  free;
  char data[PAGESIZE];
  int  addr;
} page;

int mem_begin = PAGESIZE * 0;
int mem_end   = PAGESIZE * 4;

// Begin: 0x0000, End: 0x4000

page page0 = {1, {0}, PAGESIZE * 0};
page page1 = {1, {0}, PAGESIZE * 1};
page page2 = {1, {0}, PAGESIZE * 2};
page page3 = {1, {0}, PAGESIZE * 3};

#define true  1
#define false 0

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
    kprintln("ERROR: tried to allocate allocated page!");
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
    kprintln("ERROR: tried to free freed page!");
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
  pfreeall();
}

int padding0[100] = {0};

void system(char* cmd) {
    if(streq(cmd, "ls") == 0) {
      kprintln("INITRAMFS     XKE");
    } else if(streq(cmd, "ver") == 0) {
      kprintln("XKERNEL V5");
    }
}

void kernel() {
  char buff[256];
  kprintln("X Kernel V5");
  kprintln("My fifth OS");
  kprintln("(C) LowLevelCodingCH (github.com/)");
  for(;;){
    kprint("xk5> ");
    read_word(buff, 256, ssize("xk5> "));
    // Implement command logic (command stored in buff)
    system(buff);
  }
}

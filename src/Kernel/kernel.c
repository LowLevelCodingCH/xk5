#include <Drivers/drivers.h>

#define PAGESIZE 4096

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
  pfreeall();
}

void pinfo() {
      kprintln("FREE?:");
      kprintln("Page 0: ");
      kprintln(int_to_string(page0.free));
      kprintln("Page 1: ");
      kprintln(int_to_string(page1.free));
      kprintln("Page 2: ");
      kprintln(int_to_string(page2.free));
      kprintln("Page 3: ");
      kprintln(int_to_string(page3.free));
      kprintln("");
      kprintln("ADDRESS?:");
      kprintln("Page 0: ");
      kprintln(int_to_string(page0.addr));
      kprintln("Page 1: ");
      kprintln(int_to_string(page1.addr));
      kprintln("Page 2: ");
      kprintln(int_to_string(page2.addr));
      kprintln("Page 3: ");
      kprintln(int_to_string(page3.addr));
}

int padding0[100] = {0};

char* user = "DEFAULT.XK5.USER";

void system(char cmd[10][50], int cnt) {
    if(streq(cmd[0], "ls") == 0 && cnt == 1) {
      kprintln("LS     XKB"); // XKB = X Kernel Builtin
      kprintln("VER    XKB");
      kprintln("ECHO   XKB");
      kprintln("CLEAR  XKB");
      kprintln("USER   XKB");
      kprintln("PALLOC XKB");
      kprintln("PFREE  XKB");
      kprintln("PINFO  XKB");
    } else if(streq(cmd[0], "palloc") == 0 && cnt == 1) {
      palloc();
    } else if(streq(cmd[0], "pfree") == 0 && cnt == 1) {
      pfree();
    } else if(streq(cmd[0], "pinfo") == 0 && cnt == 1) {
      pinfo();
    } else if(streq(cmd[0], "ver") == 0 && cnt == 1) {
      kprintln("X-KERNEL V5");
    } else if(streq(cmd[0], "echo") == 0) {
      int major = 0;
      for(int i = 1; i < cnt; i++) {
        kprint(cmd[i]);
        kputc(' ');
        major += ssize(cmd[i]) + 1;
      }
      kputln(major);
    } else if(streq(cmd[0], "user") == 0 && cnt == 3) {
      if(streq(cmd[1], "/set") == 0) {
         user = cmd[2];
      } else {
	     kprintln("[ERROR]: Wrong Argument");
      }
    } else if(streq(cmd[0], "clear") == 0 && cnt == 1) {
      clear();
    } else if(streq(cmd[0], "") == 0) {
    } else {
      kprintln("[ERROR]: Invalid Command");
    }
}

void kernel() {
  char buff[256];
  char cmd[10][50];
  kprintln("X Kernel V5");
  kprintln("My Fifth OS");
  kprintln("(C) LowLevelCodingCH");

	init_pit();
	beep(650, 1000);

  for(;;){
    kprint(user);
    kprint("@xk5: ");
    int count = 0;
    read_word(buff, 256, ssize("@xk5: ") + ssize(user));
    cmdsplit(buff, ' ', cmd, &count);
    system(cmd, count);
  }
}

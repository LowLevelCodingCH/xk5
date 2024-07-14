#include <Kernel/system.h>

void kernel(void) {
  set_text_mode(0);
  ATA_disk_initialize();
  identify_ata(0xA0);
  clear();

  char buff[256];
  char cmd[10][50];
  kprintln("X Kernel V5");
  kprintln("My Fifth OS");
  kprintln("(C) LowLevelCodingCH");

  init_pit();
  beep(650, 1000);

  permissions = PERMISSION_USER;

  kprint("username: ");
  read_word(user, 256, ssize("username: "), ssize("X Kernel V5") + ssize("My Fifth OS") + ssize("(C) LowLevelCodingCH") - 42);

  kprint("passcode: ");
  read_word(passcode, 256, ssize("passcode: "), ssize("username: ") + ssize("X Kernel V5") + ssize("My Fifth OS") + ssize("(C) LowLevelCodingCH") - 42 - 10);

  clear();

  kpanicln("Bug reports to:      lowlevelcodingch@outlook.com", 1);
  kreadmeln("As of 2024-06-09, your passcode will be your username and passcode.");
  kreadmeln("Perhaps \"kenter\" will just not work as expected, or both bugs will");
  kreadmeln("strike, so please, don't report that, for I am currently working on it.");
  kreadmeln("This \"OS\" is work-in-progress, so don't expect much from it.");
  kreadmeln("Also, you shouldn't expect much from a swiss 11 year old (me). lmao");
  kreadmeln("Also to note: No software is bug-free, so don't take smaller bugs");
  kreadmeln("asif they're deadly. Thanks in advance!");
  kgreenln("Press <ENTER> to continue...");

  while(read_char() != 10) {}

  clear();

  for(;;){
    kreadme(user);
    kputc('@');
    kpanic("xk5", 1);
    kprint(":> ");
    int count = 0;
    read_word(buff, 256, ssize("@xk5: ") + 1 + ssize(user), ssize("@xk5: ") + 1 - 1 + ssize(user) - 10 - 5 + 5 + 1 - 1 - ssize(user) - 4 + 8);
    cmdsplit(buff, ' ', cmd, &count);
    if(streq(cmd[0], "xkbasic") == 0 && count == 1) {
      clear();
      kprintln("Welcome to \"XKBASIC\": Basic, but for the X Kernel (V5)");
      for(;;) {
        read_word(buff, 256, 0, 0);
        xkbasic(buff);
      }
    }
    else system(cmd, count);

    *(int*)(0x10000) = get_cursor();
    int madr = *(int*)0x10000;
    set_cursor(madr);
    if(madr == 3840) clear();
  }
}

void nitrix_kernel(void) {
  gmode();
}

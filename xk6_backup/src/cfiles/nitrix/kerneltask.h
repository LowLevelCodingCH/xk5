/* Defines a couple commands that the SystemExecuteCommand can use */
void echo(char command[16][50], int count) {
    int major = 0;
    for(int i = 1; i < cnt; i++) {
        printf("%s ", command[i]);
        major += ssize(command[i]) + 1;
    }
    kputln(major);
}

void ls(char command[16][50], int count) {
    XK_Prefix(ReadXKFS)();
    XK_Prefix(ListFiles)(FST_Entries);
}

void ps(char command[16][50], int count) {
    XK_Prefix(ListTasks)();
}

void call(char command[16][50], int count) {
    XK_Prefix(CallAddress)(atoi(command[1]));
}

void kill(char command[16][50], int count) {
    XK_Prefix(RemoveTask)(atoi(command[1]));
}

void cat(char command[16][50], int count) {
XK_Prefix(ReadXKFS)();
    int idx = XK_Prefix(GetFileIndex)(FST_Entries, FST.files_amount, command[1]);
    if(idx != -1)
        XK_Prefix(ReadFileByIndexPrint)(FST_Entries, FST_Blocks, idx);
    else kprintln("[io::xkfs] No such file");
}

void rfd(char command[16][50], int count) {
    XK_Prefix(ReadXKFS)();
    int idx = XK_Prefix(GetFileIndex)(FST_Entries, FST.files_amount, command[1]);
    if(idx != -1)
        XK_Prefix(ReadFileData)(FST_Entries, FST_Blocks, idx);
    else kprintln("[io::xkfs] No such file");
}

void cbeep(char command[16][50], int count) {
    beep(atoi(command[1]),atoi(command[2]));
}

void rdram(char command[16][50], int count) {
    kprintbuf((char*)atoi(command[1]), atoi(command[2]));
}

void lsusrs(void) {
    XK_Prefix(ListUsers)(users, (int)UserAmount);
}

/* Executes a shell command with a count and command */
void XK_Prefix(SystemExecuteShellCommand(char command[16][50], int count)) {
    if(streq(command[0],"echo")==0)       echo(command, count);
    if(count == 1) {
        if(streq(command[0],"clear")==0)  clear();
        if(streq(command[0],"ps")==0)     ps(command, count);
        if(streq(command[0],"ls")==0)     ls(command, count);
        if(streq(command[0],"lsusrs")==0) lsusrs();
    }
    if(count == 2) {
        if(streq(command[0],"call")==0)   call(command, count);
        if(streq(command[0],"kill")==0)   kill(command, count);
        if(streq(command[0],"cat")==0)    cat(command, count);   
        if(streq(command[0],"rfd")==0)    rfd(command, count);
    }
    if(count == 3) {
        if(streq(command[0],"cbeep")==0)  cbeep(command, count);
        if(streq(command[0],"rdram")==0)  rdram(command, count);
    }
}

void XK_Prefix(LoadingBarForLookingCool)(int stat) {
    _glob_color = VGA_COLOR_LIGHT_GREY;
    clear();
    for(int i = 0; i < 10; i++)
        kputln(0);
    _glob_color = (VGA_COLOR_LIGHT_CYAN << 8) | '0';
    kprintln("-----------------------------XKOS Initializing----------------------------------");
    _glob_color = VGA_COLOR_WHITE;
    _glob_color = (VGA_COLOR_WHITE << 8) | '1';
    for(int i = 0; i < stat; i++)
        kputc('X');
    for(int i = 0; i < 80 - stat; i++)
        kputc('-');
    _glob_color = (VGA_COLOR_LIGHT_CYAN << 8) | '0';
    kprintln("--------------------------------------------------------------------------------");
    _glob_color = VGA_COLOR_LIGHT_GREY;
}

/* Runs on every tick */
void Nx_Prefix(KernelUpdate)(void) {
    _glob_color = VGA_COLOR_LIGHT_GREY;

    XK_PrintTimeAndDate();

    XK_ReadXKFS();
    
    kprint(user);
    kputc('/');
    kputc(XKFS.XKFS_DiskLabel);
    kputc(':');
    kputc('>');
    kputc(' ');
    read_word(buffer, 50, ssize(user) + ssize("/_:> "), 0);
    
    cmdsplit(buffer, ' ', cmdbuffer, &cnt);
    
    /* Executes the command */

    XK_SystemExecuteShellCommand(cmdbuffer, cnt);
    
    clearBuffer(buffer, 50);
    for(int i = 0; i < 16; i++) clearBuffer(cmdbuffer[i], 50);
    if(cursor >= 3840) {
        clear();
        reset_cursor();
        cursor = 0;
    }
    
    *(uint32_t*)(0x10000) = cursor;
}

/* Initializes everything that the OS needs */
void Nx_Prefix(KernelInit)(void) {

    #ifdef CoolLoadingBar
        for(int i = 0; i < 80; i++) {
            XK_LoadingBarForLookingCool(i);
            wait_ms(CoolLoadingBarDelayFF);
        }
        kprintln("Faster than Arch Linux");
        wait_ms(1100);
        clear();
    #endif

    clear();

    _glob_color = VGA_COLOR_LIGHT_GREY;
    
    //klog_println("kernel: Initializing AtA");


    XK_LoadingBarForLookingCool(0);
    wait_ms(CoolLoadingBarDelay);


    init_ata();
    //klog_println("kernel: Initializing XKFS");


    XK_LoadingBarForLookingCool(8);
    wait_ms(CoolLoadingBarDelay);
    XK_LoadingBarForLookingCool(16);
    wait_ms(CoolLoadingBarDelay);


    XK_InitXKFS();
    //klog_println("kernel: Writing to disk will likely not work!");
    //klog_println("kernel: Initializing PiT");


    XK_LoadingBarForLookingCool(24);
    wait_ms(CoolLoadingBarDelay);
    XK_LoadingBarForLookingCool(32);
    wait_ms(CoolLoadingBarDelay);


    init_pit(12);
    //klog_println("kernel: Initializing FDs");


    XK_LoadingBarForLookingCool(40);
    wait_ms(CoolLoadingBarDelay);
    XK_LoadingBarForLookingCool(48);
    wait_ms(CoolLoadingBarDelay);


    XK_FileDescriptors[0] = OUTMEM;
    XK_FileDescriptors[1] = TTY_Begin;
    //klog_println("kernel: Initializing Users");


    XK_LoadingBarForLookingCool(56);
    wait_ms(CoolLoadingBarDelay);
    XK_LoadingBarForLookingCool(64);
    wait_ms(CoolLoadingBarDelay);


    XK_InitUsers();	
    //klog_println("kernel: Initializing IDT");


    XK_LoadingBarForLookingCool(72);
    wait_ms(CoolLoadingBarDelay);
    XK_LoadingBarForLookingCool(80);
    wait_ms(CoolLoadingBarDelay);


    init_idt();

    clear();

    klog_println("kernel: Testing Sound");
    XK_PlayS(startup_beep);
    
    klog_println("kernel: Testing Handles & Memory Allocation");
    HANDLE h = XK_InitHandle();
    
    XK_AllocatePage(&h, 2);
    
    if(h.pam == 0 || h.adr == NULL) {
        klog_println("kernel: Error with handles");
        asm("hlt");
    }
    
    XK_FreeHandle(&h);
    
    if(h.pam != 0 || h.adr != NULL) {
        klog_println("kernel: Error with handles");
        asm("hlt");
    }
    
    klog_println("kernel: Done initializing!");
    XK_Prefix(ListTasks)();
    XK_Prefix(ListUsers)(users, (int)UserAmount);
}

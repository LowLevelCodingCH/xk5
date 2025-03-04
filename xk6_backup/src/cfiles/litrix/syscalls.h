void XK_Prefix(GetRegs)(regs_t* regs) {
//    __asm__ volatile (
//        "mov %%eax, %0\n\t"
//        "mov %%edx, %1\n\t"
//        "mov %%ebp, %2\n\t"
//        "mov %%ebx, %3\n\t"
//        "mov %%esi, %4\n\t"
//        "mov %%esp, %5\n\t"
//        "mov %%ecx, %6\n\t"
//        "mov %%edi, %7\n\t"
//        : "=r"(regs->eax), "=r"(regs->ebx), "=r"(regs->ecx),
//          "=r"(regs->edx), "=r"(regs->esi), "=r"(regs->edi),
//          "=r"(regs->ebp), "=r"(regs->esp));
}
void XK_Prefix(RunSyscall)(uint32_t index,
                           uint32_t arg0   /* if diskio (READ/WRITE) or read or write */,
                           uint32_t arg1 /* if diskio (READ/WRITE) or read or write */,
                           uint32_t arg2 /* if diskio (READ/WRITE) or read */) {
    switch(index) {
        case SYS_WRITE:      kprintbuf((char*)arg0, arg1);             break;
        case SYS_READ:       read_word((char*)arg0, arg1, arg2, arg1); break;
        case SYS_DISK_READ:  kprintln("Unimplemented");         break;
        case SYS_DISK_WRITE: kprintln("Unimplemented");         break;
    }
}

void XK_Prefix(RunRegSyscall)(regs_t *regs) {
    XK_Prefix(RunSyscall)(regs->eax,
                          regs->ebx,
                          regs->ecx,
                          regs->edx);
}

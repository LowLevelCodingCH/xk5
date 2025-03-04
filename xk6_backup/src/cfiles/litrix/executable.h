//TODO: Implement syscall handling
//TODO: Make it make a Task


// Programs have full access to everything. Execute wisely

void XK_Prefix(CallAddress)(uint32_t address) {
    CALL(address);
}

void XK_Prefix(RunExecutable)(bprgm prg) {
    XK_Prefix(CallAddress)(prg.begin_exec);
}

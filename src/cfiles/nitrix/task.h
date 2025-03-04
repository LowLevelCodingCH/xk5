/* Allocates a page for a handle */
void XK_Prefix(AllocatePage)(HANDLE *handle, WORD amount) {
    handle->pam += amount;
    handle->adr = (&(XK_Prefix(MemoryPool))+
        XK_Prefix(NextFreePage));
    XK_Prefix(NextFreePage) += amount;
}

/* Initializes a handle */
HANDLE XK_Prefix(InitHandle)(void) {
    HANDLE handle;
    handle.pam = 0;
    handle.adr = NULL;
    return handle;
}

/* Frees a handle */
void XK_Prefix(FreeHandle)(HANDLE *handle) {
    handle->pam = 0;
    handle->adr = NULL;
    XK_Prefix(NextFreePage) = 0;
}

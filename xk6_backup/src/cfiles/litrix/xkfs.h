/* Gets the file index via name */
int XK_Prefix(GetFileIndex)(FST_Entry entries[256], int amount, char filename[16]) {
    for(int i = 0; i < amount; i++)
        if(streq(entries[i].name, filename) == 0) return i;
    return -1;
}

/* Reads a file by the index, then prints it to stdout */
void XK_Prefix(ReadFileByIndexPrint)(FST_Entry entries[256], byte_t blocks[256][512], FILE i) {
    for(int j = 0; j < 16; j++) {
        if(entries[i].blocks[j] != 0){
            for(int k = 0; k < entries[i].size[j]; k++) {
                int blockid = (unsigned int)entries[i].blocks[j]-1;

                kputc(blocks[blockid][k]);
            }
        }
    }
}

/* Reads a file by its index and puts it somewhere (char*) */
void XK_Prefix(ReadFileByIndex)(FST_Entry entries[256], byte_t blocks[256][512], FILE i, char *where) {
    for(int j = 0; j < 16; j++) {
        if(entries[i].blocks[j] != 0){
            for(int k = 0; k < entries[i].size[j]; k++) {
                int blockid = (unsigned int)entries[i].blocks[j]-1;

                where[k] = blocks[blockid][k]; // kputc. no where flag.
            }
        }
    }
}

/* Reads and prints the file data */
void XK_Prefix(ReadFileData)(FST_Entry entries[256], byte_t blocks[256][512], FILE fd) {
    FST_Entry *entry = &entries[fd];
    kprintln("Name");
    kprintbuf(entry->name, 16);
    kputln(16);

    kprintln("Blocks");
    for(int i = 0; i < 16; i++) {
        kprint(itoa((unsigned int)entry->blocks[i]));
        kputc(',');
    }

    kputln(16*2);

    kprintln("Block Sizes");
    for(int i = 0; i < 16; i++) {
        kprint(itoa((unsigned int)entry->size[i]));
        kputc(',');
    }

    kputln(16*2);

    kprintln("Entry Number");
    kprintln(itoa(fd));
}

/* Reads a file by its name into a char* */
void XK_Prefix(ReadFile)(FST_Entry entries[256], byte_t blocks[256][512], char filename[16], char *where) {
    int ind0 = XK_Prefix(GetFileIndex)(entries, 256, filename);
    if(ind0 != -1)
        XK_Prefix(ReadFileByIndex)(entries, blocks, ind0, where);
}

/* Lists the files into stdout */
void XK_Prefix(ListFiles)(FST_Entry entries[256]) {
    for(int i = 0; i < 256; i++)
        if(entries[i].name[0] != 0)
            kprintln(entries[i].name);
}

/* Reads the XK File System and logs into stdout*/
void XK_Prefix(InitXKFS)() {
    klog_println("xkfs: Reading XKFS-Header");
    ata_read_sector(BASE_END_ALL, FileSysHeader);
    klog_println("xkfs: Reading File Segment Table (FST)");
    ata_read_sector(BASE_END_ALL+1, FileSegmentTableHeader);
    ata_read(FileSegmentTableEntries, BASE_END_ALL+2, 8);
    for(int i = 0; i < 256; i++) ata_read(FST_Blocks[i], BASE_END_ALL+10+i, 1);

    klog_println("xkfs: Assigning XKFS-Header");
    XKFS.XKFS_Label[0] = FileSysHeader[0];
    XKFS.XKFS_Label[1] = FileSysHeader[1];
    XKFS.XKFS_Label[2] = FileSysHeader[2];
    XKFS.XKFS_Label[3] = FileSysHeader[3];
    XKFS.XKFS_Version[0] = FileSysHeader[4];
    XKFS.XKFS_Version[1] = FileSysHeader[5];
    XKFS.XKFS_DiskLabel = FileSysHeader[6];
    XKFS.XKFS_MaxNameSize = FileSysHeader[7];
    XKFS.XKFS_BlockSize = 512;
    XKFS.XKFS_MaxFileSizeBlocks = FileSysHeader[10];
    FST.files_amount = FileSegmentTableHeader[0];

    klog_println("xkfs: Verifying XKFS-Header");

    if(XKFS.XKFS_Label[0] != 'X' || XKFS.XKFS_Label[1] != 'K' || XKFS.XKFS_Label[2] != 'F' || XKFS.XKFS_Label[3] != 'S') {
        clear();
        klog_println("xkfs: FAILED TO VERIFY XKFS-Header.");
        klog_print("xkfs: Got: ");
        kputc(XKFS.XKFS_Label[0]);
        kputc(XKFS.XKFS_Label[1]);
        kputc(XKFS.XKFS_Label[2]);
        kputc(XKFS.XKFS_Label[3]);
        kprint(", instead of XKFS");
        asm("hlt");
        for(;;){;};
    } 

    klog_println("xkfs: Assigning File Segment Table (FST)");
    for(int i = 0; i < (unsigned int)FST.files_amount; i++) {
        for(int j = 0; j < 16; j++)
            FST_Entries[i].blocks[j] = FileSegmentTableEntries[(i*(16+16+16))+j];
        for(int j = 0; j < 16; j++)
            FST_Entries[i].size[j] = FileSegmentTableEntries[(i*(16+16+16))+j+16];
        for(int j = 0; j < 16; j++)
            FST_Entries[i].name[j] = FileSegmentTableEntries[(i*(16+16+16))+j+16+16];
    }
    klog_print("xkfs: ");
    kprint(itoa((int)FST.files_amount));
    kprint(" files");
    kputln(17+12+ssize(itoa((int)FST.files_amount)));
}

/* Reads the XK File System and doesn't log into stdout*/
void XK_Prefix(ReadXKFS)() {
    ata_read_sector(BASE_END_ALL, FileSysHeader);
    ata_read_sector(BASE_END_ALL+1, FileSegmentTableHeader);
    ata_read(FileSegmentTableEntries, BASE_END_ALL+2, 8);
    for(int i = 0; i < 256; i++) ata_read(FST_Blocks[i], BASE_END_ALL+10+i, 1);

    XKFS.XKFS_Label[0] = FileSysHeader[0];
    XKFS.XKFS_Label[1] = FileSysHeader[1];
    XKFS.XKFS_Label[2] = FileSysHeader[2];
    XKFS.XKFS_Label[3] = FileSysHeader[3];
    XKFS.XKFS_Version[0] = FileSysHeader[4];
    XKFS.XKFS_Version[1] = FileSysHeader[5];
    XKFS.XKFS_DiskLabel = FileSysHeader[6];
    XKFS.XKFS_MaxNameSize = FileSysHeader[7];
    XKFS.XKFS_BlockSize = 512;
    XKFS.XKFS_MaxFileSizeBlocks = FileSysHeader[10];
    FST.files_amount = FileSegmentTableHeader[0];

    for(int i = 0; i < (unsigned int)FST.files_amount; i++) {
        for(int j = 0; j < 16; j++)
            FST_Entries[i].blocks[j] = FileSegmentTableEntries[(i*(16+16+16))+j];
        for(int j = 0; j < 16; j++)
            FST_Entries[i].size[j] = FileSegmentTableEntries[(i*(16+16+16))+j+16];
        for(int j = 0; j < 16; j++)
            FST_Entries[i].name[j] = FileSegmentTableEntries[(i*(16+16+16))+j+16+16];
    }
}

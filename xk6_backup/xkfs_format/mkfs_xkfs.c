#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BLOCKLEN  512
#define BLOCKAM   256
#define ENTRIESSZ 8

#define entiresize ((BLOCKLEN*2)+(ENTRIESSZ*BLOCKLEN)+(BLOCKAM*BLOCKLEN))

int main(int argc, char ** argv) {
	//char xkfs_header[BLOCKLEN]           = {0};
	//char fst_header[BLOCKLEN]            = {0};
	//char fst_entries[ENTRIESSZ*BLOCKLEN] = {0};
	//char xkfs_blocks[BLOCKAM*BLOCKLEN]   = {0};

	char xkfs[entiresize] = {0};

	if(argc != 3) fprintf(stderr, "Usage: mkfs.xkfs <FILE TO FORMAT> <DISK LABEL>\n");

	char disk_label = argv[2][0];

	xkfs[0] = 'X';
	xkfs[1] = 'K';
	xkfs[2] = 'F';
	xkfs[3] = 'S';

	xkfs[4] = 0x06; // 6
	xkfs[5] = 0x0A; // 10 / X
				   // XK6

	xkfs[6] = disk_label;

	xkfs[7] = 0x10; // Max file name size

	xkfs[8] = 0;    // Let it be this. even tho it is the block size it gets ignored
	xkfs[9] = 2;    // Let it be this. even tho it is the block size it gets ignored

	xkfs[10] = 8;   // Max file block amount

	xkfs[12] = ' '; // Idk
	xkfs[13] = '.'; // Idk

	xkfs[511] = 0x55;
	xkfs[512] = 0xaa;

	// all else is 0s. you can change stuff however. just get the partitions right

	int fd = open(argv[1], O_RDWR);

	write(fd, xkfs, entiresize);

	close(fd);
}

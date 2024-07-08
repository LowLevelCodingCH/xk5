#include <stddev.h>
// I/O port addresses for ATA primary bus
#define ATA_PRIMARY_IO      0x1F0
#define ATA_PRIMARY_CTRL    0x3F6

// ATA registers
#define ATA_REG_DATA        0x00
#define ATA_REG_ERROR       0x01
#define ATA_REG_SECCOUNT0   0x02
#define ATA_REG_LBA0        0x03
#define ATA_REG_LBA1        0x04
#define ATA_REG_LBA2        0x05
#define ATA_REG_HDDEVSEL    0x06
#define ATA_REG_COMMAND     0x07
#define ATA_REG_STATUS      0x07
#define ATA_REG_ALTSTATUS   0x06

// ATA commands
#define ATA_CMD_READ_PIO    0x20

// Status flags
#define ATA_SR_BSY          0x80
#define ATA_SR_DRDY         0x40
#define ATA_SR_DRQ          0x08
#define ATA_SR_ERR          0x01

#define ATA_PRIMARY_IO_BASE      0x1F0
#define ATA_PRIMARY_CONTROL_BASE 0x3F6
#define ATA_DRIVE_MASTER         0xA0
#define ATA_DRIVE_SLAVE          0xB0
// I/O port addresses for ATA primary bus
#define ATA_PRIMARY_IO      0x1F0
#define ATA_PRIMARY_CTRL    0x3F6

// I/O port addresses for ATA secondary bus (if needed)
// #define ATA_SECONDARY_IO    0x170
// #define ATA_SECONDARY_CTRL  0x376

// ATA registers
#define ATA_REG_DATA        0x00
#define ATA_REG_ERROR       0x01
#define ATA_REG_SECCOUNT0   0x02
#define ATA_REG_LBA0        0x03
#define ATA_REG_LBA1        0x04
#define ATA_REG_LBA2        0x05
#define ATA_REG_HDDEVSEL    0x06
#define ATA_REG_COMMAND     0x07
#define ATA_REG_STATUS      0x07
#define ATA_REG_ALTSTATUS   0x06
#define ATA_REG_CONTROL     0x02

// ATA commands
#define ATA_CMD_READ_PIO    0x20
#define ATA_CMD_IDENTIFY    0xEC

// Status flags
#define ATA_SR_BSY          0x80
#define ATA_SR_DRDY         0x40
#define ATA_SR_DRQ          0x08
#define ATA_SR_ERR          0x01

#define ATA_REG_DATA             0x00
#define ATA_REG_ERROR            0x01
#define ATA_REG_FEATURES         0x01
#define ATA_REG_SECCOUNT0        0x02
#define ATA_REG_LBA0             0x03
#define ATA_REG_LBA1             0x04
#define ATA_REG_LBA2             0x05
#define ATA_REG_HDDEVSEL         0x06
#define ATA_REG_COMMAND          0x07
#define ATA_REG_STATUS           0x07
#define ATA_REG_ALTSTATUS        0x0C
#define ATA_REG_CONTROL          0x0C

#define ATA_CMD_READ_PIO         0x20
#define ATA_SR_BSY               0x80
#define ATA_SR_DRDY              0x40
#define ATA_SR_DRQ               0x08

void ata_select_drive(uint8_t drive) {
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0 | (drive << 4));
    ata_wait_bsy();
}

void identify_ata(uint8_t drive) {
	outb(0x1F6, drive);
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);
	outb(0x1F7, 0xEC);
	uint8_t tmp = inb(0x1F7);
	if (tmp & STATUS_RDY) {
		switch (drive) {
			case 0xA0:
				kreadmeln("Master Drive is the current active Drive\n");
				break;
			case 0xB0:
				kreadmeln("Slave Drive is the current active Drive\n");
				break;
		}
	}
	else
	{
		switch (drive) {
			case 0xA0:
				kreadmeln("Master Drive is NOT the current active Drive\n");
				break;
			case 0xB0:
				kreadmeln("Slave Drive is NOT the current active Drive\n");
				break;
			}
	}
}

void wait_BSY() { while(inb(0x1F7) & STATUS_BSY); }
void wait_DRQ() { while(!(inb(0x1F7) & STATUS_RDY)); }
void insw(uint16_t port, void* addr, uint32_t count) {
    __asm__ volatile ("rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}

void ata_wait_bsy() {
    while (inb(ATA_PRIMARY_IO_BASE + ATA_REG_STATUS) & ATA_SR_BSY);
}

void ata_wait_drq() {
    while (!(inb(ATA_PRIMARY_IO_BASE + ATA_REG_STATUS) & ATA_SR_DRQ));
}

void ata_read_sector(uint32_t lba, uint8_t* buffer) {
    outb(ATA_PRIMARY_CONTROL_BASE, 0x02); // Disable IRQs

    ata_wait_bsy();
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_HDDEVSEL, ATA_DRIVE_MASTER | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_SECCOUNT0, 1); // Number of sectors to read
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ata_wait_bsy();
    ata_wait_drq();

    insw(ATA_PRIMARY_IO_BASE + ATA_REG_DATA, buffer, 256); // Read 256 words (512 bytes)

    outb(ATA_PRIMARY_CONTROL_BASE, 0x00); // Enable IRQs
}

static inline void outw(uint16_t port, uint16_t value) {
	asm("outw %0, %1" :: "a"(value), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
	uint16_t value;
	asm("inw %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline void outl(uint16_t port, uint32_t value) {
	asm("outl %%eax, %%dx" :: "d" (port), "a" (value));
}

static inline uint32_t inl(uint16_t port) {
	uint32_t ret;
	asm("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}


typedef BYTE	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */


//DSTATUS disk_initialize (BYTE pdrv);
//DSTATUS disk_status (BYTE pdrv);
//DRESULT disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);
//DRESULT disk_write (BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count);
//DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);


/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC			0	/* Complete pending write process (needed at FF_FS_READONLY == 0) */
#define GET_SECTOR_COUNT	1	/* Get media size (needed at FF_USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at FF_MAX_SS != FF_MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at FF_USE_MKFS == 1) */
#define CTRL_TRIM			4	/* Inform device that the data on the block of sectors is no longer used (needed at FF_USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */
#define CTRL_FORMAT			8	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */
#define ISDIO_READ			55	/* Read data form SD iSDIO register */
#define ISDIO_WRITE			56	/* Write data to SD iSDIO register */
#define ISDIO_MRITE			57	/* Masked write data to SD iSDIO register */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20	/* Get F/W revision */
#define ATA_GET_MODEL		21	/* Get model name */
#define ATA_GET_SN			22	/* Get serial number */

#define NUM_HEADS 16    // Assuming 16 heads
#define NUM_SECTORS 63  // Assuming 63 sectors per track

uint32_t chs2lba(int c, int h, int s) {
    // Validation of input parameters
    if (c < 0 || h < 0 || s <= 0 || h >= NUM_HEADS || s > NUM_SECTORS) {
        kpanicln("Invalid CHS values", 1);
        return 1;
    }

    // CHS to LBA conversion formula
    return (uint32_t)((c * NUM_HEADS + h) * NUM_SECTORS + (s - 1));
}

#define DEV_ATA    0   /* Example: Map ATA to physical drive 0 */

/* ATA controller ports and commands */
#define ATA_DATA        0x1F0
#define ATA_ERROR       0x1F1
#define ATA_SECCOUNT    0x1F2
#define ATA_SECTOR      0x1F3
#define ATA_CYL_LO      0x1F4
#define ATA_CYL_HI      0x1F5
#define ATA_DRIVE       0x1F6
#define ATA_COMMAND     0x1F7
#define ATA_STATUS      0x1F7

#define ATA_CMD_READ    0x20
#define ATA_CMD_WRITE   0x30
#define ATA_CMD_IDENT   0xEC

#define ATA_SR_BSY      0x80
#define ATA_SR_DRDY     0x40
#define ATA_SR_DF       0x20
#define ATA_SR_DSC      0x10
#define ATA_SR_DRQ      0x08
#define ATA_SR_CORR     0x04
#define ATA_SR_IDX      0x02
#define ATA_SR_ERR      0x01

static void ATA_wait(void) {
    while (inb(ATA_STATUS) & ATA_SR_BSY);
}

//typedef enum {
//	RES_OK = 0,		/* 0: Successful */
//	RES_ERROR,		/* 1: R/W Error */
//	RES_WRPRT,		/* 2: Write Protected */
//	RES_NOTRDY,		/* 3: Not Ready */
//	RES_PARERR		/* 4: Invalid Parameter */
//} DRESULT;

int disk_write (
    BYTE pdrv,
    const BYTE *buff,
    LBA_t sector,
    UINT count
)
{
    DRESULT res = RES_OK;

    if (pdrv != DEV_ATA) {
        return RES_PARERR;
    }

    while (count--) {
        outb(ATA_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
        outb(ATA_SECCOUNT, 1);
        outb(ATA_SECTOR, sector & 0xFF);
        outb(ATA_CYL_LO, (sector >> 8) & 0xFF);
        outb(ATA_CYL_HI, (sector >> 16) & 0xFF);
        outb(ATA_COMMAND, ATA_CMD_WRITE);

        ATA_wait();

        if (inb(ATA_STATUS) & ATA_SR_ERR) {
            return RES_ERROR;
        }

        for (int i = 0; i < 256; i++) {
            outw(ATA_DATA, ((unsigned short*)buff)[i]);
        }
        buff += 512;
        sector++;
    }

    return (int)res;
}

int ATA_disk_initialize(void) {
    // Initialize ATA drive
    outb(ATA_DRIVE, 0xA0);  // Select master drive
    outb(ATA_SECCOUNT, 0);
    outb(ATA_SECTOR, 0);
    outb(ATA_CYL_LO, 0);
    outb(ATA_CYL_HI, 0);
    outb(ATA_COMMAND, ATA_CMD_IDENT);

    ATA_wait();

    if (inb(ATA_STATUS) == 0) {
        return (int)STA_NOINIT;
    }

    // Drive initialized successfully
    return (int)0;
}

int disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res = RES_OK;

    if (pdrv != DEV_ATA) {
        return RES_PARERR;
    }

    while (count--) {
        // Send READ SECTORS command to ATA drive
        outb(ATA_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
        outb(ATA_SECCOUNT, 1);
        outb(ATA_SECTOR, sector & 0xFF);
        outb(ATA_CYL_LO, (sector >> 8) & 0xFF);
        outb(ATA_CYL_HI, (sector >> 16) & 0xFF);
        outb(ATA_COMMAND, ATA_CMD_READ);

        ATA_wait();

        if (inb(ATA_STATUS) & ATA_SR_ERR) {
            return RES_ERROR;
        }

        // Read data from ATA_DATA port
        for (int i = 0; i < 256; i++) {
            ((unsigned short*)buff)[i] = inw(ATA_DATA);
        }
        buff += 512;
        sector++;
    }

    return (int)res;
}

int ATA_disk_write(const BYTE* buff, LBA_t sector, UINT count) {
    return (int)disk_write(DEV_ATA, buff, sector, count);
}

void ata_chs_read(uint16_t cylinder, uint8_t head, uint8_t sector, uint8_t sector_count, uint16_t* buffer) {
    // Select master drive (0) or slave drive (1)
    ata_select_drive(0); // 0 for master, 1 for slave

    // Send CHS address
    outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, sector_count);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA0, sector); // Sector number (1-255)
    outb(ATA_PRIMARY_IO + ATA_REG_LBA1, cylinder & 0xFF); // Cylinder low byte
    outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (cylinder >> 8) & 0xFF); // Cylinder high byte
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0 | (head & 0x0F)); // Head number

    // Send the read command
    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    // Read data
    for (int i = 0; i < sector_count; i++) {
        ata_wait_bsy();
        ata_wait_drq();

        // Read a sector (256 words)
        for (int j = 0; j < 256; j++) {
            buffer[i * 256 + j] = inw(ATA_PRIMARY_IO + ATA_REG_DATA);
        }
    }
}

# X Kernel V6
## The sixth version of my 10th attempt at OsDev.
#### A(nother) LLCCH-Project
#### I use arch btw
### Versions:
```
NitrixBootLD (1)  (Bootsector only)      // Availible on github. Not same name
NitrixGBoot  (2)  (Bootsector only)      // Game, like snake. Was availible on github, but i deleted it.
NXKRNLLD     (3)  (Bootsector & "Kernel")
Kerneli      (4)  ("Kernel")
NxKernV      (5)  ("Kernel")
NxOS         (6)  (Bootsector & "Kernel")
SQOS         (7)  (Bootsector & "Kernel") // Availible on github
ak48         (8)  (Bootsector & Kernel)   // a = 1. k = kernel. 4 = for. 8 = V8
NXV9         (9)  (Kernel)
NitxX0       (10) (Bootsector & "Kernel")
MergeNITX    (11) (Bootsector & Kernel)
xk2          (12) (Bootsector & "Kernel")
xk3          (13) (Bootsector & Kernel)
xk4          (14) (Bootsector & "Kernel")
xk5          (15) (Bootsector & Kernel & Simple WM)   // Availible on github
xk6          (16) (Work in progress) (Bootsector (GrUB) & Kernel & Shell (TTY) & File System & (Likely) a Simple WM)
```

### Q&A:
You may ask: `But why did you stop on all these, especially xk5?`
My answer: `I moved to nixos and the WM didn't work, I removed it. I wanted the TTY black and it didn't work. This is a fresh start.`
UPDATE: `I moved back to Linux Mint and will likely install Arch. Arch's qemu supports all that so I'll add another WM. But XK5's code was horrible anyways.` (2 weeks later)
UPDATE: `I did install Arch. I USE ARCH BTW. But osdev will stay at linux mint` (1 week later)
UPDATE: `Yay it works on arch (or should I say, paru it works on arch (ok this was horrible)). Anyways I'll stay on arch!` (1 week later)

You may ask: `Will you stay with XK(V)?`
My answer: `Yes I likely will. The X doesn't mean nitriX. But 10 in the roman numerals. Once I get to v20, I'll do Nx(V). Just for fun.`

You may ask: `How old are you?`
My answer: `I don't have to tell you. But this is my answer: 0b000001100.`

You may ask: `Will you stay with GrUB?`
My answer: `No, as long as my kernel is within the weird 52 sector limit (size-wise), I won't use GrUB.`
UPDATE: `I use GrUB now.` (1 week later)

You may ask: `You are (0b000001100) years old and you have so much free time?`
My answer: `No, I code until 1 am CET.`

### Dependencies
#### I will try to make this work with 64-BIT so you don't have to install all this, though I think it already is.
For Debian users, get your Apateu (APT) to install "grub" and "gcc"
For us Arch users aswell, install "grub gcc mtools" (mtools is important for grub)"
For NixOS users, add this to `/etc/nixos/configuration.nix`:
(Sound wont work because of qemu. Maybe updating or something works)
```
gcc
qemu      # If you wanna execute the ISO immediatly. Can be run by others like VirtualBox or VMWare
bash
nasm
grub2
xorriso
clang
```

### Compilation and Modification
To compile Nitrix XK6 you have to simply run `sh main.sh`
To run it in QEMU, run `sh qemu.sh`
To modify, here's a documentation on directories:

C-ROOT:    ./src/cfiles/
ASM-ROOT:  ./src/asmfiles/

Drivers:                             jitrix/
Variables and #defines:              nitrix/
Syscalls, Executables and Emulation: litrix/

Boot: asm

### Mario
MAsteR Input Output
(old name: Port IO)
Please don't sue me, Nintendo

### Users
#### There is a XK$USERS file which contains the users in this format:
USERNAME (16 char)
PASSWORD (Plain text, 16 char)

There are 2 default users:
	|_ Root
	|	|_ Rootuser
	|_ Guest
		|_ (No password)

### Tools
Nitrix provides tools such as:
	- mkfs.xkfs
	- xkfs
	- xkernelv6

### Mkfs.XKFS
Usage is: `mkfs.xkfs <FILE NAME> <DRIVE LETTER/LABEL>`

### XKFS
The X-Kernel-File-System, aka XKFS, is a file system that consists out of this structure:
(FST = File Segment Table)

It begins at LBA 26484 (my linux mint) or 60816 (my arch linux), change in nitrix/std.h `(BASE_END_ALL)` accordingly.
To find the actual value, comment out the "xkfs" lines in the main.sh file, then run it.
Then do "ls -l Binaries/Nx/xos.iso"
Then divide the size by 512, then you have the LBA.

XKFS-Header
	|_ FST-Header
	|_ FST-Entries
	|  |_ ...
	|_ Blocks
	   |_ ...

The XKFS-Header consists of:
"XKFS" because it, in fact, is XKFS, and we wanna verify it.
Version: (xk6): 0x06, 0x0a; (xk5): 0x05, 0x0a; (SQOS): 0xFF, 0x00;
Disk Label: (Usually): 0x41 ('A')
Max file name size: (Standard): 0x10 (CANNOT BE CHANGED. IS ALWAYS THE SAME IN KERNEL)
Ignored: 0x00
Ignored: 0x02
Max file block amount: (Standard) 0x08 (CANNOT BE CHANGED. IS ALWAYS THE SAME IN KERNEL)

#### The FST-Header consists of:
Amount of files

#### The FST-Entries consist of:
(One entry)
Block addresses (16 addresses. start at 1. 0 to make a block not be allocated).
	|_ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		// for block 1 

Sizes of each block (Again, 0 to ignore and make the size 0, 16 Sizes)
	|_ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

The name of the file (16 chars)
	|_ "hi.txt", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

Not Yet Implemented (Stops weirdly at READING FST): If the file is a directory or not (1 char)
	|_ 0
		true (1). false (0).

#### The Blocks consist of:
The data. Each block is 512 bytes long.

"Hello", 0, 0, ...

#### The max amount of blocks is 256
#### As you can see, this filesystem is very easy to use. Without directories for now
#### The max usable amount of chars in a block in a file is 255
#### Max file length is (Amount of usable blocks in a file * Max length per block) `(16*255)` = ~4096 chars
####                                                                                             |_ ~4kb

### X Kernel V6
It is a Kernel launched by GrUB. If you get it to work in another way, cool!
There is "multitasking" (An init function being called once on a task. the update one being called every time the main loop updates (once per <ENTER> press))
The kernel is a Task with TID (Task IDentifier) 0. Name is SystemX
	|_ Its init is Nx_Prefix(KernelInit)
	|_ Its update is Nx_Prefix(KernelUpdate)
It writes to the output (0xb8000)
It handles ATA I/O
It is NOT UNIX. NOT EVEN SIMMILAR
It has built-in commands such as `"call <ADDRESS>" "rfd <FILE>" "cat <FILE>" "ls" "kill <TID>" "cbeep <FREQ> <DUR>" "ps" "clear" "echo <ARGS..>" "rdram <FROM> <TO>" "lsusrs"`
It uses a custom init system named SystemX

### SystemX
Simple init system for XK
Runs once on init
Runs on every tick afterwards

### XKLogin
Simple login manager (cfiles/litrix/users.h)
FNAME: `XK_TryLogin`

### Notes
Please use `backup.sh` regularly to back it up.
Use qemu for testing
DO NOT break anything.
I will test every pull request 10 times.

### Coding Style
4 spaces per indent.
Inline braces.
As I say (I am the new Göthe (if you don't know who that is, don't even try to contribute (jk))) "Spaces before braces"
If some statement fits on the if, do it like so:
if short
``
    if(COND) STATEMENT;
``
if long
``
    if(COND)
        STATEMENT;
``

elsewise, do so:
``
    if(COND) {
        STATEMENT;
        STATEMENT;
        STATEMENT;
        STATEMENT;
    }
``

Do NOT nest to deep.
No coding style applies to Drivers (jitrix). Do whatever there.

### CoolLoadingBar
nitrix/std.h
CoolLoadingBar        // The togglable bar
CoolLoadingBarDelay   // The one that always is there (for beauty)
CoolLoadingBarDelayFF // The togglable bar
If you want a cool loading bar

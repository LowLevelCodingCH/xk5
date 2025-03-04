export PATH=$PATH:/usr/local/i386elfgcc/bin

mkdir isodir

mkdir Binaries
mkdir Binaries/Nx

mkdir Nx
mkdir Nx/Kernel
mkdir Nx/Boot

echo loader
gcc -ffreestanding -m32 -g -c src/asmfiles/boot.s -o Binaries/boot.o
nasm -f elf32 "src/asmfiles/interrupts.s" -o "Binaries/interrupts.o"

echo filesys
nasm -fbin -o Binaries/xkfs_header.bin src/asmfiles/xkfs_header.s
nasm -fbin -o Binaries/xkfs_fst_header.bin src/asmfiles/xkfs_fst_header.s
nasm -fbin -o Binaries/xkfs.bin src/asmfiles/xkfs.s
nasm -fbin -o Binaries/xkfs_blocks.bin src/asmfiles/xkfs_blocks.s

echo kernel
gcc -ffreestanding -m32 -g -c "src/cfiles/NxKernel.c" -o "Binaries/NxKernel.o" -Isrc/cfiles

echo link
ld -melf_i386 -T "src/linker.ld" -o "Binaries/Nx/OS.o" Binaries/interrupts.o "Binaries/boot.o" "Binaries/NxKernel.o"

echo dd
dd if=/dev/zero of=Binaries/Nx/OS.img bs=512 count=2880
dd if=Binaries/Nx/OS.o of=Binaries/Nx/OS.img bs=512 count=2880 conv=notrunc

echo ifgf
if grub-file --is-x86-multiboot Binaries/Nx/OS.o; then
	echo mkdir
	mkdir -p isodir/boot/grub
	
	echo cp
	cp Binaries/Nx/OS.img isodir/boot/xos.elf32
	cp grub.cfg isodir/boot/grub/grub.cfg

	echo grubmkres
	grub-mkrescue -o Binaries/Nx/xos.iso isodir
	cat Binaries/Nx/xos.iso Binaries/xkfs_header.bin Binaries/xkfs_fst_header.bin Binaries/xkfs.bin Binaries/xkfs_blocks.bin > Binaries/Nx/xos.isox
	#cat Binaries/Nx/xos.iso xkfs_format/format > Binaries/Nx/xos.isox
	rm Binaries/Nx/xos.iso
	mv Binaries/Nx/xos.isox Binaries/Nx/xos.iso
fi

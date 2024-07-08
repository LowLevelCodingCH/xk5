##
# Nitrix
#
# @LLCCH
# @version 0.1

#!/bin/bash
export PATH=$PATH:/usr/local/i386elfgcc/bin

# Find all .c and .h files and count lines
find . \( -name '*.c' -or -name '*.h' \) -print0 | xargs -0 wc -l

# Compile assembly and C source files
nasm -fbin src/FileSystem/system.asm -o bin/filesys.bin

i386-elf-gcc -c -ffreestanding -m32 -g "src/bootldr/bootgrub.s" -o "bin/boot.o" -Isrc
i386-elf-gcc -c -ffreestanding -m32 -g "src/Kernel/kernel.c" -o "bin/krnl32.o" -Isrc
i386-elf-gcc -c -ffreestanding -m32 -g "src/Kerncall/Kerncall.c" -o "bin/Kerncall.o" -Isrc
i386-elf-gcc -c -ffreestanding -m32 -g "src/Drivers/drivers.c" -o "bin/drivers.o" -Isrc

# Link the object files to create the binary
ld -m elf_i386 -T conf/linker.ld -o bin/xk5.o -nostdlib "bin/boot.o" "bin/krnl32.o" "bin/Kerncall.o" "bin/drivers.o"

nasm -felf src/Kernel/entry.s -o bin/entry.o

i386-elf-ld -o bin/xkos5.bin -nostdlib -Ttext 0x1000 "bin/entry.o" "bin/krnl32.o" "bin/Kerncall.o" "bin/drivers.o" --oformat binary

cat bin/xkos5.bin bin/filesys.bin > bin/xkos.bin

rm bin/xkos5.bin

dd if=/dev/zero of=bin/xos.img bs=512 count=2880
dd if=bin/xk5.o of=bin/xos.img bs=512 count=2880 conv=notrunc

if grub-file --is-x86-multiboot bin/xk5.o; then
    echo "Multiboot confirmed"

    mkdir -p isodir/boot/grub

    cp bin/xos.img isodir/boot/xos.img
    cp src/LegacyNitrix isodir/boot/LegacyNitrix
    cp conf/grub.cfg isodir/boot/grub/grub.cfg

    grub-mkrescue -o bin/xos5.iso isodir

    cat bin/xos5.iso bin/filesys.bin > bin/xkos.iso

    rm bin/xos.img
    rm bin/xos5.iso
    rm bin/xk5.o

    qemu-system-x86_64 -soundhw pcspk bin/xkos.iso
else
    echo "The file is not multiboot"
fi
# end

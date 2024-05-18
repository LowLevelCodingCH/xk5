##
# Nitrix
#
# @LLCCH
# @version 0.1

echo "EX /usr/local/i386elfgcc/bin"
export PATH=$PATH:/usr/local/i386elfgcc/bin

echo "AS src/Kernel/entry.s"
nasm -felf -o entry.o src/Kernel/entry.s

echo "AS src/Boot/boot.s"
nasm -fbin -o boot.bin src/Boot/boot.s

echo "AS src/Padding/padding.s"
nasm -fbin -o padding.bin src/Padding/padding.s

echo "CC src/Kernel/kernel.c"
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c -o kernel.o src/Kernel/kernel.c -Isrc
echo "CC src/Drivers/drivers.c"
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c -o drivers.o src/Drivers/drivers.c -Isrc

echo "LD entry.o kernel.o drivers.o"
/usr/local/i386elfgcc/bin/i386-elf-ld -o kernel.bin -nostdlib -Ttext 0x1000 entry.o kernel.o drivers.o --oformat binary

echo "CT boot.bin kernel.bin padding.bin"
cat boot.bin kernel.bin padding.bin > image.bin

echo "DD image"
dd if=/dev/zero of=image.img bs=512 count=2880

echo "DD boot kernel"
dd if=image.bin of=image.img bs=512 count=2880 conv=notrunc

# end

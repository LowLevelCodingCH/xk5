##
# XKOS5
#
# @file
# @version 0.1

build:
	sh build.sh

clean:
	rm -rf isodir/boot/grub/grub.cfg isodir/boot/xos.img isodir/boot/Xos.bin isodir/boot/Xos.binr isodir/boot/LegacyNitrix isodir/boot/Xos.img
	rm -rf bin/*

# end

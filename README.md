# How to install
install MacPorts for mac, then run `sudo port install i386-elf-gcc`, then modify the commands in build.sh to match `i386-elf-XXX`, not `/usr/bin/i386-elf-XXX`
For linux: `https://github.com/mell-o-tron/MellOs/blob/main/A_Setup/setup-gcc-debian.sh`, RIP, but it takes ~20 mins on a bad pc
then run `sh build.sh` and `qemu-system-x86_64 image.img`
# Setting the user
to set the user, type: `user /set <name>`, dont add spaces ANYWHERE, otherwise it WILL break

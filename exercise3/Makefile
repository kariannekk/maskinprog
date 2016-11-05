#
# Simple development makefile
#


setup:
	ptxdist select configs/ptxconfig
	ptxdist platform configs/platform-energymicro-efm32gg-dk3750/platformconfig
	ptxdist toolchain /opt/ex3/OSELAS.Toolchain-2012.12.0/arm-cortexm3-uclinuxeabi/gcc-4.7.2-uclibc-0.9.33.2-binutils-2.22-kernel-3.6-sanitized/bin

all:
	ptxdist images
	ptxdist test flash-all

game:
	ptxdist clean game
	ptxdist compile game
	ptxdist targetinstall game
	ptxdist image root.romfs
	ptxdist test flash-rootfs

driver:
	ptxdist clean driver-gamepad
	ptxdist compile driver-gamepad
	ptxdist targetinstall driver-gamepad
	ptxdist targetinstall kernel
	ptxdist image root.romfs
	ptxdist test flash-rootfs

link:
	@echo "\n--- Useful within link: 'game' and 'modprobe driver-gamepad'. ---"
	miniterm.py -b 115200 -p /dev/ttyUSB0

.PHONY: setup all game driver link

# end of development makefile

#
# Simple development makefile
#


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
	miniterm.py -b 115200 -p /dev/ttyUSB0

.PHONY: game link

# end of development makefile

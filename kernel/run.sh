#!/bin/bash

################################################################################
# File: run.sh
#       Once the System module is compiled and the swap.img file containing the
#       I/O and User modules is ready, the system can be executed using this
#       bash script. At the very beginning, the processor will start up in
#       16-bit not-protected mode (the so called 'real mode') and must move, via
#       software, to 32-bit protected mode. This is usually done by the
#       bootstrap. As far as it concerns the QEMU Linux Kernel this is done by
#       the emulator itself. We will have to handle by ourself how to move to
#       the 64-bit mode: the 64-bit mode really is just a sub-mode of the memory
#       paging and the only thing we have to do is to implement virtual memory
#       for our system.
#
# Usage: ./run
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 31/08/2019
################################################################################

################################################################################
#
################################################################################
[ -f conf/conf.sh ] && source conf/conf.sh

################################################################################
# Locates QEMU executable file.
################################################################################
find_qemu()
{
    [ -n "$QEMU" ] && return

    # qemu executable file name
    local b=qemu-system-x86_64

    # qemu executable file path
    QEMU=$HOME/QEMU_LINUX_KERNEL/bin/$b
    [ -x "$QEMU" ] && return

    QEMU=/usr/local/QLK/bin/$b
    [ -x "$QEMU" ] && return

    QEMU=$b
}

################################################################################
# Locates the boot.bin file.
################################################################################
find_boot()
{
    [ -n "$QEMU_BOOT" ] && return

    for d in $HOME/QEMU_LINUX_KERNEL /usr/local/QLK /usr/QLK /usr/local /usr /; do
        QEMU_BOOT=$d/lib/qlk/boot.bin
        [ -f "$QEMU_BOOT" ] && return
    done

    echo "Can't find boot.bin (is libqlk installed?)"
    exit 1
}

################################################################################
#
################################################################################
cmd=
find_qemu
find_boot
CEHDPATH=${CEHDPATH:-$HOME/QEMU_LINUX_KERNEL/share/hd.img}
source util/start.mk

if [ -n "$QEMU_FIFOS" ]; then
	for f in $QEMU_FIFOS; do
		rm -f $f.in $f.out
		mkfifo $f.in $f.out
	done
fi

cmd="$QEMU_PRE_CMD $QEMU \
	-kernel $QEMU_BOOT \
	-initrd build/system \
	-drive file=.swap,format=raw,index=0,cache=unsafe \
	-no-reboot \
	$QEMU_EXTRA_OPTIONS \
	-m $MEM"

if [ "$1" == -g ]; then
	cmd="$cmd -s -append \"-s\""
	shift
fi
	
trap 'stty sane' exit

if [ "$AUTOCORR" == 1 ]; then
	cmd="$cmd -nographic"
else
	cmd="$cmd -serial stdio"
fi

if [ -f "$CEHDPATH" ]; then
	cmd="$cmd -drive file=\"$CEHDPATH\",index=2,format=raw"
fi

cmd="$cmd $@"

if [ -n "$CERAW" ]; then
    show_log=
else
    show_log=${QEMU_POST_CMD:-"| util/show_log.pl"}
fi

echo Executing: $cmd
eval $cmd $show_log


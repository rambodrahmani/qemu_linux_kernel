#!/bin/sh

################################################################################
#   File: cleanup.sh
#         Utility script used during development for ease of use.
#
#  Usage: ./cleanup.sh
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019
################################################################################

# clean up QEMU folder
echo "*********************************"
echo "*  Cleaning up the QEMU folder  *"
echo "*********************************"
cd qemu-lk
rm -rf qemu*
cd ..
echo "Completed."

# clean up QEMU Linux Kernel library
echo ""
echo "*********************************"
echo "*  Cleaning up the QLK library  *"
echo "*********************************"
cd lib
rm -f *.o
rm -f *.a
rm -f *.bin
cd 64
rm -f *.o
cd ../32
rm -f *.o
cd ../boot64
rm -f *.o
cd ..
cd ..
echo "Completed."

# clean up I/O examples folder
echo ""
echo "*****************************************"
echo "*  Cleaning up the I/O examples folder  *"
echo "*****************************************"
cd io_examples

for D in `find . -type d`
do
    # skip the parent folder and the scripts folder
    if [ "$D" = . ] || [ "$D" = ./scripts ]
    then
        continue
    fi

    # remove executable and object file
    rm -f $D/*.out
    rm -f $D/*.o
done

rm -f setenv
cd scripts
rm -f compile
rm -f debug
rm -f boot
cd ..
cd ..
echo "Completed."

# clean up kernel folder
echo ""
echo "***********************************"
echo "*  Cleaning up the Kernel folder  *"
echo "***********************************"
cd kernel
rm -f system/*.o io/*.o user/*.o util/*.o
rm -f util/start.mk util/start.gdb util/start.pl
rm .swap
rm swap.img
cd ..
echo "Completed."

# clean up home folder
echo ""
echo "*************************************"
echo "*  Cleaning up the QLK home folder  *"
echo "*************************************"
rm -rf "$HOME/QEMU_LINUX_KERNEL"
echo "Completed."

# all clean ups terminated
echo ""
echo "************************************"
echo "*  Cleanup successfully completed  *"
echo "************************************"


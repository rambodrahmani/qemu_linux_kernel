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

# clean up home folder
echo "*************************************"
echo "*  Cleaning up the QLK home folder  *"
echo "*************************************"
rm -rf "$HOME/QEMU_LINUX_KERNEL"
echo "Completed."

# clean up QEMU folder
echo ""
echo "*****************************"
echo "*  Cleaning up QEMU folder  *"
echo "*****************************"
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
rm *.o
rm *.a
rm *.bin
cd 64
rm *.o
cd ../32
rm *.o
cd ../boot64
rm *.o
cd ..
cd ..
echo "Completed."

# clean up I/O examples folder
echo ""
echo "*****************************************"
echo "*  Cleaning up the I/O examples folder  *"
echo "*****************************************"
cd io_examples
rm setenv
cd scripts
rm compile
rm debug
rm boot
cd ..
echo "Completed."

echo ""
echo "************************************"
echo "*  Cleanup successfully completed  *"
echo "************************************"


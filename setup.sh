#!/bin/sh

################################################################################
#   File: setup.sh
#         This script
#           1) Install the patched version of QEMU;
#           2) Compile and install the library;
#           3) Prepare the the scripts to work with the I/O examples.
#
#  Usage: ./setup.sh
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019
################################################################################

# call the QEMU install bash script
echo "**********************************"
echo "*  Patching and installing QEMU  *"
echo "**********************************"
cd qemu-lk
/bin/bash install.sh
cd ..

# compile and install the QEMU Linux Kernel library
echo ""
echo "**********************************************"
echo "*  Compiling and installing the QLK library  *"
echo "**********************************************"
cd lib
make
make install
cd ..

# Prepare scripts to work with the I/O examples
echo ""
echo "***********************************************"
echo "*  Compiling and installing the I/O examples  *"
echo "***********************************************"
cd io_examples
make
make install
cd ..

echo ""
echo "**********************************"
echo "*  Setup successfully completed  *"
echo "**********************************"


#!/bin/sh

################################################################################
#   File: install.sh
#         This script downloads, patches, configures, builds and installs QEMU.
#         The applied patches are required in order to be able to use QEMU
#         for the Linux Kernel.
#
#  Usage: ./install.sh
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 04/07/2019
################################################################################

# check and create the LK (Linux Kernel) folder in the home directory
if [ -z "$PREFIX" ]; then
	PREFIX="$HOME/LK"
fi
VERS=3.1.0

# download QEMU source files in the current directory
echo "Downloading QEMU v$VERS..."
[ -e qemu-$VERS.tar.xz ] || wget https://download.qemu.org/qemu-$VERS.tar.xz

# unzip qemu source files in the current directory
echo "Unzipping source files..."
rm -rf qemu-$VERS
tar xf qemu-$VERS.tar.xz

# cd to the qemu source files folder
cd qemu-$VERS

# patch the source files
echo "Patching..."
for f in ../*.patch; do 
	patch -p1 < $f
done

# configure the source files for building: prefix = $HOME/LK
echo "Configuring..."
./configure \
	--target-list=x86_64-softmmu \
	--disable-xen \
	--disable-vnc \
	--disable-curses \
	--disable-user \
	--disable-vhost-net \
	--disable-virtfs \
	--disable-attr \
	--disable-slirp \
	--enable-sdl \
	--audio-drv-list=alsa \
	--enable-debug \
	--disable-pie \
	--disable-capstone \
	--prefix=$PREFIX \

# build qemu source files
echo "Building..."
make
cp pc-bios/optionrom/multiboot.bin pc-bios

# install qemu
echo "Installing..."
make install || sudo make install


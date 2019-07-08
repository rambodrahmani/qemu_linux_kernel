# Kernel

The Kernel is made up of the following modules

1. system
2. io
3. user

The `system` module must be loaded by the boot-loader while the `io` and the
`user` modules have to be loaded from the swap partition.

The `io` module contains the external processes concerning the I/O devices:

1. serial ports
2. keyboard
3. text mode video
4. hard disk
5. bus mastering

other external devices can be added modifying the `io` module itself.

The `user` module contains the program which must be executed by the kernel. The
user program can be coded in C++ and can use all the utility methods introduced
in the QEMU Linux Kernel Library.

Once compiled, the `io` module and the `user` module must be placed in the swap
partition.

The Kernel can be executed on the real machine (not advised) or in an emulator.
When using an emulator, the swap partition will be emulated using a swap file.

## Prerequisites
Before moving on and working with the Kernel you will have to

1. install the QEMU Linux Kernel Library;
2. install the patched QEMU version.

Both of these are provided within this repo with plenty of instructions.

## Writing a User Program
The following steps must be executed everytime you want to try a new user
program or make any changes to the `io` module.

The user programs must be written in C++, compiled and compied in the swap
partition.

Keep in mind that you will be able to use only the library methods declared in
`user/include/lib.h` and the system primitives declared in `user/include/sys.h`.
Some example programs can be found in `user/examples`.

--

Originally seen here: [Calcolatori Elettronici - Unipi - Ing. Informatica](http://calcolatori.iet.unipi.it/)


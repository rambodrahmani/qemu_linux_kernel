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
Some example programs can be found in `user/examples`. These example programs
are written using keywords such as `process`, `semaphpore` etc.., and must
therefore be first processed using the `parse` tool which takes an `.in` file as
intput and produces the `user.cpp` file containing the main function.

Place your program in the `user/prog` folder (you can also just copy there one
of the provided examples at the beginning), and compile it using the following
commands:
```console
make
make swap
```

If everything works fine, the following files will be created:

1. `build/system`: the system module to be loaded by the boot-loader;
2. `build/parse`: the program which automatically produces the `user.cpp` file;
3. `build/make_swap`: the program which copies the `user` and the `io` module in
the swap partition;
4. `build/user.cpp`: the `user.cpp` file produced by the `parse` program;
5. `build/user`: the `user` module resulting from the compilation of
`user/user.cpp`, `user/user.s` and `user/lib.cpp` files;
6. `build/io`: the `io` module resulting from the compilation of `io/io.cpp` and
`io/io.s`;
7. `swap.img`: the swap partition.

## Boot the Kernel
To start the Kernel you can use the provided script:
```console
./run.sh
```
Which will launch the virtual machine, boot the `build/system` module in 64 bit
mode, and jump to the `start` label in `system/system.s`. The terminal where the
script is executed can be used to receive feedback messages from the serial port
of the emulator running the Kernel. The first outputs come from the boot-loader
program and then those coded using the `flog` function.

## Using the debugger


# TO-DO
- [ ] Finish writing the README file
- [ ] Finish comment the source code files
- [ ] Split up `io` and `system` source files into multiple source files

--

Originally seen here: [Calcolatori Elettronici - Unipi - Ing. Informatica](http://calcolatori.iet.unipi.it/)


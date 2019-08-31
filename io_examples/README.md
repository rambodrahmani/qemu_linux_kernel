# I/O Examples

This folder contains several I/O source code examples written in C++ which can
be compiled and executed on the core system.

## Install utility scripts
You can use
```console
make
make install
```
To install the utility scripts

1. compile
2. boot
3. debug

to the QEMU Linux Kernel directory and generate the `setenv` file.

## Compile and run
Each I/O example is contained in a separate folder. To compile and run any of
them just move to the desidered example folder and execute
```console
compile
boot
```

###### Keep in mind you will first have to execute
```console
source setenv
```

## I/O Examples Order
Make sure you go through the I/O examples in the following order:
1.  keyboard-1
2.  keyboard-2
3.  svga-1
4.  svga-2
5.  svga-3
6.  video-1
7.  video-2
8.  timer-1
9.  serial-1
10. serial-2
11. hard-disk-1
12. hard-disk-2
13. keyboard-1-interrupt
14. keyboard-2-interrupt
15. interrupt-keyboard
16. interrupt-timer-1
17. interrupt-timer-2

# TO-DO

- [ ] Add I/O examples `mem1` and `mem2`

--

Originally seen here: [Calcolatori Elettronici - Unipi - Ing. Informatica](http://calcolatori.iet.unipi.it/)


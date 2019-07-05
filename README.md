# QEMU Linux Kernel
A custom linux based kernel for QEMU.

## Why?
When a computer is turned off, its software — including operating systems,
application code, and data — remains stored on non-volatile memory. When the
computer is powered on, it typically does not have an operating system or its
loader in random-access memory (RAM). The computer first executes a relatively
small program stored in read-only memory (ROM) along with a small amount of
needed data, to access the nonvolatile device or devices from which the
operating system programs and data can be loaded into RAM.The small program that
starts this sequence is known as a bootstrap loader, bootstrap or boot loader.
This small program's only job is to load other data and programs which are then
executed from RAM.

#### Once the boot loader is done loading the operating system, very little control is left to the user in order to be able to directly work with the interfaces or the interrupt mechanism.

For this reason, the following linux based kernel was developed in order to be
able to write Assembly or C++ programs and be able to
1. use all available instructions: also thos related to the interrupt mechanism;
2. freely access to the interfaces registers, also to the ones critical for the
operating system (e.g. the keyboard and the video output);
3. freely access the interrupt controller registers;
4. fully access the IDT (Interrupt Descriptor Table) for the interrupt
mechanism;

## C++ and Assembly
The Kernel was mainly developed in Assembly and C++, using the GNU/GCC (The GNU
Compiler Collection).

Assembly was used wherever C++ could not complete the required tasks (e.g.
accessing the I/O space) and both x32 and x64 implementations of the native
functions are provided.

User software can be developed using both Assembly and C++.

## Text and graphic mode

## Compiling and executing a user program
First of all, anytime you open up a new terminal instance, you will have to set
your `PATH` in order to be able to use some custom executables developer for
easy of use when deploying to the Linux Kernel.
```console
source setenv
```
Each user program can be made up of several source files. For the sake of
simplicity, let's assume there are all in the same folder: as you will be able
to see, this is also the case for all the examples showing the in I/O Examples
folder. In order to compile and link the program you can use the following
command:
```console
compile output-file-name
```
This will take care of compiling and linking all the C++ and Assembly files
contained in the current directory, producing the executable `output-file-name`
(output-file-name can be omitted, in which case the default `a.out` executable
file name will be used).

To load the program on the QEMU emulator and execute it, execute the following
command within the directory where the executable is placed:
```console
boot output-file-name
```
again, `output-file-name` is the executable file name.

### Compiling and executing a user program with Graphic Mode enabled
If the user program uses the graphic mode, you will have to add the `-G` option
to the `compile` command:
```console
compile -G output-file-name
```
whereas he `boot` command stays the same
```console
boot output-file-name
```

## Using GDB
GDB (GNU Debugger) can be used to debug user programs running on the QEMU
emulator. In order to be able to do it:

1. the user program bust be compiled with the debugging option (`-g`);
2. the qemu emulator, at startup, will pause and wait for an incoming connection
from GDB;
3. GDB need connection parameters to communicate with QEMU.

The `compile` command always enables the debugging option as default, so you
won't have to do any change to the procedures previously descripted. However you
will have to change the `boot` command in order to tell QEMU of the incoming
connection from GDB. To do so, just add the `-g` option to the `boot` command:
```console
boot -g output-file-name
```
Once done, you will have the following result: QEMU is waiting for the incoming
GDB connection
![qemu_witing_for_gdb_connection](/img/gdb-0.png)
Open up a new terminal and move to the same folder where you executed the
`boot -g` command for the user program. This time execute the `debug` command
![gdb_connection_to_qemu](/img/gdb-1.png)
Use the `continue` command in the GDB terminal to start the program.
![qemu_execution](/img/gdb-2.png)
The `debug` command will

1. execute GDB and provide connection parameters to reach the QEMU emulator;
2. add a breakpoint at the beginning of the `main` function;
3. launch the user program execution;
4. the execution will hang at the first default breakpoint, at which point,
you can debug your program using the terminal where you executed GDB (the
`debug` command) as you normally do using GDB.

## Patching and installing QEMU
QEMU (short for Quick Emulator) is a free and open-source emulator that performs
hardware virtualization. QEMU is a hosted virtual machine monitor: it emulates
the machine's processor through dynamic binary translation and provides a set of
different hardware and device models for the machine, enabling it to run a
variety of guest operating systems. It also can be used with KVM to run virtual
machines at near-native speed (by taking advantage of hardware extensions such
as Intel VT-x). QEMU can also do emulation for user-level processes, allowing
applications compiled for one architecture to run on another.

#### We will be using a patched version of QEMU.

### Prerequisites
The following packages must be installed before continuing:

1. zlib
2. SDL2
3. pixman
4. gcc
5. python

On Arch Linux all you need is:
```console
[rambodrahmani@rr-laptop ~]$ yay -S lib32-zlib zlib lib32-sdl2 sdl2 lib32-pixman pixman make gcc lib32-gcc-libs python python2
[sudo] password for rambodrahmani: 
resolving dependencies...
looking for conflicting packages...

Packages (12) lib32-libxcursor-1.2.0-1  gcc-9.1.0-2  lib32-gcc-libs-9.1.0-2  lib32-pixman-0.38.4-1  lib32-sdl2-2.0.9-1  lib32-zlib-1.2.11-2
              make-4.2.1-3  pixman-0.38.4-1  python-3.7.3-2  python2-2.7.16-1  sdl2-2.0.9-1  zlib-1:1.2.11-3

Total Download Size:   101.04 MiB
Total Installed Size:  447.23 MiB
Net Upgrade Size:        2.08 MiB

:: Proceed with installation? [Y/n] Y
:: Retrieving packages...
 zlib-1:1.2.11-3-x86_64                                            81.3 KiB  2033K/s 00:00 [#####################################################] 100%

                                            [...]

(12/12) installing python2                                                               [#####################################################] 100%
:: Running post-transaction hooks...
(1/2) Arming ConditionNeedsUpdate...
(2/2) Updating the info directory file...
```

### Compiling and installing
The install.sh script in the qemu-lk folder will take care of downloading the
qemu source files, patching them, configuring them for building, and finally
build and install qemu from source.
```console
[rambodrahmani@rr-laptop qemu_linux_kernel]$ ./qemu-lk/install.sh
Downloading QEMU v3.1.0...
--2019-07-04 16:55:35--  https://download.qemu.org/qemu-3.1.0.tar.xz
Loaded CA certificate '/etc/ssl/certs/ca-certificates.crt'

                                            [...]

Unzipping source files...
Patching...
patching file hw/i386/multiboot.c
patching file pc-bios/optionrom/multiboot.S
patching file default-configs/pci.mak
patching file hw/misc/Makefile.objs
patching file hw/misc/ce.c
patching file hw/ide/pci.c
patching file exec.c
patching file gdbstub.c
Configuring...
Install prefix    /home/rambodrahmani/LK
BIOS directory    /home/rambodrahmani/LK/share/qemu
firmware path     /home/rambodrahmani/LK/share/qemu-firmware
binary directory  /home/rambodrahmani/LK/bin
library directory /home/rambodrahmani/LK/lib

                                            [...]

sheepdog support  yes
capstone          no
docker            no
libpmem support   no
libudev           yes

NOTE: cross-compilers enabled:  'cc'
Building...
  GEN     x86_64-softmmu/config-devices.mak.tmp
  GEN     x86_64-softmmu/config-devices.mak
  GEN     config-all-devices.mak
  GEN     config-host.h

                                            [...]

	 DEP tests/find_property.c
	 DEP tests/root_node.c
	 DEP tests/get_mem_rsv.c
	 DEP libfdt/fdt_overlay.c
	 DEP libfdt/fdt_addresses.c
	 DEP libfdt/fdt_empty_tree.c
	 DEP libfdt/fdt_strerror.c
	 DEP libfdt/fdt_rw.c
	 DEP libfdt/fdt_sw.c
	 DEP libfdt/fdt_wip.c
	 DEP libfdt/fdt_ro.c
	 DEP libfdt/fdt.c
	 DEP util.c

                                            [...]

  GEN     qga/qapi-generated/qapi-gen
  GEN     docs/interop/qemu-ga-ref.html
  GEN     docs/interop/qemu-ga-ref.txt
  GEN     docs/interop/qemu-ga-ref.7
  GEN     docs/qemu-block-drivers.7
  GEN     docs/qemu-cpu-models.7
  CC      qga/commands.o
  CC      qga/guest-agent-command-state.o
  CC      qga/main.o
  CC      qga/commands-posix.o
  CC      qga/channel-posix.o

                                            [...]

  CC      x86_64-softmmu/target/i386/hyperv.o
  CC      x86_64-softmmu/target/i386/sev.o
  GEN     trace/generated-helpers.c
  CC      x86_64-softmmu/trace/generated-helpers.o
  CC      x86_64-softmmu/trace/control-target.o
  LINK    x86_64-softmmu/qemu-system-x86_64
  AS      optionrom/multiboot.o
  BUILD   optionrom/multiboot.img
  BUILD   optionrom/multiboot.raw
  SIGN    optionrom/multiboot.bin
  AS      optionrom/linuxboot.o
  BUILD   optionrom/linuxboot.img
  BUILD   optionrom/linuxboot.raw
  SIGN    optionrom/linuxboot.bin
  CC      optionrom/linuxboot_dma.o
  BUILD   optionrom/linuxboot_dma.img
  BUILD   optionrom/linuxboot_dma.raw
  SIGN    optionrom/linuxboot_dma.bin
  AS      optionrom/kvmvapic.o
  BUILD   optionrom/kvmvapic.img
  BUILD   optionrom/kvmvapic.raw
  SIGN    optionrom/kvmvapic.bin
Installing...
	CHK version_gen.h
install -d -m 0755 "/home/rambodrahmani/LK/share/doc/qemu"
install -c -m 0644 qemu-doc.html "/home/rambodrahmani/LK/share/doc/qemu"
install -c -m 0644 qemu-doc.txt "/home/rambodrahmani/LK/share/doc/qemu"
install -c -m 0644 docs/interop/qemu-qmp-ref.html "/home/rambodrahmani/LK/share/doc/qemu"
install -c -m 0644 docs/interop/qemu-qmp-ref.txt "/home/rambodrahmani/LK/share/doc/qemu"
install -d -m 0755 "/home/rambodrahmani/LK/share/man/man1"
install -c -m 0644 qemu.1 "/home/rambodrahmani/LK/share/man/man1"
install -d -m 0755 "/home/rambodrahmani/LK/share/man/man7"
install -c -m 0644 docs/interop/qemu-qmp-ref.7 "/home/rambodrahmani/LK/share/man/man7"
install -c -m 0644 docs/qemu-block-drivers.7 "/home/rambodrahmani/LK/share/man/man7"
install -c -m 0644 docs/qemu-cpu-models.7 "/home/rambodrahmani/LK/share/man/man7"
install -c -m 0644 qemu-img.1 "/home/rambodrahmani/LK/share/man/man1"
install -d -m 0755 "/home/rambodrahmani/LK/share/man/man8"
install -c -m 0644 qemu-nbd.8 "/home/rambodrahmani/LK/share/man/man8"
install -c -m 0644 qemu-ga.8 "/home/rambodrahmani/LK/share/man/man8"
install -c -m 0644 docs/interop/qemu-ga-ref.html "/home/rambodrahmani/LK/share/doc/qemu"
install -c -m 0644 docs/interop/qemu-ga-ref.txt "/home/rambodrahmani/LK/share/doc/qemu"
install -c -m 0644 docs/interop/qemu-ga-ref.7 "/home/rambodrahmani/LK/share/man/man7"
install -d -m 0755 "/home/rambodrahmani/LK/share/qemu"

                                            [...]
                                            
install -c -m 0644 /home/rambodrahmani/DevOps/qemu_linux_kernel/qemu-lk/qemu-3.1.0/trace-events-all "/home/rambodrahmani/LK/share/qemu/trace-events-all"
for d in x86_64-softmmu; do \
make --no-print-directory --quiet BUILD_DIR=/home/rambodrahmani/DevOps/qemu_linux_kernel/qemu-lk/qemu-3.1.0 TARGET_DIR=$d/ -C $d install || exit 1 ; \
        done
```

--

Originally seen here: [Calcolatori Elettronici - Unipi - Ing. Informatica](http://calcolatori.iet.unipi.it/)


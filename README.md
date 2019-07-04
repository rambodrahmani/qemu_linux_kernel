# QEMU Linux Kernel
A custom linux based kernel for QEMU.

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

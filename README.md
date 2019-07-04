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
 lib32-gcc-libs-9.1.0-2-x86_64                                     19.3 MiB  4.69M/s 00:04 [#####################################################] 100%
 make-4.2.1-3-x86_64                                              412.7 KiB  5.04M/s 00:00 [#####################################################] 100%
 gcc-9.1.0-2-x86_64                                                33.9 MiB  4.69M/s 00:07 [#####################################################] 100%

                                            [...]

(11/12) reinstalling python                                                                [#####################################################] 100%
(12/12) reinstalling python2                                                               [#####################################################] 100%
:: Running post-transaction hooks...
(1/2) Arming ConditionNeedsUpdate...
(2/2) Updating the info directory file...
```

# Library

Support library for the Kernel. This library contains utility functions such as
reading a char from the keyboard or output some text to the QEMU video output.

The utility functions can be used also by user program: just include the
`libqlk.h` file in each source file where you need to use them.

## A brief note on x86-64 and x86-32
Nowadays, within Personal Computers, there are mainly x86-64 architecture CPUs,
which use 64 bits registers, addresses and data types. This architecture was
initially introduced by AMD and later adopted by Intel (it is actually different
from the IA 64, the one originally proposed by Intel and only used by the
Itanium processor).

For compatibility reasons, x86-64 processors can operate in "legacy mode",
allowing the user to execute programs developed for the x86-32 architeture: to
be precise, at startup, an x86-64 CPU shows up running in 8086 mode, and later
decide whether to work on x86-32 mode or x86-64 mode.

### Real Mode
Real mode, also called real address mode, is an operating mode of all
x86-compatible CPUs. Real mode is characterized by a 20-bit segmented memory
address space (giving exactly 1 MiB of addressable memory) and unlimited direct
software access to all addressable memory, I/O addresses and peripheral
hardware. Real mode provides no support for memory protection, multitasking, or
code privilege levels.

Before the release of the 80286, which introduced protected mode, real mode was
the only available mode for x86 CPUs; and for backward compatibility, all x86
CPUs start in real mode when reset, though it is possible to emulate real mode
on other systems when starting on other modes.

Intel introduced protected mode into the x86 family with the intention that
operating systems which used it would run entirely in the new mode and that
all programs running under a protected mode operating system would run in
protected mode as well. Because of the substantial differences between real
mode and even the rather limited 286 protected mode, programs written for real
mode cannot run in protected mode without being rewritten. Therefore, with a
wide base of existing real mode applications which users depended on,
abandoning real mode posed problems for the industry, and programmers sought a
way to switch between the modes at will. However, Intel, consistent with their
intentions for the processor's usage, provided an easy way to switch into
protected mode on the 80286 but no easy way to switch back to real mode.
Before the 386 the only way to switch from protected mode back to real mode
was to reset the processor; after a reset it always starts up in real mode to
be compatible with earlier x86 CPUs back to the 8086. Resetting the processor
does not clear the system's RAM, so this, while awkward and inefficient, is
actually feasible. From protected mode, the processor's state is saved in
memory, then the processor is reset, restarts in real mode, and executes some
real mode code to restore the saved state from memory. It can then run other
real mode code until the program is ready to switch back to protected mode.
The switch to real mode is costly in terms of time, but this technique allows
protected mode programs to use services such as BIOS, which runs entirely in
real mode (having been designed originally for the 8088-based IBM Personal
Computer model (machine type) 5150). This mode-switching technique is also the
one used by DPMI (under real, not emulated, DOS) and DOS extenders like
DOS/4GW to allow protected mode programs to run under DOS; the DPMI system or
DOS extender switches to real mode to invoke DOS or BIOS calls, then switches
back to return to the application program which runs in protected mode.

### Protected Mode
In computing, protected mode, also called protected virtual address mode, is
an operational mode of x86-compatible central processing units (CPUs). It
allows system software to use features such as virtual memory, paging and safe
multi-tasking designed to increase an operating system's control over
application software. When a processor that supports x86 protected mode is
powered on, it begins executing instructions in real mode, in order to
maintain backward compatibility with earlier x86 processors. Protected mode
may only be entered after the system software sets up one descriptor table and
enables the Protection Enable (PE) bit in the control register 0 (CR0).
Protected mode was first added to the x86 architecture in 1982, with the
release of Intel's 80286 (286) processor, and later extended with the release
of the 80386 (386) in 1985. Due to the enhancements added by protected mode,
it has become widely adopted and has become the foundation for all subsequent
enhancements to the x86 architecture, although many of those enhancements,
such as added instructions and new registers, also brought benefits to the
real mode.

#### Features
Protected mode has a number of features designed to enhance an operating
system's control over application software, in order to increase security and
system stability. These additions allow the operating system to function in a
way that would be significantly more difficult or even impossible without proper
hardware support.

##### Privilege levels
##### Real mode application compatibility
##### Virtual 8086 mode 
##### Paging
##### Multitasking

# TO-DO
- [ ] Finish writing the README file
- [ ] Finish comment the source code files

--

Originally seen here: [Calcolatori Elettronici - Unipi - Ing. Informatica](http://calcolatori.iet.unipi.it/)


#*******************************************************************************
# File: 8259.s
#       The Intel 8259 is a Programmable Interrupt Controller (PIC) designed for
#       the Intel 8085 and Intel 8086 microprocessors. The initial part was
#       8259, a later A suffix version was upward compatible and usable with the
#       8086 or 8088 processor. The 8259 combines multiple interrupt input
#       sources into a single interrupt output to the host microprocessor,]
#       extending the interrupt levels available in a system beyond the one or
#       two levels found on the processor chip. The 8259A was the interrupt
#       controller for the ISA bus in the original IBM PC and IBM PC AT. 
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
# The master controller is located in the I/O space starting from offset 0x20
# while the slave controller is located in the I/O space starting from offset
# 0x0A.

# Initialization Control Word 1 (ICW1) Master Register Address
.set  ICW1M,  0x20

# Initialization Control Word 2 (ICW1) Master Register Address
.set  ICW2M,  0x21

# Initialization Control Word 3 (ICW1) Master Register Address
.set  ICW3M,  0x21

# Initialization Control Word 4 (ICW1) Master Register Address
.set  ICW4M,  0x21

# Operation Control Word 1 (OCW1) Master Register Address
.set  OCW1M,  0x21

# Operation Control Word 2 (OCW1) Master Register Address
.set  OCW2M,  0x20

# Operation Control Word 3 (OCW1) Master Register Address
.set  OCW3M,  0x20

# Initialization Control Word 1 (ICW1) Slave Register Address
.set  ICW1S,  0xa0

# Initialization Control Word 2 (ICW1) Slave Register Address
.set  ICW2S,  0xa1

# Initialization Control Word 3 (ICW1) Slave Register Address
.set  ICW3S,  0xa1

# Initialization Control Word 4 (ICW1) Slave Register Address
.set  ICW4S,  0xa1

# Operation Control Word 1 (OCW1) Slave Register Address
.set  OCW1S,  0xa1

# Operation Control Word 2 (OCW1) Slave Register Address
.set  OCW2S,  0xa0

# Operation Control Word 3 (OCW1) Salve Register Address
.set  OCW3S,  0xa0
#-------------------------------------------------------------------------------
.DATA
ocw1m_save:
    .BYTE 0

ocw1s_save:
    .BYTE 0
#-------------------------------------------------------------------------------
.GLOBAL disable_8259
#-------------------------------------------------------------------------------
disable_8259:
    pushq %rbp              # prologue
    movq  %rsp, %rbp
    pushq %rax

# master setup
    movb $0x11, %al         # 0x11 = 1000 1000
    outb %al, $ICW1M        # cascaded mode

    movb $0x20, %al
    outb %al, $ICW2M

    movb $0x04, %al
    outb %al, $ICW3M

    movb $0x01, %al
    outb %al, $ICW4M

    inb  $OCW1M, %al        # read current masked interrupts
    movb %al, ocw1m_save    # save current masked interrupts

    movb $0b11111011, %al   # mask all interrupts except those coming from the
    outb %al, $OCW1M        # cascaded slave

    movb $0x48, %al
    outb %al, $OCW3M

# slave setup
    movb $0x11, %al
    outb %al, $ICW1S
    movb $0x28, %al
    outb %al, $ICW2S
    movb $0x02, %al
    outb %al, $ICW3S
    movb $0x01, %al
    outb %al, $ICW4S

    inb  $OCW1S, %al        # read current masked interrupts
    movb %al, ocw1s_save    # save current masked interrupts

    movb $0b11111111, %al   # mask all interrupts
    outb %al, $OCW1S

    movb $0x48, %al		// fully nested
    outb %al, $OCW3S

    popq %rax
    leave
    ret

#-------------------------------------------------------------------------------
# Enables the 8259 in fully nested mode with the master having base type 0x08
# and the slave having type 0x70.
.GLOBAL enable_8259
#-------------------------------------------------------------------------------
enable_8259:
    pushq %rbp              # prologue
    movq  %rsp, %rbp
    pushq %rax

# master setup
    movb $0x11, %al         # 0x11 = 1000 1000
    outb %al, $ICW1M        # cascaded mode

    movb $0x08, %al         # master base type 0x08
    outb %al, $ICW2M

    movb $0x04, %al         # 0x04 = 0000 0100
    outb %al, $ICW3M        # slave connected to ping 2

    movb $0x01, %al         # 0x01 = 0000 0001
    outb %al, $ICW4M        # special fully nested mode

    movb ocw1m_save, %al    # presaved masked interrupts mask
    outb %al, $OCW1M        # set masked interrupts

    movb $0x48, %al         # 0x48 = 0100 1000
    outb %al, $OCW3M

# slave setup
    movb $0x11, %al         # 0x11 = 1000 1000
    outb %al, $ICW1S        # cascaded mode

    movb $0x70, %al         # slave base type 0x70
    outb %al, $ICW2S

    movb $0x02, %al         # 0x02 = 0000 0010
    outb %al, $ICW3S        # set slave id

    movb $0x01, %al         # 0x01 = 0000 0001
    outb %al, $ICW4S        # fully nested mode

    movb ocw1s_save, %al    # presaved masked interrupts mask
    outb %al, $OCW1S        # set masked interrupts

    movb $0x48, %al         # 0x48 = 0100 1000
    outb %al, $OCW3S

    popq %rax
    leave
    ret
#*******************************************************************************


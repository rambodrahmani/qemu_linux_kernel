#*******************************************************************************
# File: 8259.s
#       The Intel 8259 is a Programmable Interrupt Controller (PIC) designed for
#       the Intel 8085 and Intel 8086 microprocessors. The initial part was
#       8259, a later A suffix version was upward compatible and usable with the
#       8086 or 8088 processor. The 8259 combines multiple interrupt input
#       sources into a single interrupt output to the host microprocessor,
#       extending the interrupt levels available in a system beyond the one or
#       two levels found on the processor chip. The 8259A was the interrupt
#       controller for the ISA bus in the original IBM PC and IBM PC AT.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
# Please refer to ../64/8259.s for proper comments.
#-------------------------------------------------------------------------------
.set  ICW1M,  0x20
.set  ICW2M,  0x21
.set  ICW3M,  0x21
.set  ICW4M,  0x21
.set  OCW1M,  0x21
.set  OCW3M,  0x20
.set  ICW1S,  0xa0
.set  ICW2S,  0xa1
.set  ICW3S,  0xa1
.set  ICW4S,  0xa1
.set  OCW1S,  0xa1
.set  OCW3S,  0xa0
.set  OCW2M,  0x20
.set  OCW3M,  0x20
.set  OCW2S,  0xa0
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
    pushl %ebp
    movl  %esp, %ebp
    pushl %eax

    movb $0x11, %al
    outb %al, $ICW1M

    movb $0x20, %al
    outb %al, $ICW2M

    movb $0x04, %al
    outb %al, $ICW3M

    movb $0x01, %al
    outb %al, $ICW4M

    inb  $OCW1M, %al
    movb %al, ocw1m_save

    movb $0b11111011, %al
    outb %al, $OCW1M

    movb $0x48, %al
    outb %al, $OCW3M

    movb $0x11, %al
    outb %al, $ICW1S

    movb $0x28, %al
    outb %al, $ICW2S

    movb $0x02, %al
    outb %al, $ICW3S

    movb $0x01, %al
    outb %al, $ICW4S

    inb  $OCW1S, %al
    movb %al, ocw1s_save

    movb $0b11111111, %al
    outb %al, $OCW1S

    movb $0x48, %al
    outb %al, $OCW3S

    popl %eax
    leave
    ret

#-------------------------------------------------------------------------------
.GLOBAL enable_8259
#-------------------------------------------------------------------------------
enable_8259:
    pushl %ebp
    movl  %esp, %ebp
    pushl %eax

    movb $0x11, %al
    outb %al, $ICW1M

    movb $0x08, %al
    outb %al, $ICW2M

    movb $0x04, %al
    outb %al, $ICW3M

    movb $0x01, %al
    outb %al, $ICW4M

    movb ocw1m_save, %al
    outb %al, $OCW1M

    movb $0x48, %al
    outb %al, $OCW3M

    movb $0x11, %al
    outb %al, $ICW1S

    movb $0x70, %al
    outb %al, $ICW2S

    movb $0x02, %al
    outb %al, $ICW3S

    movb $0x01, %al
    outb %al, $ICW4S

    movb ocw1s_save, %al
    outb %al, $OCW1S

    movb $0x48, %al
    outb %al, $OCW3S

    popl %eax
    leave
    ret
#*******************************************************************************


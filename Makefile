################################################################################
#
# File:	Makefile
# 		Code compilation directives.
# 		Compile and install using
# 			$ make
# 			$ make install
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 19/03/2019
#
################################################################################

################################################################################
# Compilation output paths.
################################################################################
PREFIX=$(HOME)/QEMU_LINUX_KERNEL
ETC=$(PREFIX)/etc
BIN=$(PREFIX)/bin
LIB=$(PREFIX)/lib/ce
LIB64=$(PREFIX)/lib64/ce
INCLUDE=$(PREFIX)/include/ce

################################################################################
# QEMU Directives.
################################################################################
HD_SIZE=20M
HD_PATH=$(PREFIX)/share/hd.img
QEMU_BOOT=$(LIB)/boot.bin

################################################################################
# Compilation directives.
################################################################################
CXXFLAGS=-nostdlib -fno-exceptions -g -fno-rtti -fno-stack-protector -mno-red-zone -gdwarf-2 -fpic -fuse-ld=gold
CXX=g++
AS=gcc -c -x assembler-with-cpp -g
BCC:=$(CXX) -m32
BLD:=ld.gold
BCFLAGS:=$(CXXFLAGS) -m32 -I.
BLDFLAGS:=$(LDFLAGS) -melf_i386 -L. -L..
BLDLIBS:=-lce
START_BOOT=0x0000000000100000
CXX_SOURCES:=$(wildcard *.cpp)
AS32_SOURCES:=$(wildcard 32/*.s)
AS64_SOURCES:=$(wildcard 64/*.s)
HEADERS:=$(wildcard *.h)
OBJECTS32:=$(addsuffix .o, $(basename $(CXX_SOURCES))) $(addsuffix _s.o, $(basename $(AS32_SOURCES)))
OBJECTS64:=$(addsuffix -m64.o, $(basename $(CXX_SOURCES))) $(addsuffix _s.o, $(basename $(AS64_SOURCES)))

all: libce.a libce64.a boot.bin start64.o libce.conf

$(OBJECTS32): CXXFLAGS += -m32
$(OBJECTS32): AS += -m32
$(OBJECTS64): CXXFLAGS += -m64 -DX86_64
$(OBJECTS64): AS += -m64

$(OBJECTS32) $(OBJECTS64): $(HEADERS)

libce.a: $(OBJECTS32)
	ar r libce.a $?

libce64.a: $(OBJECTS64)
	ar r libce64.a $?

%_s.o: %.s
	$(AS) $*.s -o $@

%-m64.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $*.cpp

start64.o: rt/start64.s 
	$(AS) -m64 rt/start64.s -o $@

start.o: rt/start.s 
	$(AS) -m32 rt/start.s -o $@

boot.bin: boot64/boot_s.o boot64/boot_cpp.o
	$(BLD) $(BLDFLAGS) -o boot.bin -Ttext $(START_BOOT) boot64/boot_s.o boot64/boot_cpp.o $(BLDLIBS)

# Compile boot.s
boot64/boot_s.o: boot64/boot.S boot64/mboot.h
	$(BCC) $(BCFLAGS) -c boot64/boot.S -o boot64/boot_s.o

# Compile boot.cpp
boot64/boot_cpp.o: boot64/boot.cpp boot64/mboot.h boot64/elf64.h
	$(BCC) $(BCFLAGS) -c boot64/boot.cpp -o boot64/boot_cpp.o

################################################################################
# Install directives.
################################################################################


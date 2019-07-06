################################################################################
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
#                           Compilation output paths
################################################################################
PREFIX=$(HOME)/QEMU_LINUX_KERNEL
ETC=$(PREFIX)/etc
BIN=$(PREFIX)/bin
LIB=$(PREFIX)/lib/qlk
LIB64=$(PREFIX)/lib64/qlk
INCLUDE=$(PREFIX)/include/qlk

################################################################################
#                               QEMU directives
################################################################################
HD_SIZE=20M
HD_PATH=$(PREFIX)/share/hd.img
QEMU_BOOT=$(LIB)/boot.bin

################################################################################
#                            Compilation directives
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

all: libqlk.a libqlk64.a boot.bin start64.o libqlk.conf

$(OBJECTS32): CXXFLAGS += -m32
$(OBJECTS32): AS += -m32
$(OBJECTS64): CXXFLAGS += -m64 -DX86_64
$(OBJECTS64): AS += -m64

$(OBJECTS32) $(OBJECTS64): $(HEADERS)

libqlk.a: $(OBJECTS32)
	ar r libqlk.a $?

libqlk64.a: $(OBJECTS64)
	ar r libqlk64.a $?

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
#                             Install directives
################################################################################
.PHONY: zerohd clean install

$(HD_PATH):
	mkdir -p $(dir $(HD_PATH))
	truncate -s $(HD_SIZE) $(HD_PATH)

zerohd:
	dd if=/dev/zero of=$(HD_PATH) bs=$(HD_SIZE) count=1

libqlk.conf:
	rm -f $@
	( $(foreach v,BIN LIB LIB64 INCLUDE HD_SIZE HD_PATH QEMU_BOOT,echo $v=$($v);) ) > $@

install: $(HD_PATH) 
	install -d					$(ETC)
	install -m 0444 libqlk.conf	$(ETC)
	install -d					$(BIN)
	install -d					$(LIB)
	install -d					$(LIB64)
	install -m 0444 libqlk.a	$(LIB)
	install -m 0444 libqlk64.a	$(LIB64)
	install -m 0444 start64.o	$(LIB64)
	install -d					$(INCLUDE)
	install -m 0444 *.h			$(INCLUDE)
	install -m 0444 libqlk.s	$(INCLUDE)
	install -m 0444 boot.bin	$(LIB)

clean:
	rm -f *.o 32/*.o 64/*.o libqlk.a libqlk64.a boot.bin boot64/*.o


################################################################################
# File:	Makefile
#       Code compilation directives.
#       Compile and install using
#          $ make
#          $ make swap
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 19/03/2019
#
################################################################################

##
LIBQLKCONF ?= $(firstword $(wildcard /usr/local/etc/libqlk.conf) $(wildcard /etc/libqlk.conf) $(wildcard $(HOME)/QEMU_LINUX_KERNEL/etc/libqlk.conf))
$(if $(LIBQLKCONF),,$(error libqlk not found))
include $(LIBQLKCONF)

START_SISTEMA = 0x0000000000200000
SWAP = swap.img

-include util/start.mk

NCC ?= g++
NLD ?= ld.gold

COMM_CFLAGS=$(CFLAGS)     \
    -Wall    -            \
    -nostdlib             \
    -fno-exceptions       \
    -fno-rtti             \
    -fno-stack-protector  \
    -Iinclude             \
    -I$(INCLUDE)          \
    -mno-red-zone         \
    -g3

COMM_LDFLAGS=\
       -nostdlib

COMM_LDLIBS=

NCFLAGS=$(COMM_CFLAGS) -m64 -Wno-builtin-declaration-mismatch -DX86_64
ifdef DEBUG
	NCFLAGS+=-DDEBUG
endif
NLDFLAGS=$(COMM_LDFLAGS) -melf_x86_64 -L$(LIB64)
NLDLIBS=$(COMM_LDLIBS) -lce64

ifdef AUTOCORR
	NCFLAGS+=-DAUTOCORR
endif

all: \
     build/system     \
     build/parse      \
     build/make_swap  \
     user/prog        \
     util/start.gdb

toparse := $(wildcard user/prog/*)

build/system: system/system_s.o system/system_cpp.o
	$(NLD) $(NLDFLAGS) -o build/system -Ttext $(START_SISTEMA) system/system_s.o system/system_cpp.o $(NLDLIBS)

build/io: io/io_s.o io/io_cpp.o
	$(NLD) $(NLDFLAGS) -o build/io -Ttext $(START_IO) io/io_s.o io/io_cpp.o $(NLDLIBS)

build/user: user/user_s.o user/lib.o user/user_cpp.o
	$(NLD) $(NLDFLAGS) -o build/user -Ttext $(START_UTENTE) user/user_cpp.o user/user_s.o user/lib.o $(NLDLIBS)

# compilazione di system.s e system.cpp
system/syst_s.o: system/system.s include/constants.h
	$(NCC) $(NCFLAGS) -x assembler-with-cpp -c system/system.s -o systema/syst_s.o

system/syst_cpp.o: system/system.cpp include/constants.h
	$(NCC) $(NCFLAGS) -c system/system.cpp -o system/syst_cpp.o

# compilazione di io.s e io.cpp
io/io_s.o: io/io.s include/constants.h
	$(NCC) $(NCFLAGS) -x assembler-with-cpp -c io/io.s -o io/io_s.o

io/io_cpp.o: io/io.cpp include/constants.h
	$(NCC) $(NCFLAGS) -fpic -c io/io.cpp -o io/io_cpp.o

# compilazione di user.s e user.cpp
user/user_s.o: user/user.s include/constants.h
	$(NCC) $(NCFLAGS) -x assembler-with-cpp -c user/user.s -o user/user_s.o

user/user.cpp: build/parse user/include/* user/prog $(toparse)
	build/parse -o user/user.cpp $(toparse)

user/user_cpp.o: user/user.cpp
	$(NCC) $(NCFLAGS) -fpic -Iuser/include -c user/user.cpp -o user/user_cpp.o

user/lib.o: user/lib.cpp user/include/lib.h
	$(NCC) $(NCFLAGS) -fpic -Iuser/include -c user/lib.cpp -o user/lib.o

# creazione di parse e createimg
build/parse: util/parse.c util/src.h
	gcc -o build/parse util/parse.c

util/coff.o: include/constants.h util/interp.h util/coff.h util/dos.h util/coff.cpp
	g++ -c -g -Iinclude -o util/coff.o util/coff.cpp

util/elf32.o:  include/costanti.h util/interp.h include/elf.h util/elf32.h util/elf32.cpp
	g++ -c -g -Iinclude -o util/elf32.o util/elf32.cpp

util/elf64.o:  include/costanti.h util/interp.h include/elf.h include/elf64.h util/elf64.cpp
	g++ -c -g -Iinclude -o util/elf64.o util/elf64.cpp

util/interp.o: include/costanti.h util/interp.h util/interp.cpp
	g++ -c -g -Iinclude -o util/interp.o util/interp.cpp

util/swap.o: include/costanti.h util/swap.h util/swap.cpp
	g++ -c -g -Iinclude -o util/swap.o util/swap.cpp

util/fswap.o: include/costanti.h util/swap.h util/fswap.cpp
	g++ -c -g -Iinclude -o util/fswap.o util/fswap.cpp

util/creatimg.o: include/costanti.h util/interp.h util/swap.h util/creatimg.cpp
	g++ -c -g -Iinclude -o util/creatimg.o util/creatimg.cpp

build/creatimg: util/creatimg.o util/elf32.o util/elf64.o util/coff.o util/interp.o util/swap.o util/fswap.o
	g++ -g -o build/creatimg util/creatimg.o util/elf32.o util/elf64.o util/coff.o util/interp.o util/swap.o util/fswap.o

# creazione del file di swap
$(SWAP): util/start.mk
	truncate -s $(SWAP_SIZE) $(SWAP)

.PHONY: swap clean reset
swap: build/creatimg build/io build/utente $(SWAP)
	build/creatimg $(SWAP) build/io build/user && ln -fs $(SWAP) .swap

clean:
	rm -f system/*.o io/*.o user/*.o util/*.o
	rm -f util/start.mk util/start.gdb util/start.pl

reset: clean
	rm -f build/* $(SWAP) .swap

build:
	mkdir -p $@

user/prog:
	mkdir -p $@

build/mkstart: include/costanti.h util/mkstart.cpp
	g++ -g -Iinclude -o build/mkstart util/mkstart.cpp

util/start.mk: include/costanti.h include/tipo.h build/mkstart
	build/mkstart

util/start.gdb: build/io build/utente
	objdump -h build/io | perl -nae '$$F[1] eq ".text" && do { printf "set \$$START_IO=%u\n",hex($$F[3]);}' > $@
	objdump -h build/utente | perl -nae '$$F[1] eq ".text" && do { printf "set \$$START_UTENTE=%u\n",hex($$F[3]);}' >> $@

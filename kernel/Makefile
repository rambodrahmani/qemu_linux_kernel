################################################################################
# File:	Makefile
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 01/09/2019
################################################################################

LIBCECONF ?= $(firstword $(wildcard /usr/local/etc/libqlk.conf) $(wildcard /etc/libqlk.conf) $(wildcard $(HOME)/QEMU_LINUX_KERNEL/etc/libqlk.conf))
$(if $(LIBCECONF),,$(error libqlk not found))
include $(LIBCECONF)

START_SISTEMA = 0x0000000000200000
SWAP = swap.img
-include util/start.mk

NCC ?= g++
NLD ?= ld.gold

COMM_CFLAGS=$(CFLAGS) \
	-Wall 			\
	-nostdlib		\
	-fno-exceptions 	\
	-fno-rtti 		\
	-fno-stack-protector 	\
	-Iinclude		\
	-I$(INCLUDE)		\
	-mno-red-zone		\
	-g3

COMM_LDFLAGS=\
       -nostdlib

COMM_LDLIBS=

NCFLAGS=$(COMM_CFLAGS) -m64 -Wno-builtin-declaration-mismatch -DX86_64
ifdef DEBUG
	NCFLAGS+=-DDEBUG
endif
NLDFLAGS=$(COMM_LDFLAGS) -melf_x86_64 -L$(LIB64)
NLDLIBS=$(COMM_LDLIBS) -lqlk64

ifdef AUTOCORR
	NCFLAGS+=-DAUTOCORR
endif

all: \
     build/sistema \
     build/parse   \
     build/creatimg \
     utente/prog \
     util/start.gdb

toparse := $(wildcard utente/prog/*)

build/sistema: sistema/sist_s.o sistema/sist_cpp.o
	$(NLD) $(NLDFLAGS) -o build/sistema -Ttext $(START_SISTEMA) sistema/sist_s.o sistema/sist_cpp.o $(NLDLIBS)

build/io: io/io_s.o io/io_cpp.o
	$(NLD) $(NLDFLAGS) -o build/io -Ttext $(START_IO) io/io_s.o io/io_cpp.o $(NLDLIBS)

build/utente: utente/uten_s.o utente/lib.o utente/uten_cpp.o
	$(NLD) $(NLDFLAGS) -o build/utente -Ttext $(START_UTENTE) utente/uten_cpp.o utente/uten_s.o utente/lib.o $(NLDLIBS)

# compilazione di system.s e sistema.cpp
sistema/sist_s.o: sistema/system.s include/constants.h
	$(NCC) $(NCFLAGS) -x assembler-with-cpp -c sistema/system.s -o sistema/sist_s.o

sistema/sist_cpp.o: sistema/sistema.cpp include/constants.h
	$(NCC) $(NCFLAGS) -c sistema/sistema.cpp -o sistema/sist_cpp.o

# compilazione di io.s e io.cpp
io/io_s.o: io/io.s include/constants.h
	$(NCC) $(NCFLAGS) -x assembler-with-cpp -c io/io.s -o io/io_s.o

io/io_cpp.o: io/io.cpp include/constants.h
	$(NCC) $(NCFLAGS) -fpic -c io/io.cpp -o io/io_cpp.o

# compilazione di utente.s e utente.cpp
utente/uten_s.o: utente/utente.s include/constants.h
	$(NCC) $(NCFLAGS) -x assembler-with-cpp -c utente/utente.s -o utente/uten_s.o

utente/utente.cpp: build/parse utente/include/* utente/prog $(toparse)
	build/parse -o utente/utente.cpp $(toparse)

utente/uten_cpp.o: utente/utente.cpp
	$(NCC) $(NCFLAGS) -fpic -Iutente/include -c utente/utente.cpp -o utente/uten_cpp.o

utente/lib.o: utente/lib.cpp utente/include/lib.h
	$(NCC) $(NCFLAGS) -fpic -Iutente/include -c utente/lib.cpp -o utente/lib.o

# creazione di parse e createimg
build/parse: util/parse.c util/src.h
	gcc -o build/parse util/parse.c

util/coff.o: include/constants.h util/interp.h util/coff.h util/dos.h util/coff.cpp
	g++ -c -g -Iinclude -o util/coff.o util/coff.cpp

util/elf32.o:  include/constants.h util/interp.h include/elf.h util/elf32.h util/elf32.cpp
	g++ -c -g -Iinclude -o util/elf32.o util/elf32.cpp

util/elf64.o:  include/constants.h util/interp.h include/elf.h include/elf64.h util/elf64.cpp
	g++ -c -g -Iinclude -o util/elf64.o util/elf64.cpp

util/interp.o: include/constants.h util/interp.h util/interp.cpp
	g++ -c -g -Iinclude -o util/interp.o util/interp.cpp

util/swap.o: include/constants.h util/swap.h util/swap.cpp
	g++ -c -g -Iinclude -o util/swap.o util/swap.cpp

util/fswap.o: include/constants.h util/swap.h util/fswap.cpp
	g++ -c -g -Iinclude -o util/fswap.o util/fswap.cpp

util/creatimg.o: include/constants.h util/interp.h util/swap.h util/creatimg.cpp
	g++ -c -g -Iinclude -o util/creatimg.o util/creatimg.cpp

build/creatimg: util/creatimg.o util/elf32.o util/elf64.o util/coff.o util/interp.o util/swap.o util/fswap.o
	g++ -g -o build/creatimg util/creatimg.o util/elf32.o util/elf64.o util/coff.o util/interp.o util/swap.o util/fswap.o

# creazione del file di swap
$(SWAP): util/start.mk
	truncate -s $(SWAP_SIZE) $(SWAP)

.PHONY: swap clean reset
swap: build/creatimg build/io build/utente $(SWAP)
	build/creatimg $(SWAP) build/io build/utente && ln -fs $(SWAP) .swap

clean:
	rm -f sistema/*.o io/*.o utente/*.o util/*.o
	rm -f util/start.mk util/start.gdb util/start.pl

reset: clean
	rm -f build/* $(SWAP) .swap

build:
	mkdir -p $@

utente/prog:
	mkdir -p $@

build/mkstart: include/constants.h util/mkstart.cpp
	g++ -g -Iinclude -o build/mkstart util/mkstart.cpp

util/start.mk: include/constants.h include/type.h build/mkstart
	build/mkstart

util/start.gdb: build/io build/utente
	objdump -h build/io | perl -nae '$$F[1] eq ".text" && do { printf "set \$$START_IO=%u\n",hex($$F[3]);}' > $@
	objdump -h build/utente | perl -nae '$$F[1] eq ".text" && do { printf "set \$$START_UTENTE=%u\n",hex($$F[3]);}' >> $@


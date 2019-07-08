#!/bin/bash

################################################################################
#   File: compile.sh
#
#  Usage: DO NOT CALL THIS SCRIPT MANUALLY. Use the make command in the parent
#         folder.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019
################################################################################

OUTPUT=${1:-a.out}

case "$OUTPUT" in
    *.cpp|*.s)
        if [ -f "$OUTPUT" ]; then
            echo "`basename $0`: Avoid overriding existing file '$OUTPUT'" > /dev/stderr
            exit 1
        fi
        ;;
    *)
        ;;
esac

COMPILER_OPTIONS="
    -g
    -fno-exceptions
    -fno-rtti
    -fno-pic
    -fno-stack-protector
    -mno-red-zone
    -gdwarf-2
    -I@INCLUDE@
    -Wno-main
    -m64
    -DX86_64
"

ASSEMBLER_OPTIONS="
    -g
    -m64
    -I@INCLUDE@
    -x assembler-with-cpp
"

LINKER_OPTIONS="
    -melf_x86_64
    -nostdlib
    -Ttext 0x200000
    -L@LIB64@
    @LIB64@/start64.o
"

set -e

g++ $COMPILER_OPTIONS -c *.cpp

shopt -s nullglob

for f in *.s; do
	g++ $ASSEMBLER_OPTIONS -c -o ${f%.s}_s.o $f
done

shopt -u nullglob

ld.gold $LINKER_OPTIONS *.o -lqlk64 -o "$OUTPUT"


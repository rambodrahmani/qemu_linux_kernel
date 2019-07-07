#!/bin/bash

################################################################################
#   File: debug.sh
#
#  Usage: DO NOT CALL THIS SCRIPT MANUALLY. Use the make command in the parent
#         folder.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019
################################################################################

EXE=${1:-a.out}

GDBINIT=$(mktemp)

trap "rm '$GDBINIT'" exit

cat <<EOF > "$GDBINIT"
set arch i386:x86-64
file @LIB@/boot.bin
target remote localhost:1234
print wait_for_gdb=0
file $EXE
break main
EOF

gdb -x "$GDBINIT"


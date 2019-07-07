#!/bin/bash

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

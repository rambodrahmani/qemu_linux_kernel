#!/bin/bash

[ -n "$QEMU" ] && {
	[ -x "$QEMU" ] || { 
		[ "$REPORT" != 0 ] && echo "$QEMU non esistente o non eseguibile" >&2
		QEMU=
		REPORT=${REPORT:-1}
	}
}
[ -z "$QEMU" ] && QEMU=@BIN@/qemu-system-x86_64
[ -x "$QEMU" ] || QEMU=$(which qemu-system-x86_64)
[ -x "$QEMU" ] || { echo "qemu non trovato" >&2; exit 1; }

[ "$REPORT" = 1 ] && echo "uso $QEMU" >&2

cmd="\"$QEMU\" \
	-serial stdio \
	-soundhw pcspk \
	-no-reboot \
	-m ${MEM:-16} \
        -drive file=\"@HD_PATH@\",index=0,media=disk,cache=writeback,format=raw"

boot="boot.bin"
exe="a.out"
append=""
for opt
do
	case $opt in
	-g) cmd="$cmd -s -append \"-s\"" ;;
	*)  exe=$opt
	esac
done

if [ -d "$exe" ]; then
		exe="$exe/a.out"
fi

cmd="$cmd -kernel \"@LIB@/$boot\" -initrd \"$exe\""

trap 'stty sane 2>/dev/null' exit

echo Eseguo: $cmd
eval $cmd

# utente.s
#
#include <costanti.h>

	.global _start, start
_start:
start:
	.cfi_startproc
	call lib_init
	jmp main
	.cfi_endproc

# Tipi di interruzione per le chiamate di sistema e per le primitive di IO
#

	.text
	.global activate_p
activate_p:
	.cfi_startproc
	int $TIPO_A
	ret
	.cfi_endproc

	.global terminate_p
terminate_p:
	.cfi_startproc
	int $TIPO_T
	ret
	.cfi_endproc

	.global sem_ini
sem_ini:
	.cfi_startproc
	int $TIPO_SI
	ret
	.cfi_endproc

	.global sem_wait
sem_wait:
	.cfi_startproc
	int $TIPO_W
	ret
	.cfi_endproc

	.global sem_signal
sem_signal:
	.cfi_startproc
	int $TIPO_S
	ret
	.cfi_endproc

	.global delay
delay:
	.cfi_startproc
	int $TIPO_D
	ret
	.cfi_endproc

	.global readse_n
readse_n:
	.cfi_startproc
	int $IO_TIPO_RSEN
	ret
	.cfi_endproc

	.global readse_ln
readse_ln:
	.cfi_startproc
	int $IO_TIPO_RSELN
	ret
	.cfi_endproc

	.global writese_n
writese_n:
	.cfi_startproc
	int $IO_TIPO_WSEN
	ret
	.cfi_endproc

	.global writese_0
writese_0:
	.cfi_startproc
	int $IO_TIPO_WSE0
	ret
	.cfi_endproc

	.global readconsole
readconsole:
	.cfi_startproc
	int $IO_TIPO_RCON
	ret
	.cfi_endproc

	.global writeconsole
writeconsole:
	.cfi_startproc
	int $IO_TIPO_WCON
	ret
	.cfi_endproc

	.global iniconsole
iniconsole:
	.cfi_startproc
	int $IO_TIPO_INIC
	ret
	.cfi_endproc

	.global readhd_n
readhd_n:
	.cfi_startproc
	int $IO_TIPO_HDR
	ret
	.cfi_endproc

	.global writehd_n
writehd_n:
	.cfi_startproc
	int $IO_TIPO_HDW
	ret
	.cfi_endproc

	.global dmareadhd_n
dmareadhd_n:
	.cfi_startproc
	int $IO_TIPO_DMAHDR
	ret
	.cfi_endproc

	.global dmawritehd_n
dmawritehd_n:
	.cfi_startproc
	int $IO_TIPO_DMAHDW
	ret
	.cfi_endproc

# EXTENSION 2017-02-24

#-------------------------------------------------------------------------------
.GLOBAL countres                                   # extern "C" natq countres();
#-------------------------------------------------------------------------------
countres:
    int $TIPO_CRES
    ret

#-------------------------------------------------------------------------------
.GLOBAL cedmaread  # extern "C" bool cedmaread(natl id, natl quanti, char *buf);
#-------------------------------------------------------------------------------
cedmaread:
    int $IO_TIPO_CEDMAREAD
    ret

# EXTENSION 2017-02-24

	.global do_log
do_log:
	.cfi_startproc
	int $TIPO_L
	ret
	.cfi_endproc


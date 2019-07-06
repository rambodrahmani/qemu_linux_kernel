/**
 * File: def.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#define ID_CODE_SYS		1
#define ID_DATA_SYS		2
#define ID_CODE_USR		3
#define ID_DATA_USR		4
#define ID_SYS_TSS		5
#define SEL_CODICE_SISTEMA	(ID_CODE_SYS << 3)
#define SEL_DATI_SISTEMA 	(ID_DATA_SYS << 3)
#define SEL_CODICE_UTENTE	(ID_CODE_USR << 3 | LIV_UTENTE)
#define SEL_DATI_UTENTE 	(ID_DATA_USR << 3 | LIV_UTENTE)
#define SEL_SYS_TSS 		(ID_SYS_TSS  << 3)
#define	PRES			0x80 // 0b10000000
#define NON_P			0x00 // 0b00000000
#define	SEG_CODICE		0x1A // 0b00011010
#define SEG_DATI		0x12 // 0b00010010
#define	SEG_TSS			0x09 // 0b00001001
#define G_PAGINA		0x80 // 0b10000000
#define G_BYTE			0x00 // 0b00000000
#define LIV_UTENTE		3
#define LIV_SISTEMA		0

#define SERV_PCIBIOS32		0x49435024 // $PCI
#define SERV_PCIIRQRT		0x52495024 // $PIR


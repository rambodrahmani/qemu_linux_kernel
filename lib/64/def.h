/**
 * File: def.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#define ID_CODE_SYS		1
#define ID_DATA_SYS		0
#define ID_CODE_USR		2
#define ID_DATA_USR		3
#define ID_SYS_TSS		4
#define SEL_CODICE_SISTEMA	(ID_CODE_SYS << 3)
#define SEL_DATI_SISTEMA 	(ID_DATA_SYS << 3)
#define SEL_CODICE_UTENTE	(ID_CODE_USR << 3)
#define SEL_DATI_UTENTE 	(ID_DATA_USR << 3)
#define SEL_SYS_TSS 		(ID_SYS_TSS  << 3)
#define LIV_UTENTE		3
#define LIV_SISTEMA		0

#define SERV_PCIBIOS32		0x49435024 // $PCI
#define SERV_PCIIRQRT		0x52495024 // $PIR


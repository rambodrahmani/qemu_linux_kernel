// sistema.cpp
//
#include "mboot.h"		// *****
#include "elf64.h"
#define MAX_LOG LOG_ERR
#include "libqlk.h"
#ifdef X86_64
#error "this must be compiled for 32 bit"
#endif

// carica un nuovo valore in cr3 [vedi sistema.S]
extern "C" void loadCR3(addr dir);

// restituisce il valore corrente di cr3 [vedi sistema.S]
extern "C" addr readCR3();

// attiva la paginazione [vedi sistema.S]
extern "C" void attiva_paginazione(natl entry, int debug);

void ini_COM1();

natl carica_modulo(multiboot_module_t* mod) {
	Elf64_Ehdr* elf_h = (Elf64_Ehdr*)mod->mod_start;

	if (!(elf_h->e_ident[EI_MAG0] == ELFMAG0 &&
	      elf_h->e_ident[EI_MAG1] == ELFMAG1 &&
	      elf_h->e_ident[EI_MAG2] == ELFMAG2 &&
	      elf_h->e_ident[EI_MAG2] == ELFMAG2))
	{
		flog(LOG_ERR, "Formato del modulo '%s' non riconosciuto",
			mod->cmdline);
		return 0;
	}

	if (!(elf_h->e_ident[EI_CLASS] == ELFCLASS64  &&
	      elf_h->e_ident[EI_DATA]  == ELFDATA2LSB &&
	      elf_h->e_type	       == ET_EXEC     &&
	      elf_h->e_machine 	       == EM_AMD64))
	{ 
		flog(LOG_ERR, "Il modulo '%s' non contiene "
			"un esegubile per x86_64", mod->cmdline);
		return 0;
	}

	Elf64_Phdr* elf_ph = (Elf64_Phdr*)(mod->mod_start + elf_h->e_phoff);
	for (int i = 0; i < elf_h->e_phnum; i++) {
		if (elf_ph->p_type != PT_LOAD)
			continue;

		memcpy((void*)elf_ph->p_vaddr,
		       (void*)(mod->mod_start + elf_ph->p_offset),
		       elf_ph->p_filesz);
		flog(LOG_INFO, "Copiata sezione di %d byte all'indirizzo %p",
			(long)elf_ph->p_filesz, (void*)elf_ph->p_vaddr);
		memset((void*)(elf_ph->p_vaddr + elf_ph->p_filesz), 0,
		       elf_ph->p_memsz - elf_ph->p_filesz);
		flog(LOG_INFO, "azzerati ulteriori %d byte",
				elf_ph->p_memsz - elf_ph->p_filesz);
		elf_ph = (Elf64_Phdr*)((unsigned int)elf_ph +
			elf_h->e_phentsize);
	}
	flog(LOG_INFO, "entry point %p", elf_h->e_entry);
	return (natl)elf_h->e_entry;
}

static const int MAXARGS = 10;
static int debug_mode = 0;

static void
parse_args(char *cmd)
{
	char *argv[MAXARGS];
	int argc = 0;

	char *scan = cmd;
	do {
		argv[argc] = scan;
		while (*scan && *scan != ' ')
			scan++;
		if (*scan) {
			*scan = '\0';
			for (scan++; *scan && *scan == ' '; scan++)
				;
		}
		flog(LOG_INFO, "argv[%d] = '%s'", argc, argv[argc]);
		argc++;
	} while (*scan && argc <= MAXARGS);

	for (int i = 0; i < argc; i++) {
		if (argv[i][0] != '-')
			continue;
		if (argv[i][1] == 's' && !debug_mode) {
			flog(LOG_INFO, "DEBUG MODE");
			debug_mode = 1;
		}
	}
}

int wait_for_gdb;
addr tab4 = (addr)0x1000;
extern "C" void cmain (natl magic, multiboot_info_t* mbi)
{
	natl entry;
	
	// (* inizializzazione Seriale per il Debugging
	ini_COM1();
	flog(LOG_INFO, "Boot loader Calcolatori Elettronici, v0.01");
	// *)
	
	// (* controlliamo di essere stati caricati
	//    da un bootloader che rispetti lo standard multiboot
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		flog(LOG_ERR, "Numero magico non valido: 0x%x", magic);
	}
	// *)

	if (mbi->flags & MULTIBOOT_INFO_CMDLINE) {
		flog(LOG_INFO, "argomenti: %s", mbi->cmdline);
		parse_args((char *)mbi->cmdline);
	}

	// (* Carichiamo i vari moduli
	//    Il numero dei moduli viene passato dal bootloader in mods_count
	if (!(mbi->flags & MULTIBOOT_INFO_MODS) ||
	      mbi->mods_count != 1)
	{
		flog(LOG_ERR, "Informazioni sui moduli assenti o errate");
		return;
	}

	flog(LOG_INFO, "mods_count = %d, mods_addr = 0x%x",
			mbi->mods_count, mbi->mods_addr);
	multiboot_module_t* mod = (multiboot_module_t*) mbi->mods_addr;
	flog(LOG_INFO, "mod[0]:%s: start 0x%x end 0x%x",
		mod->cmdline, mod->mod_start, mod->mod_end);
	entry = carica_modulo(mod);
	// *)
	
	loadCR3(tab4);
	if (debug_mode)
		flog(LOG_INFO, "Attendo collegamento da gdb...");
	attiva_paginazione(entry, debug_mode);
	
	/* mai raggiunto */
	return;
}
// )






/**
 * File: boot64.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "mboot.h"
#include "elf64.h"
#include "libqlk.h"
#include "internal.h"
#include "log.h"

#define MAX_LOG LOG_ERR

#ifdef X86_64
    #error "this must be compiled for 32 bit"
#endif

/**
 * Loads the given address to the CR3 register.
 *
 * @param   dir   the address to be loaded
 */
extern "C" void loadCR3(addr dir);

/**
 * Reads the address contained in the CR3 register.
 *
 * @return  the retrived address
 */
extern "C" addr readCR3();

// attiva la paginazione [vedi sistema.S]
extern "C" void attiva_paginazione(natl entry, int debug);

/**
 *
 */
static const int MAXARGS = 10;

/**
 *
 */
static int debug_mode = 0;

/**
 *
 */
int wait_for_gdb;

/**
 *
 */
addr tab4 = (addr)0x1000;

/**
 * Multiboot2 Boot modules. [1]
 */
natl carica_modulo(multiboot_module_t* mod)
{
    Elf64_Ehdr* elf_h = (Elf64_Ehdr*)mod->mod_start;

    if (!(elf_h->e_ident[EI_MAG0] == ELFMAG0 &&
          elf_h->e_ident[EI_MAG1] == ELFMAG1 &&
          elf_h->e_ident[EI_MAG2] == ELFMAG2 &&
          elf_h->e_ident[EI_MAG2] == ELFMAG2))
    {
        flog(LOG_ERR, "Formato del modulo '%s' non riconosciuto", mod->cmdline);

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

    for (int i = 0; i < elf_h->e_phnum; i++)
    {
        if (elf_ph->p_type != PT_LOAD)
        {
            continue;
        }

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

/**
 *
 */
static void parse_args(char *cmd)
{
    char *argv[MAXARGS];

    int argc = 0;

    char *scan = cmd;

    do
    {
        argv[argc] = scan;

        while (*scan && *scan != ' ')
        {
            scan++;
        }

        if (*scan)
        {
            *scan = '\0';
            for (scan++; *scan && *scan == ' '; scan++)
                ;
		}

        flog(LOG_INFO, "argv[%d] = '%s'", argc, argv[argc]);

        argc++;
	} while (*scan && argc <= MAXARGS);

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
		{
            continue;
        }

        if (argv[i][1] == 's' && !debug_mode)
        {
            flog(LOG_INFO, "DEBUG MODE");
            debug_mode = 1;
        }
    }
}

/**
 *
 */
extern "C" void cmain (natl magic, multiboot_info_t* mbi)
{
    natl entry;
	
    // intialize serial interface COM1: needed for logging
    init_COM1();

    // print info log message
    flog(LOG_INFO, "QEMU Linux Kernel Boot Loader, v0.01");

    // check if the boot loader supports multiboot2 standard [2]
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        flog(LOG_ERR, "Invalid magic number: 0x%x", magic);
	}

    //
    if (mbi->flags & MULTIBOOT_INFO_CMDLINE)
    {
        flog(LOG_INFO, "parameters: %s", mbi->cmdline);
        parse_args((char *)mbi->cmdline);
    }

	// (* Carichiamo i vari moduli
	//    Il numero dei moduli viene passato dal bootloader in mods_count
	if (!(mbi->flags & MULTIBOOT_INFO_MODS) ||
	      mbi->mods_count != 1)
	{
		flog(LOG_ERR, "Modules data missing or wrong.");
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

    // check if debug mode is enabled
    if (debug_mode)
    {
        // wait for gdb connection
        flog(LOG_INFO, "Waiting for gdb connection...");
    }

    attiva_paginazione(entry, debug_mode);

	// never reached
	return;
}

/* [1]
 * Multiboot2 Boot modules
 * -----------------------
 * Many modern operating system kernels, such as Mach and the microkernel
 * in VSTa, do not by themselves contain enough mechanism to get the
 * system fully operational: they require the presence of additional
 * software modules at boot time in order to access devices, mount file
 * systems, etc. While these additional modules could be embedded in the
 * main OS image along with the kernel itself, and the resulting image be
 * split apart manually by the operating system when it receives control,
 * it is often more flexible, more space-efficient, and more convenient
 * to the operating system and user if the boot loader can load these
 * additional modules independently in the first place.Thus, this
 * specification should provide a standard method for a boot loader to
 * indicate to the operating system what auxiliary boot modules were
 * loaded, and where they can be found. Boot loaders don’t have to
 * support multiple boot modules, but they are strongly encouraged to,
 * because some operating systems will be unable to boot without them.
 */

/* [2]
 * The magic fields of Multiboot2 header
 * -------------------------------------
 * ‘magic’
 *      The field ‘magic’ is the magic number identifying the header, which must
 *      be the hexadecimal value 0xE85250D6.
 *
 * ‘architecture’
 *      The field ‘architecture’ specifies the Central Processing Unit
 *      Instruction Set Architecture. Since ‘magic’ isn’t a palindrome it
 *      already specifies the endianness ISAs differing only in endianness
 *      recieve the same ID. ‘0’ means 32-bit (protected) mode of i386. ‘4’
 *      means 32-bit MIPS.
 *
 * ‘header_length’
 *      The field ‘header_length’ specifies the Length of Multiboot2 header in
 *      bytes including magic fields.
 *
 * ‘checksum’
 *      The field ‘checksum’ is a 32-bit unsigned value which, when added to the
 *      other magic fields (i.e. ‘magic’, ‘architecture’ and ‘header_length’),
 *      must have a 32-bit unsigned sum of zero. 
 */


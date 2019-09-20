/**
 * File: system.cpp
 *       System Module C++ implementation.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 30/08/2019.
 */

#include "constants.h"
#include <libqlk.h>
#include <log.h>
#include <apic.h>

////////////////////////////////////////////////////////////////////////////////
//                                PROCESSES                                   //
////////////////////////////////////////////////////////////////////////////////

/**
 * Maximum process priority.
 */
const natl MAX_PRIORITY	= 0xfffffff;

/**
 * Minimum process priority.
 */
const natl MIN_PRIORITY	= 0x0000001;

/**
 * Dummy processo priority.
 */
const natl DUMMY_PRIORITY = 0x0000000;

/**
 * Number of registers of the contest array field of the des_proc struct.
 */
const int N_REG = 16;

/**
 * Memory Virtual Address.
 */
typedef natq vaddr;

/**
 * Memory Physical Address.
 */
typedef natq faddr;

/**
 *
 */
typedef natq tab_entry;

/**
 * Process Descriptor. Each process has its own process descriptor, a system
 * stack and its own memory (which contains its code, data and user stack).
 * In order to be able to switch between processes and allow for a little
 * parallel execution we will have to take a full snapshot of the system state
 * (CPU, memory, devices etc..) in order to be able to come back to the
 * execution where it has been left.
 */
struct des_proc
{
    // hardware required
    struct __attribute__ ((packed))
    {
        natl riservato1;

        /**
         * Each process has its own system stack and the way the system stack is
         * changed moving from one process to another is up to the hardware
         * interrupt mechanism. We will place this pointer to the process system
         * stack where we know the hardware will look for it. When the process
         * is started, the CPU will save in this stack the pointer to the
         * previous stack, the content of the RFLAGS register, the previous
         * privilege level, and the address of the next instruction to be
         * executed.
         * When a process is at user level its system stack is always empty. The
         * system stack will be filled when moving to the system level and
         * emptied out when returning to user level.
         */
        vaddr system_stack;

        // due quad  a disposizione (puntatori alle pile ring 1 e 2)
        natq disp1[2];
        natq riservato2;

        //entry della IST, non usata
        natq disp2[7];
        natq riservato3;
        natw riservato4;
        natw iomap_base; // si veda crea_processo()
    };

    // custom data
    faddr cr3;

    // process context: contains a copy of the CPU registers content
    natq context[N_REG];

    natl cpl;
};

/**
 * Number of active user processes.
 */
volatile natl user_processes;

/**
 * Destroys the process currently pointed by 'execution' and jumps to a new
 * process. Keep in mind that it won't return to the calling function, but only
 * abort the current process and schedule a new one.
 */
extern "C" void c_abort_p();

/**
 * Returns the process descriptor for the given process id.
 * See system.s.
 *
 * @param  id  the process ID.
 */
extern "C" des_proc *des_p(natl id);

/**
 * Dummy process ID, created during initialization.
 * The dummy process is the process with the lowest priority among the ready
 * processes and it is used to avoid deadlock conditions where no process is
 * available to go under execution. The dummy process, when executed only checks
 * the number of process currently active in the system. If all the process are
 * terminate the dummy process will eventually shutdown the system.
 */
natl dummy_proc;

/**
 * Indexes for the context[] array in the des_proc struct.
 * Each CPU register has its own index.
 */
enum
{
    I_RAX,
    I_RCX,
    I_RDX,
    I_RBX,
    I_RSP,
    I_RBP,
    I_RSI,
    I_RDI,
    I_R8,
    I_R9,
    I_R10,
    I_R11,
    I_R12,
    I_R13,
    I_R14,
    I_R15
};

/**
 * Can be used to represent a process in a queue.
 */
struct proc_elem
{
    /**
     * Every time a process is created an entry in the GDT must be filled with
     * the pointer to the process descriptor. The offset of the GDT entry used
     * will also be used as the process id.
     */
    natl id;

    /**
     * There are many types of process queueing criterias. We will base our
     * processes queues on the processes priority. At any given time the process
     * under execution (pointed by 'execution') is the process with the highest
     * priority.
     */
    natl priority;
    proc_elem *next;
};

/**
 * Current Process under execution at any given time.
 */
proc_elem *execution;

/**
 * List of processes ready to be executed.
 */
proc_elem *ready_proc;

/**
 * Inserts the given process element in the given processes list. The list is
 * ordered based on each process priority. For processes with the same priority
 * the new process will be inserted at the end.
 *
 * @param  p_list  processes list where to insert the given process;
 * @param  p_elem  process to be inserted.
 */
void list_insert(proc_elem *&p_list, proc_elem *p_elem)
{
    proc_elem *pp, *prevp;

    // point to the top of the list
    pp = p_list;

    prevp = 0;

    // loop through the processes list
    while (pp != 0 && pp->priority >= p_elem->priority)
    {
        prevp = pp;
        pp = pp->next;
    }

    // insert in the ordered list
    p_elem->next = pp;

    // check if it is the top of the list
    if (prevp == 0)
	{
        // insert process in the top
        p_list = p_elem;
    }
    else
    {
        // change previous process next element pointer
        prevp->next = p_elem;
    }
}

/**
 * Removes from the given processes list the process having the highest priority
 * and returns it using p_elem.
 * Because the processes list is kept ordered based on each process element
 * priority, the only thing we have to do is to remove the process placed on the
 * top of the list.
 *
 * @param  p_list  the processes list where to remove the process from;
 * @param  p_elem  pointer to the process elem removed from the list.
 */
void list_remove(proc_elem *&p_list, proc_elem *&p_elem)
{
    // copies the top element of the list into p_elem (usually execution)
    p_elem = p_list;

    // chekc if the list is not empty
    if (p_list)
    {
        // remove top element
        p_list = p_list->next;
    }

    // check if the removed element is not 0
    if (p_elem)
    {
        // remove next pointer
        p_elem->next = 0;
    }
}

/**
 * Inserts the process currently under execution at the top of the ready_proc
 * list.
 */
extern "C" void ins_ready_proc()
{
    execution->next = ready_proc;
    ready_proc = execution;
}

/**
 * Generally speacking, each process can be found in one of the following
 * states:
 *  1. execution: the process is currently under execution by the CPU;
 *  2. ready: the process is ready to be executed by the CPU;
 *  3. wait: the process is not ready to be executed by the CPU as it is waiting
 *           for certain conditions to be met.
 *
 * Selects the next process to be placed under execution. Since the processes
 * list is ordered according to priority we can just remove the process on the
 * top of the list.
 */
extern "C" void schedule(void)
{
    // remove process from the top of 'ready_proc' and place it in 'execution'
    list_remove(ready_proc, execution);
}

////////////////////////////////////////////////////////////////////////////////
//                                 SEMAPHORES                                 //
////////////////////////////////////////////////////////////////////////////////
// We will be using sempagores for two main reasons: mutual exclusion (mutex)
// and synchronization. Semaphores are allocated once and never deallocated: the
// assumption we make is that there is a finite number of avilable sempahore
// which can be used by the user program (multiple processes program) to share
// a given area of memory (each process also has its own memory space).
// Each semaphore is descripted by the des_sem struct.

/**
 * Semaphore element descriptor.
 */
struct des_sem
{
    /**
     * When the sempahore is used for mutual exclusion it is a positive number
     * and represents the number of simultaneously active process. Each process
     * receiving the mutex lock must decrement this of 1.
     *
     * When the semaphore is used for synchronization must be initialized to a
     * value lower or equal to 0 (<= 0). In this case one process is waiting for
     * other processes to be activated before executing and each of these
     * processes will increment counter by 1.
     */
    int counter;

    // processes waiting for this semaphore
    proc_elem *pointer;
};

/**
 * Available semaphore descriptors.
 */
des_sem array_dess[MAX_SEM];

/**
 * Initializes a semaphore with the given value for the counter field and
 * returns the initialized semaphore ID.
 */
extern "C" natl sem_ini(int);

/**
 * Checks if the given semaphore ID is valid (belongs to an allocated
 * semaphore).
 */
bool sem_valido(natl sem);

/**
 * Waits for the semaphore identified by the given id.
 */
extern "C" void c_sem_wait(natl sem)
{
    // semaphore pointer
    des_sem *s;

    // check if the given semaphore id is valid
    if (!sem_valido(sem))
    {
        // if not, print a warning log message
        flog(LOG_WARN, "Invalid semaphore: %d", sem);

        // abort current process under execution
        c_abort_p();

        // just return to the calling function
        return;
    }

    // otherwise, retrieve pointer to the specified semaphore
    s = &array_dess[sem];

    // decrease semaphore counter
    (s->counter)--;

    // if the semaphore counter is lower than zero
    if ((s->counter) < 0)
    {
        // insert process in the waiting list
        list_insert(s->pointer, execution);

        // schedule next process
        schedule();
    }
}

/**
 * This method is called by a process to notify all waiting processes that he
 * has unlocked the reference object.
 */
extern "C" void c_sem_signal(natl sem)
{
    // semaphore pointer
    des_sem *s;

    // process pointer
    proc_elem *proc;

    // check if the given semaphore id is valid
    if (!sem_valido(sem))
    {
        // if not, print a warning message to the console
        flog(LOG_WARN, "Invalid semaphore: %d", sem);

        // abort current process under execution
        c_abort_p();

        // just return to the calling function
        return;
    }

    // retrieve semaphore identified by the given id
    s = &array_dess[sem];

    // increment semaphore counter
    (s->counter)++;

    // check if the semaphore counter is below or equal to zero
    if ((s->counter) <= 0)
    {
        // remove a process from the semaphore wait list into 'proc'
        list_remove(s->pointer, proc);

        // preemption: insert current process into the ready processes list
        ins_ready_proc();

        // preemption: insert retrieved process into the ready processes list
        list_insert(ready_proc, proc);

        // schedule: both the current process and the process retrieved from the
        // semaphore wai list must be inserted in the ready processes list and
        // the system must reschedule again in order to make sure that at any
        // given time the process with the highest priority is under execution
        schedule();
    }
}

////////////////////////////////////////////////////////////////////////////////
//                                   TIMER                                    //
////////////////////////////////////////////////////////////////////////////////

/**
 * Timer request.
 */
struct richiesta
{
    natl d_attesa;
    richiesta *p_rich;
    proc_elem *pp;
};

/**
 *
 */
richiesta *p_sospesi;

/**
 *
 */
void inserimento_lista_attesa(richiesta *p);

// parte "C++" della primitiva delay
extern "C" void c_delay(natl n)
{
	richiesta *p;

	p = static_cast<richiesta*>(alloca(sizeof(richiesta)));
	p->d_attesa = n;
	p->pp = execution;

	inserimento_lista_attesa(p);
	schedule();
}

// inserisce P nella coda delle richieste al timer
void inserimento_lista_attesa(richiesta *p)
{
	richiesta *r, *precedente;
	bool ins;

	r = p_sospesi;
	precedente = 0;
	ins = false;

	while (r != 0 && !ins)
    {
		if (p->d_attesa > r->d_attesa) {
			p->d_attesa -= r->d_attesa;
			precedente = r;
			r = r->p_rich;
		} else
			ins = true;
    }

	p->p_rich = r;
	if (precedente != 0)
		precedente->p_rich = p;
	else
		p_sospesi = p;

	if (r != 0)
		r->d_attesa -= p->d_attesa;
}


// (* in caso di errori fatali, useremo la segunte funzione, che blocca il sistema:
extern "C" void panic(cstr msg) __attribute__ (( noreturn ));
// implementazione in [P_PANIC]
// *)

////////////////////////////////////////////////////////////////////////////////
//                                 EXCEPTIONS                                 //
////////////////////////////////////////////////////////////////////////////////

/**
 * Descriptive string for the exceptions.
 *
 * The processor will sometimes need to signal your kernel. Something major may
 * have happened, such as a divide-by-zero, or a page fault. To do this, it uses
 * the first 32 interrupts. It is therefore doubly important that all of these
 * are mapped and non-NULL - else the CPU will triple-fault and reset (bochs
 * will panic with an 'unhandled exception' error).
 */
static const char *exceptions[] =
{
    "Division by zero exception",   // 0
    "Debug exception",              // 1
    "Non maskable interrupt",       // 2
    "Breakpoint exception",         // 3
    "Overflow exception",           // 4
    "Out of bounds exception",      // 5
    "Invalid opcode exception",     // 6
    "No coprocessor exception",     // 7
    "Double fault",                 // 8, pushes an error code
    "Coprocessor segment overrun",  // 9
    "Bad TSS",                      // 10, pushes an error code
    "Segment not present",          // 11, pushes an error code
    "Stack fault",                  // 12, pushes an error code
    "General protection fault",     // 13, pushes an error code
    "Page fault",                   // 14, pushes an error code
    "Unknown interrupt exception",  // 15
    "Coprocessor fault",            // 16
    "Alignment check exception",    // 17
    "Machine check exception",      // 18
    "SIMD exception",               // 19
};

/**
 * Dumps the current process data.
 */
void process_dump(natl id, log_sev sev);

/**
 * All exception are handled in the same way. This function writes a log message
 * containing info about the handled exception, dumps the current process data 
 * which caused the exception data and aborts it.
 */
extern "C" void handle_exception(int type, natq err, addr eip)
{
    // log warning message about the Exception
    flog(LOG_WARN, "Exception %ld, error = %lx, EIP = %p\n", type, err, eip);

    // dump current process info
    process_dump(execution->id, LOG_WARN);

    // abort current process
    c_abort_p();
}

// (*il microprogramma di gestione delle eccezioni di page fault lascia in cima
//   alla pila (oltre ai valori consueti) una parola quadrupla i cui 4 bit meno
//   significativi specificano piu' precisamente il motivo per cui si e'
//   verificato un page fault. Il significato dei bit e' il seguente:
//   - prot: se questo bit vale 1, il page fault si e' verificato per un errore
//   di protezione: il processore si trovava a livello utente e la pagina
//   era di livello sistema (bit US = 0 in una qualunque delle tabelle
//   dell'albero che porta al descrittore della pagina). Se prot = 0, la pagina
//   o una delle tabelle erano assenti (bit P = 0)
//   - write: l'accesso che ha causato il page fault era in scrittura (non
//   implica che la pagina non fosse scrivibile)
//   - user: l'accesso e' avvenuto mentre il processore si trovava a livello
//   utente (non implica che la pagina fosse invece di livello sistema)
//   - res: uno dei bit riservati nel descrittore di pagina o di tabella non
//   avevano il valore richiesto (il bit D deve essere 0 per i descrittori di
//   tabella, e il bit pgsz deve essere 0 per i descrittori di pagina)
struct pf_error
{
	natq prot  : 1;
	natq write : 1;
	natq user  : 1;
	natq res   : 1;
	natq pad   : 60; // bit non significativi
};

/**
 *
 */
bool c_routine_pf();

/**
 *
 */
extern "C" vaddr readCR2();

/**
 *
 */
extern "C" faddr readCR3();

/**
 *
 */
extern "C" natq end;	// ultimo indirizzo del codice sistema (fornito dal collegatore)

bool in_pf = false;	//* true mentre stiamo gestendo un page fault
// (* c_pre_routine_pf() e' la routine che viene chiamata in caso di page
//    fault. Effettua dei controlli aggiuntivi prima di chiamare la
//    routine c_routine_pf() che provvede a caricare le tabelle e pagine
//    mancanti
// *)

extern "C" void c_pre_routine_pf(
		int tipo,		/* 14 */
		pf_error errore,	/* vedi sopra */
		addr rip		/* ind. dell'istruzione che ha causato il fault */
	)
{

	// (* se durante la gestione di un page fault si verifica un altro page fault
	//    c'e' un bug nel modulo sistema.
	if (in_pf) {
		panic("page fault ricorsivo: STOP");
	}
	// *)

	in_pf = true;	//* inizia la gestione del page fault
	// (* il sistema non e' progettato per gestire page fault causati
	//   dalle primitie di nucleo. Se cio' si e' verificato,
	//   si tratta di un bug
	if ((errore.user == 0 && rip < &end)|| errore.res == 1) {
		vaddr v = readCR2();
		flog(LOG_ERR, "PAGE FAULT a %p, rip=%lx", v, rip);
		if (v < DIM_PAGINA)
			flog(LOG_ERR, "Probabile puntatore NULL");
		flog(LOG_ERR, "dettagli: %s, %s, %s, %s",
			errore.prot  ? "protezione"	: "pag/tab assente",
			errore.write ? "scrittura"	: "lettura",
			errore.user  ? "da utente"	: "da sistema",
			errore.res   ? "bit riservato"	: "");
		panic("errore di sistema");
	}
	// *)

	// (* l'errore di protezione non puo' essere risolto: il processo ha
	//    tentato di accedere ad indirizzi proibiti (cioe', allo spazio
	//    sistema)
	if (errore.prot == 1) {
		flog(LOG_WARN, "errore di protezione: rip=%lx, ind=%lx, %s, %s", rip, readCR2(),
			errore.write ? "scrittura"	: "lettura",
			errore.user  ? "da utente"	: "da sistema");
		c_abort_p();
		goto out;
	}

	// (* proviamo a caricare la pagina/tabella. Il caricamento potrebbe
	// ancora fallire perche' abbiamo finito lo spazio nello swap o perche'
	// il processo ha tentato di accedere ad una zona non mappata.)
	if (!c_routine_pf()) {
		vaddr v = readCR2();
		flog(LOG_WARN, "PAGE FAULT a %p, rip=%lx non risolto", v, rip);
		c_abort_p();
	}
out:
	in_pf = false;	//* fine della gestione del page fault
}

////////////////////////////////////////////////////////////////////////////////
//                                  FRAMES                                    //
////////////////////////////////////////////////////////////////////////////////
// frame = physical memory page
// page = virtual memory page

/**
 * This structure describes a memory frame.
 *
 * The entire available memory of the system is devided into two main blocks:
 *  M1: contains data structures and subroutines relative to the operating
 *      system;
 *  M2: contains the available frames (physical pages) which can be used to
 *      store paging tables (always permanent) and the user virtual pages.
 */
struct des_frame
{
    /**
     * Each memory frame can contain 
     * -1 = free
     *  0 = page
     * >0 = table
     */
    int	livello;

    /**
     * Permanent or removable page.
     */
    bool residente;

    /**
     * Frame content owner process ID.
     */
    natl processo;

    /**
     * Statistics counter.
     */
	natl contatore;

	// blocco da cui l'entita' contenuta nel frame era stata caricata
	natq ind_massa;

	// per risparmiare un po' di spazio uniamo due campi che
	// non servono mai insieme:
	// - ind_virtuale serve solo se il frame e' occupato
	// - prossimo_libero serve solo se il frame e' libero
	union {
		// indirizzo virtuale che permette di risalire al
		// descrittore che punta all'entita' contenuta nel
		// frame. Per le pagine si tratta di un qualunque
		// indirizzo virtuale interno alla pagina. Per le
		// tabelle serve un qualunque indirizzo virtuale la
		// cui traduzione passa dalla tabella.
		vaddr	ind_virtuale;
		des_frame*	prossimo_libero;
	};
};

/**
 * Frame descriptors array allocated in M1.
 */
des_frame* vdf;

/**
 * Physical address of the first frame available in the M2 memory.
 */
faddr primo_frame_utile;

/**
 * Number of frames contained in the M2 memory.
 */
natq N_DF;

/**
 * First available frame descriptor pointer.
 */
des_frame* frame_liberi;

// dato un indirizzo di un frame restituisce un puntatore al
// corrispondente descrittore
des_frame* descrittore_frame(faddr indirizzo_frame)
{
	if (indirizzo_frame < primo_frame_utile)
		return 0;
	natq indice = (indirizzo_frame - primo_frame_utile) / DIM_PAGINA;
	if (indice >= N_DF)
		return 0;
	return &vdf[indice];
}

// dato un puntatore ad un descrittore di frameestituisce
// l'indirizzo del primo byte del frame corrispondente
faddr indirizzo_frame(des_frame* df)
{
	natq indice = df - &vdf[0];
	return primo_frame_utile + indice * DIM_PAGINA;
}

// restituisce il piu' piccolo numero maggiore o uguale ad a
// e multiplo di m
natq allinea(natq a, natq m)
{
	return (a + m - 1) & ~(m - 1);
}

/**
 * Called during initialization. All the available memory not yet occupied is
 * used to allocate page frames. A page, memory page, or virtual page is a
 * fixed-length contiguous block of virtual memory. Similarly, a page frame is
 * the smallest fixed-length contiguous block of physical memory into which
 * memory pages are mapped by the operating system.
 */
bool init_des_frame()
{
	faddr vdf_start;
	// L'array di decrittori di frame comincia subito dopo
	// la fine del programma
	vdf_start = allinea(reinterpret_cast<natq>(&end), sizeof(natq));
	// N_DPF e' il numero di frame di cui sara' composta M2.
	// Per calcolarlo dobbiamo tenere conto che ci servira' un des_frame
	// per ogni frame.
	N_DF = (MEM_TOT - vdf_start) / (DIM_PAGINA + sizeof(des_frame));
	// M1 finisce dopo la fine dell'array dpf;
	natq fine_M1 = vdf_start + N_DF * sizeof(des_frame);
	// primo_frame_utile e' il primo frame che inizia dopo la fine di M1
	primo_frame_utile = allinea(fine_M1, DIM_PAGINA);

	// creiamo la lista dei frame liberi, che inizialmente contiene
	// tutti i frame di M2
	vdf = reinterpret_cast<des_frame*>(vdf_start);
	frame_liberi = &vdf[0];
	for (natl i = 0; i < N_DF - 1; i++) {
		vdf[i].livello = -1;
		vdf[i].prossimo_libero = &vdf[i + 1];
	}
	vdf[N_DF - 1].livello = -1;
	vdf[N_DF - 1].prossimo_libero = 0;

	return true;
}

// estrea un frame libero dalla lista, se non vuota
des_frame* alloca_frame_libero()
{
	des_frame* p = frame_liberi;
	if (frame_liberi != 0)
		frame_liberi = frame_liberi->prossimo_libero;
	return p;
}

/**
 * Frees the given memory frame.
 *
 * @param  df  memory descriptor of the frame to be freed.
 */
void rilascia_frame(des_frame* df)
{
    df->livello = -1;
    df->prossimo_libero = frame_liberi;
    frame_liberi = df;
}

/**
 * @param  proc
 * @param  liv
 * @param  ind_virtuale
 */
des_frame* scegli_vittima(natl proc, int liv, vaddr ind_virtuale);

/**
 * @param  proc
 * @param  livello
 * @param  ind_virt
 */
des_frame* alloca_frame(natl proc, int livello, vaddr ind_virt);

////////////////////////////////////////////////////////////////////////////////
//                               MEMORY PAGING                                //
////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
static const natq BIT_SEGNO = (1UL << 47);

/**
 *
 */
static const natq MASCHERA_MODULO = BIT_SEGNO - 1;

// restituisce la versione normalizzata (16 bit piu' significativi uguali al
// bit 47) dell'indirizzo a
static inline vaddr norm(vaddr a)
{
	return (a & BIT_SEGNO) ? (a | ~MASCHERA_MODULO) : (a & MASCHERA_MODULO);
}

// restituisce la dimensione di una regione di livello liv
static inline natq dim_region(int liv)
{
	natq v = 1UL << (liv * 9 + 12);
	return v;
}

// dato un indirizzo 'a', restituisce l'indirizzo della
// regione di livello 'liv' a cui 'a' appartiene
static inline vaddr base(vaddr v, int liv)
{
	natq mask = dim_region(liv) - 1;
	return v & ~mask;
}

// copia 'n' descrittori a partire da quello di indice 'i' dalla
// tabella di indirizzo 'src' in quella di indirizzo 'dst'
void copy_des(faddr src, faddr dst, natl i, natl n)
{
	natq *pdsrc = reinterpret_cast<natq*>(src),
	     *pddst = reinterpret_cast<natq*>(dst);
	for (natl j = i; j < i + n && j < 512; j++)
		pddst[j] = pdsrc[j];
}

// indirizzo virtuale di partenza delle varie zone della memoria
// virtuale dei proceii

const vaddr ini_sis_c = norm(I_SIS_C * dim_region(3)); // sistema condivisa
const vaddr ini_sis_p = norm(I_SIS_P * dim_region(3)); // sistema privata
const vaddr ini_mio_c = norm(I_MIO_C * dim_region(3)); // modulo IO
const vaddr ini_utn_c = norm(I_UTN_C * dim_region(3)); // utente condivisa
const vaddr ini_utn_p = norm(I_UTN_P * dim_region(3)); // utente privata

// indirizzo del primo byte che non appartiene alla zona specificata
const vaddr fin_sis_c = ini_sis_c + dim_region(3) * N_SIS_C;
const vaddr fin_sis_p = ini_sis_p + dim_region(3) * N_SIS_P;
const vaddr fin_mio_c = ini_mio_c + dim_region(3) * N_MIO_C;
const vaddr fin_utn_c = ini_utn_c + dim_region(3) * N_UTN_C;
const vaddr fin_utn_p = ini_utn_p + dim_region(3) * N_UTN_P;

//   ( definiamo alcune costanti utili per la manipolazione dei descrittori
//     di pagina e di tabella. Assegneremo a tali descrittori il tipo "natq"
//     e li manipoleremo tramite maschere e operazioni sui bit.
const natq BIT_P    = 1U << 0; // il bit di presenza
const natq BIT_RW   = 1U << 1; // il bit di lettura/scrittura
const natq BIT_US   = 1U << 2; // il bit utente/sistema(*)
const natq BIT_PWT  = 1U << 3; // il bit Page Wright Through
const natq BIT_PCD  = 1U << 4; // il bit Page Cache Disable
const natq BIT_A    = 1U << 5; // il bit di accesso
const natq BIT_D    = 1U << 6; // il bit "dirty"
const natq BIT_PS   = 1U << 7; // il bit "page size"
const natq BIT_ZERO = 1U << 9; // (* nuova pagina, da azzerare *)

const natq ACCB_MASK  = 0x00000000000000FF; // maschera per il byte di accesso
const natq ADDR_MASK  = 0x7FFFFFFFFFFFF000; // maschera per l'indirizzo
const natq INDMASS_MASK = 0x7FFFFFFFFFFFF000; // maschera per l'indirizzo in mem. di massa
const natq INDMASS_SHIFT = 12;	    // primo bit che contiene l'ind. in mem. di massa

/**
 *
 */
bool  extr_P(tab_entry descrittore)
{ // (
	return (descrittore & BIT_P); // )
}

/**
 *
 */
bool extr_D(tab_entry descrittore)
{ // (
	return (descrittore & BIT_D); // )
}

/**
 *
 */
bool extr_A(tab_entry descrittore)
{ // (
	return (descrittore & BIT_A); // )
}

/**
 *
 */
bool extr_ZERO(tab_entry descrittore)
{ // (
	return (descrittore & BIT_ZERO); // )
}

/**
 *
 */
faddr extr_IND_FISICO(tab_entry descrittore)
{ // (
	return descrittore & ADDR_MASK; // )
}

/**
 *
 */
natq extr_IND_MASSA(tab_entry descrittore)
{ // (
	return (descrittore & INDMASS_MASK) >> INDMASS_SHIFT; // )
}

/**
 *
 */
void set_P(tab_entry& descrittore, bool bitP)
{ // (
	if (bitP)
		descrittore |= BIT_P;
	else
		descrittore &= ~BIT_P; // )
}

/**
 *
 */
void set_A(tab_entry& descrittore, bool bitA)
{ // (
	if (bitA)
		descrittore |= BIT_A;
	else
		descrittore &= ~BIT_A; // )
}

/**
 *
 */
void set_ZERO(tab_entry& descrittore, bool bitZERO)
{
	if (bitZERO)
		descrittore |= BIT_ZERO;
	else
		descrittore &= ~BIT_ZERO;
}

// (* definiamo anche la seguente funzione:
//    clear_IND_M: azzera il campo M (indirizzo in memoria di massa)
void clear_IND_MASSA(tab_entry& descrittore)
{
	descrittore &= ~INDMASS_MASK;
}

/**
 *
 */
void  set_IND_FISICO(tab_entry& descrittore, faddr ind_fisico) //
{ // (
	clear_IND_MASSA(descrittore);
	descrittore |= ind_fisico & ADDR_MASK; // )
}

/**
 *
 */
void set_IND_MASSA(tab_entry& descrittore, natq ind_massa)
{ // (
	clear_IND_MASSA(descrittore);
	descrittore |= (ind_massa << INDMASS_SHIFT); // )
}

/**
 *
 */
void set_D(tab_entry& descrittore, bool bitD)
{
	if (bitD)
		descrittore |= BIT_D;
	else
		descrittore &= ~BIT_D;
}

/**
 *
 */
bool  extr_PS(tab_entry descrittore)
{
	return (descrittore & BIT_PS);
}

// dato un indirizzo virtuale 'ind_virt' ne restituisce
// l'indice del descrittore corrispondente nella tabella di livello 'liv'
int i_tab(vaddr ind_virt, int liv)
{
	int shift = 12 + (liv - 1) * 9;
	natq mask = 0x1ffUL << shift;
	return (ind_virt & mask) >> shift;
}

// dato l'indirizzo di una tabella e un indice, restituisce un
// riferimento alla corrispondente entrata
tab_entry& get_entry(faddr tab, natl index)
{
	tab_entry *pd = reinterpret_cast<tab_entry*>(tab);
	return  pd[index];
}

// dato un identificatore di processo, un livello e
// un indirizzo virtuale 'ind_virt', restituisce un riferimento
// all'entrata di quel livello relativo alla pagina che
// contiene 'ind_virt' (tutte le tabelle di livello
// precedente devono essere gia' presenti)
tab_entry& get_des(natl processo, int livello, vaddr ind_virt)
{
	des_proc *p = des_p(processo);
	if (!p) {
		flog(LOG_ERR, "get_des(%d): processo non trovato", processo);
		panic("errore interno");
	}
	faddr tab = p->cr3;
	for (int i = 4; i > livello; i--) {
		tab_entry e = get_entry(tab, i_tab(ind_virt, i));
		if (!extr_P(e))
			panic("P=0 non ammesso");
		tab = extr_IND_FISICO(e);
	}
	return get_entry(tab, i_tab(ind_virt, livello));
}

/**
 * Loads the given address in the CPU CR3 register.
 *
 * @param  dir  the address to be loaded.
 */
extern "C" void loadCR3(faddr dir);

/**
 * Reads and returns the current address contained in the CPU CR3 register.
 */
extern "C" faddr readCR3();

//invalida il TLB
extern "C" void invalida_TLB();

// mappa la memoria fisica in memoria virtuale, inclusa l'area PCI
// (copiamo la finestra gia' creata dal boot loader)
bool crea_finestra_FM(faddr tab4)
{
	faddr boot_dir = readCR3();
	copy_des(boot_dir, tab4, I_SIS_C, N_SIS_C);
	return true;
}

/**
 *
 */
const natl MAX_IRQ  = 24;

/**
 *
 */
proc_elem *a_p[MAX_IRQ];

/**
 *
 */
natq alloca_blocco();

/**
 *
 */
des_frame* swap(natl proc, int livello, vaddr ind_virt);

/**
 *
 */
bool crea(natl proc, vaddr ind_virt, int liv, natl priv)
{
	tab_entry& dt = get_des(proc, liv + 1, ind_virt);
	bool bitP = extr_P(dt);
	if (!bitP) {
		natl blocco = extr_IND_MASSA(dt);
		if (!blocco) {
			if (! (blocco = alloca_blocco()) ) {
				flog(LOG_ERR, "swap pieno");
				return false;
			}
			set_IND_MASSA(dt, blocco);
			set_ZERO(dt, true);
			dt = dt | BIT_RW;
			if (priv == LEV_USER) dt = dt | BIT_US;
		}
		if (liv > 0) {
			des_frame *df = swap(proc, liv, ind_virt);
			if (!df) {
				flog(LOG_ERR, "swap(%d, %d, %p) fallita",
					proc, liv, ind_virt);
				return false;
			}
			df->residente = (priv == LEV_SYSTEM);
		}
	}
	return true;
}

/**
 *
 */
bool crea_pagina(natl proc, vaddr ind_virt, natl priv)
{
	for (int i = 3; i >= 0; i--) {
		if (!crea(proc, ind_virt, i, priv))
			return false;
	}
	return true;
}

/**
 *
 */
bool crea_pila(natl proc, vaddr bottom, natq size, natl priv)
{
	size = allinea(size, DIM_PAGINA);

	for (vaddr ind = bottom - size; ind != bottom; ind += DIM_PAGINA)
		if (!crea_pagina(proc, ind, priv))
			return false;
	return true;
}

/**
 *
 */
faddr carica_pila_sistema(natl proc, vaddr bottom, natq size)
{
	des_frame *dp = 0;
	for (vaddr ind = bottom - size; ind != bottom; ind += DIM_PAGINA) {
		dp = swap(proc, 0, ind);
		if (!dp)
			return 0;
		dp->residente = true;
	}
	return indirizzo_frame(dp) + DIM_PAGINA;
}

/**
 *
 */
faddr crea_tab4()
{
	des_frame* df = alloca_frame_libero();
	if (df == 0) {
		flog(LOG_ERR, "Impossibile allocare tab4");
		panic("errore");
	}
	df->livello = 4;
	df->residente = true;
	df->processo = execution->id;
	faddr tab4 = indirizzo_frame(df);
	memset(reinterpret_cast<void *>(tab4), 0, DIM_PAGINA);

	return tab4;
}

/**
 * Allocates a TSS for the given process descriptor.
 */
extern "C" natl allocate_tss(des_proc*);

/**
 *
 */
extern "C" void rilascia_tss(int indice);

/**
 *
 */
const natl BIT_IF = 1L << 9;

/**
 * Retrieves a valid process id using the given process TSS offset.
 *
 * @param  tss_off  Process TSS offset.
 */
extern "C" natl tss_to_id(natl tss_off);

/**
 * Returns the TSS offset for the given process ID.
 *
 * @param  id  process ID.
 */
extern "C" natl id_to_tss(natl id);

/**
 *
 */
void crea_tab4(faddr dest)
{
	faddr pdir = readCR3();

	memset(reinterpret_cast<void*>(dest), 0, DIM_PAGINA);

	copy_des(pdir, dest, I_SIS_C, N_SIS_C);
	copy_des(pdir, dest, I_MIO_C, N_MIO_C);
	copy_des(pdir, dest, I_UTN_C, N_UTN_C);
}

/**
 *
 */
void rilascia_tutto(faddr tab4, natl i, natl n);

/**
 * Creates a new process with the given parameters.
 * For each process a des_proc, proc_elem and system stack must be allocated.
 * Once a process has been initialized it will be inserted in the ready_proc
 * queue and will eventually be scheduled and executed sooner or later. The
 * execution will take place when the process is pointed by 'execution' which
 * will result in a call to the load_state and a final iretq.
 *
 * @param  f     function address for the process %RIP register; for
 *               simplicity we will assume that the process body is
 *               represented by a void function having an integer parameter
 *               only;
 * @param  a     f function integer parameter;
 * @param  prio  process priority level;
 * @param  liv   process privilege level;
 * @param  IF
 *
 * @return a pointer to the newly created process.
 */
proc_elem* crea_processo(void f(int), int a, int prio, char liv, bool IF)
{
    // new process element
    proc_elem *p;

    // tss offset for the gdt
    natl tss_off;

    // process identifier
    natl identifier;

    // process descriptor
    des_proc *pdes_proc;

    // process tab4
    des_frame* dpf_tab4;

    // system stack
    faddr pila_sistema;

	// allocate a new process descriptor
    pdes_proc = static_cast<des_proc*>(alloca(sizeof(des_proc)));

    // check if the space was correctly allocated
    if (pdes_proc == 0)
    {
        // otherwise go to error #1
        goto error1;
    }

    // zero out the process descriptor
    memset(pdes_proc, 0, sizeof(des_proc));

    // allocate TSS for the process and save the tss offset
	tss_off = allocate_tss(pdes_proc);

    // check if the tss offset is valid
    if (tss_off == 0)
    {
        // otherwise go to error #2
        goto error2;
    }

    // retrieve process id using process tss offset
    identifier = tss_to_id(tss_off);

    // allocate a new proc_elem
    p = static_cast<proc_elem*>(alloca(sizeof(proc_elem)));

    // check if the proc_elem was correctly allocated
    if (p == 0)
    {
        // otherwise go to error #3
        goto error3;
    }

    // set process identifier
    p->id = identifier;

    // set process priority
    p->priority = prio;

    // set process next element
    p->next = 0;

    // process tab4 creation: each process has its own level 4 table
    dpf_tab4 = alloca_frame(p->id, 4, 0);

    // check if the process tab4 was correctly allocated
    if (dpf_tab4 == 0)
    {
        // otherwise go to error #4
        goto error4;
    }

    // set table level
    dpf_tab4->livello = 4;

    // make table persistent in physical memory
    dpf_tab4->residente = true;

    // set table process
    dpf_tab4->processo = identifier;

    // set process cr3 address: this must be loaded in the CPU CR3 register
    // evereytime a process switch occurs
    pdes_proc->cr3 = indirizzo_frame(dpf_tab4);
    crea_tab4(pdes_proc->cr3);

    // create process system stack
    if (!crea_pila(p->id, fin_sis_p, DIM_SYS_STACK, LEV_SYSTEM))
    {
        // in case of errors go to error #5
        goto error5;
    }

    pila_sistema = carica_pila_sistema(p->id, fin_sis_p, DIM_SYS_STACK);
    if (pila_sistema == 0)
    {
        goto error6;
    }

    // check if the given process level is USER
    if (liv == LEV_USER)
    {
        // if so, allocate system stack
		natq* pl = reinterpret_cast<natq*>(pila_sistema);

        // set RIP: execution will continue from here when the process is
        // scheduled
		pl[-5] = reinterpret_cast<natq>(f);

        // 
		pl[-4] = SEL_CODICE_UTENTE;	    // CS (codice utente)

        //
		pl[-3] = IF ? BIT_IF : 0;	    // RFLAGS

        //
		pl[-2] = fin_utn_p - sizeof(natq);  // RSP

        //
		pl[-1] = SEL_DATI_UTENTE;	    // SS (pila utente)

        // create user stack
        if (!crea_pila(p->id, fin_utn_p, DIM_USR_STACK, LEV_USER))
        {
            flog(LOG_WARN, "User stack creation failed.");
            goto error6;
		}

		// ( infine, inizializziamo il descrittore di processo
		//   indirizzo del bottom della pila sistema, che verra' usato
		//   dal meccanismo delle interruzioni
		pdes_proc->system_stack = fin_sis_p;

		//   inizialmente, il processo si trova a livello sistema, come
		//   se avesse eseguito una istruzione INT, con la pila sistema
		//   che contiene le 5 parole lunghe preparate precedentemente
		pdes_proc->context[I_RSP] = fin_sis_p - 5 * sizeof(natq);

        // set function f (RIP) parameter
        pdes_proc->context[I_RDI] = a;

		//pdes_proc->context[I_FPU_CR] = 0x037f;
		//pdes_proc->context[I_FPU_TR] = 0xffff;

        // set current privilege level to user level
        pdes_proc->cpl = LEV_USER;
	
		//   il campo iomap_base contiene l'offset (nel TSS) dell'inizio 
		//   della "I/O bitmap". Questa bitmap contiene un bit per ogni
		//   possibile indirizzo di I/O. Le istruzioni in e out eseguite
		//   da livello utente verranno permesse se il bit corrispondente
		//   all'indirizzo di I/O a cui si riferiscono vale 1.
		//   Per disattivare questo meccanismo dobbiamo inizializzare
		//   il campo iomap_base con un offset maggiore o uguale
		//   della dimensione del segmento TSS (come scritta nel
		//   descrittore di segmento TSS nella GDT, vedere 'set_entry_tss'
		//   in sistema.S)
		pdes_proc->iomap_base = DIM_DESP;

		//   tutti gli altri campi valgono 0
		// )
	} else {
		// ( inizializzazione della pila sistema
		natq* pl = reinterpret_cast<natq*>(pila_sistema);
		pl[-6] = reinterpret_cast<natq>(f);  	// RIP (codice sistema)
		pl[-5] = SEL_CODICE_SISTEMA;            // CS (codice sistema)
		pl[-4] = IF ? BIT_IF : 0;  	        // RFLAGS
		pl[-3] = fin_sis_p - sizeof(natq);	// RSP
		pl[-2] = 0;			        // SS
		pl[-1] = 0;			        // ind. rit.
							//(non significativo)
		//   i processi esterni lavorano esclusivamente a livello
		//   sistema. Per questo motivo, prepariamo una sola pila (la
		//   pila sistema)
		// )

		// ( inizializziamo il descrittore di processo
		pdes_proc->context[I_RSP] = fin_sis_p - 6 * sizeof(natq);
		pdes_proc->context[I_RDI] = a;

		//pdes_proc->context[I_FPU_CR] = 0x037f;
		//pdes_proc->context[I_FPU_TR] = 0xffff;

        // set current privilege level to system level
        pdes_proc->cpl = LEV_SYSTEM;

		//   tutti gli altri campi valgono 0
		// )
	}

    // return newly created process descriptor
    return p;

//
error6:	rilascia_tutto(indirizzo_frame(dpf_tab4), I_SIS_P, N_SIS_P);

//
error5:	rilascia_frame(dpf_tab4);

//
error4:	dealloca(p);

//
error3:	rilascia_tss(tss_off);

//
error2:	dealloca(pdes_proc);

// just return
error1:	return 0;
}

/**
 * C++ implementation for a_activate_p defined in system/system.s.
 * User Modile Primitive activate_p().
 * Copies the given process from the swap partition to the M2 memory space.
 */
extern "C" void c_activate_p(void f(int), int a, natl prio, natl liv)
{
    // new proc_elem
    proc_elem *p;

    // id to be returned in case of failure
	natl id = 0xFFFFFFFF;

    // a priority lower than the one of the dummy process or higher than the one
    // of the calling process can not be used
    if (prio < MIN_PRIORITY || prio > execution->priority)
    {
        // print warning log message
        flog(LOG_WARN, "Invalid process priority: %d", prio);

        // abort current process
        c_abort_p();

        // just return to the calling function
        return;
    }
	// *)

	// (* controlliamo che 'liv' contenga un valore ammesso
	//    [segnalazione di E. D'Urso]
	if (liv != LEV_USER && liv != LEV_SYSTEM) {
		flog(LOG_WARN, "livello non valido: %d", liv);
		c_abort_p();
		return;
	}
	// *)

	if (liv == LEV_SYSTEM && des_p(execution->id)->cpl == LEV_USER) {
		flog(LOG_WARN, "errore di protezione");
		c_abort_p();
		return;
	}

	// (* accorpiamo le parti comuni tra c_activate_p e c_activate_pe
	// nella funzione ausiliare crea_processo
	// (questa svolge, tra l'altro, i punti 1-3 in)
	p = crea_processo(f, a, prio, liv, (liv == LEV_USER));
	// *)

	if (p != 0) {
		list_insert(ready_proc, p);
		user_processes++;
		id = p->id;			// id del processo creato
						// (allocato da crea_processo)
		flog(LOG_INFO, "proc=%d entry=%p(%d) prio=%d liv=%d", id, f, a, prio, liv);
	}

	des_proc *self = des_p(execution->id);
	self->context[I_RAX] = id;
}


/**
 * USER-PRIMITIVE-EXAMPLE.
 * C++ implementation for the a_getid assembly subroutine.
 */
extern "C" natl c_getid()
{
    return execution->id;
}

/**
 *
 */
void rilascia_tutto(addr tab4, natl i, natl n);

/**
 *
 */
void riassegna_tutto(natl proc, faddr tab4, natl i, natl n);

/**
 *
 */
void dealloca_blocco(natl blocco);

// rilascia tutte le strutture dati private associate al processo puntato da
// "p" (tranne il proc_elem puntato da "p" stesso)
faddr ultimo_terminato;

/**
 *
 */
extern "C" void distruggi_pila_precedente()
{
	rilascia_tutto(ultimo_terminato, I_SIS_P, N_SIS_P);
	rilascia_frame(descrittore_frame(ultimo_terminato));
	ultimo_terminato = 0;
}

/**
 * Destroys the given process.
 *
 * @param  p  the process to be destroyed.
 */
void destroy_process(proc_elem* p)
{
    des_proc* pdes_proc = des_p(p->id);

    faddr tab4 = pdes_proc->cr3;

    riassegna_tutto(p->id, tab4, I_MIO_C, N_MIO_C);

    riassegna_tutto(p->id, tab4, I_UTN_C, N_UTN_C);

    rilascia_tutto(tab4, I_UTN_P, N_UTN_P);

    ultimo_terminato = tab4;

    if (p != execution)
    {
        distruggi_pila_precedente();
    }

    rilascia_tss(id_to_tss(p->id));

    dealloca(pdes_proc);
}

// rilascia ntab tabelle (con tutte le pagine da esse puntate) a partire da
// quella puntata dal descrittore i-esimo di tab4.
void rilascia_ric(faddr tab, int liv, natl i, natl n)
{
	for (natl j = i; j < i + n && j < 512; j++) {
		tab_entry& dt = get_entry(tab, j);
		natl blocco;
		if (extr_P(dt)) {
			faddr sub = extr_IND_FISICO(dt);
			if (liv > 1)
				rilascia_ric(sub, liv - 1, 0, 512);
			des_frame *df = descrittore_frame(sub);
			blocco = df->ind_massa;
			rilascia_frame(df);
		} else {
			blocco = extr_IND_MASSA(dt);
		}
		dealloca_blocco(blocco);
		dt = 0;
	}
}

/**
 *
 */
void rilascia_tutto(faddr tab4, natl i, natl n)
{
	rilascia_ric(tab4, 4, i, n);
}

/**
 *
 */
void riassegna_ric(natl proc, faddr tab, int liv, natl i, natl n)
{
    for (natl j = i; j < i + n && j < 512; j++)
    {
        tab_entry& dt = get_entry(tab, j);
        
        if (extr_P(dt))
        {
            faddr sub = extr_IND_FISICO(dt);
            
            if (liv > 1)
            {
                riassegna_ric(proc, sub, liv - 1, 0, 512);
            }

            des_frame *df = descrittore_frame(sub);
            
            if (df->processo == proc)
            {
                df->processo = dummy_proc;
            }
        }
    }
}

/**
 *
 */
void riassegna_tutto(natl proc, faddr tab4, natl i, natl n)
{
	riassegna_ric(proc, tab4, 4, i, n);
}

/**
 * Aborts the current process in execution.
 *
 * @param  sev  log message severity;
 * @param  mode
 */
void term_cur_proc(log_sev sev, const char *mode)
{
    // retrive current process
    proc_elem *p = execution;

    // destroy the current process
    destroy_process(p);

    // descrease active user processes counter
    user_processes--;

    // print log message
    flog(sev, "Process %d %s", p->id, mode);

    // dealloc memory space
    dealloca(p);

    // schedule next process
    schedule();
}

// parte "C++" della terminate_p
extern "C" void c_terminate_p()
{
	term_cur_proc(LOG_INFO, "aborted");
}

/**
 * Aborts the process currently pointed by 'execution'. A new process will be
 * scheduled without returning to the calling function.
 *
 * The only difference with c_terminate_p is that an additionally warning log is
 * also sent whem using this method. Must be used when a process is aborted as a
 * result of an occurring error.
 */
extern "C" void c_abort_p()
{
	term_cur_proc(LOG_WARN, "Current execution process aborted.");
}

// driver del timer
extern "C" void c_driver_td(void)
{
	richiesta *p;

	ins_ready_proc();

	if (p_sospesi != 0) {
		p_sospesi->d_attesa--;
	}

	while (p_sospesi != 0 && p_sospesi->d_attesa == 0) {
		list_insert(ready_proc, p_sospesi->pp);
		p = p_sospesi;
		p_sospesi = p_sospesi->p_rich;
		dealloca(p);
	}

    // schedule next process
    schedule();
}

/**
 *
 */
void scrivi_swap(addr src, natl blocco);

/**
 *
 */
void leggi_swap(addr dest, natl blocco);

/**
 *
 */
void carica(des_frame* df)
{
	tab_entry& e = get_des(df->processo, df->livello + 1, df->ind_virtuale);
	if (extr_ZERO(e)) {
		memset((addr)indirizzo_frame(df), 0, DIM_PAGINA);
	} else {
		leggi_swap((addr)indirizzo_frame(df), df->ind_massa);
	}
}

/**
 *
 */
void scarica(des_frame* df)
{
	scrivi_swap((addr)indirizzo_frame(df), df->ind_massa);
	tab_entry& e = get_des(df->processo, df->livello + 1, df->ind_virtuale);
	set_D(e, false);
}

/**
 *
 */
void collega(des_frame *df)
{
	tab_entry& e = get_des(df->processo, df->livello + 1, df->ind_virtuale);
	set_IND_FISICO(e, indirizzo_frame(df));
	set_P(e, true);
	set_D(e, false);
	set_A(e, false);
}

/**
 *
 */
extern "C" void invalida_entrata_TLB(vaddr ind_virtuale);

/**
 *
 */
bool scollega(des_frame* df)
{
	bool bitD;
	tab_entry& e = get_des(df->processo, df->livello + 1, df->ind_virtuale);
	bitD = extr_D(e);
	bool occorre_salvare = bitD || df->livello > 0;
	set_IND_MASSA(e, df->ind_massa);
	set_P(e, false);
	if (occorre_salvare)
		set_ZERO(e, false);
	if (df->processo == execution->id)
		invalida_entrata_TLB(df->ind_virtuale);
	return occorre_salvare;	//
}

// alloca un frame destinato a contenere l'entita' del
// livello specificato, relativa all'indirizzo virtuale ind_virt
// nello spazio di indirizzamento di proc
des_frame* alloca_frame(natl proc, int livello, vaddr ind_virt)
{
	des_frame *df = alloca_frame_libero();
	if (df == 0) {
		df = scegli_vittima(proc, livello, ind_virt);
		if (df == 0)
			return 0;
		bool occorre_salvare = scollega(df);
		if (occorre_salvare)
			scarica(df);
	}
	return df;
}

// carica l'entita' del livello specificato, relativa all'indirizzo virtuale
// ind_virt nello spazio di indirizzamento di proc
des_frame* swap(natl proc, int livello, vaddr ind_virt)
{
	tab_entry e = get_des(proc, livello + 1, ind_virt);
	natq m = extr_IND_MASSA(e);
	if (!m) {
		flog(LOG_WARN,
		     "indirizzo %p fuori dallo spazio virtuale allocato",
		     ind_virt);
		return 0;
	}
	des_frame* df = alloca_frame(proc, livello, ind_virt);
	if (!df) {
		flog(LOG_WARN, "memoria esaurita");
		return 0;
	}
	df->livello = livello;
	df->residente = 0;
	df->processo = proc;
	df->ind_virtuale = ind_virt;
	df->ind_massa = m;
	df->contatore = 0;
	carica(df);
	collega(df);
	return df;
}

/**
 *
 */
void stat();

/**
 *
 */
bool c_routine_pf()
{
	vaddr ind_virt = readCR2();
	natl proc = execution->id;

	stat();

	for (int i = 3; i >= 0; i--) {
		tab_entry d = get_des(proc, i + 1, ind_virt);
		bool bitP = extr_P(d);
		if (!bitP) {
			des_frame *df = swap(proc, i, ind_virt);
			if (!df)
				return false;
		}
	}
	return true;
}

/**
 *
 */
bool vietato(des_frame* df, natl proc, int liv, vaddr ind_virt)
{
	if (df->livello > liv && df->processo == proc &&
	    base(df->ind_virtuale, df->livello) == base(ind_virt, df->livello))
		return true;
	return false;
}

/**
 *
 */
des_frame* scegli_vittima(natl proc, int liv, vaddr ind_virt)
{
	des_frame *df, *df_vittima;
	df = &vdf[0];
	while ( df < &vdf[N_DF] &&
		(df->residente ||
		 vietato(df, proc, liv, ind_virt)))
		df++;
	if (df == &vdf[N_DF]) return 0;
	df_vittima = df;
	for (df++; df < &vdf[N_DF]; df++) {
		if (df->residente ||
		    vietato(df, proc, liv, ind_virt))
			continue;
		if (df->contatore < df_vittima->contatore ||
		    (df->contatore == df_vittima->contatore &&
		     df_vittima->livello > df->livello))
			df_vittima = df;
	}
	return df_vittima;
}

/**
 *
 */
void stat()
{
	des_frame *df1, *df2;
	faddr f1, f2;
	bool bitA;

	for (natq i = 0; i < N_DF; i++) {
		df1 = &vdf[i];
		if (df1->livello < 1)
			continue;
		f1 = indirizzo_frame(df1);
		for (int j = 0; j < 512; j++) {
			tab_entry& des = get_entry(f1, j);
			if (!extr_P(des))
				continue;
			bitA = extr_A(des);
			set_A(des, false);
			f2 = extr_IND_FISICO(des);
			df2 = descrittore_frame(f2);
			if (!df2 || df2->residente)
				continue;
			df2->contatore >>= 1;
			if (bitA)
				df2->contatore |= 0x80000000;
		}
	}
	invalida_TLB();
}

// funzione di supporto per carica_tutto()
bool carica_ric(natl proc, faddr tab, int liv, vaddr ind, natl n)
{
	natq dp = dim_region(liv);

	natl i = i_tab(ind, liv + 1);
	for (natl j = i; j < i + n; j++, ind += dp) {
		tab_entry e = get_entry(tab, j);
		if (!extr_IND_MASSA(e))
			continue;
		des_frame *df = swap(proc, liv, ind);
		if (!df) {
			flog(LOG_ERR, "impossibile caricare pagina virtuale %p", ind);
			return false;
		}
		df->residente = true;
		if (liv > PRELOAD_LEVEL &&
				!carica_ric(proc, indirizzo_frame(df), liv - 1, ind, 512))
			return false;
	}
	return true;
}

// carica e rende residenti tutte le pagine e tabelle allocate nello swap e
// relative alle entrate della tab4 del processo proc che vanno da i (inclusa)
// a i+n (esclusa)
bool carica_tutto(natl proc, natl i, natl n)
{
	des_proc *p = des_p(proc);

	return carica_ric(proc, p->cr3, 3, norm(i * dim_region(3)), n);
}

// super blocco (vedi e [P_SWAP] avanti)
struct superblock_t
{
	char	magic[8];
	natq	bm_start;
	natq	blocks;
	natq	directory;
	void	(*user_entry)(int);
	natq	user_end;
	void	(*io_entry)(int);
	natq	io_end;
	int	checksum;
};

// descrittore di swap (vedi [P_SWAP] avanti)
struct des_swap
{
	natl *free;		// bitmap dei blocchi liberi
	superblock_t sb;	// contenuto del superblocco
} swap_dev; 	// c'e' un unico oggetto swap

/**
 *
 */
bool swap_init();

// chiamata in fase di inizializzazione, carica in memoria fisica
// tutte le parti condivise di livello IO e utente.
bool crea_spazio_condiviso()
{
	// ( lettura del direttorio principale dallo swap
	flog(LOG_INFO, "lettura del direttorio principale...");
	addr tmp = alloca(DIM_PAGINA);
	if (tmp == 0) {
		flog(LOG_ERR, "memoria insufficiente");
		return false;
	}
	leggi_swap(tmp, swap_dev.sb.directory);
	// )

	// (  carichiamo le parti condivise nello spazio di indirizzamento del processo
	//    dummy
	faddr dummy_dir = des_p(dummy_proc)->cr3;
	copy_des((faddr)tmp, dummy_dir, I_MIO_C, N_MIO_C);
	copy_des((faddr)tmp, dummy_dir, I_UTN_C, N_UTN_C);
	dealloca(tmp);

	if (!carica_tutto(dummy_proc, I_MIO_C, N_MIO_C))
		return false;
	if (!carica_tutto(dummy_proc, I_UTN_C, N_UTN_C))
		return false;
	// )

	// ( copiamo i descrittori relativi allo spazio condiviso anche nel direttorio
	//   corrente, in modo che vengano ereditati dai processi che creeremo in seguito
	faddr my_dir = des_p(execution->id)->cr3;
	copy_des(dummy_dir, my_dir, I_MIO_C, N_MIO_C);
	copy_des(dummy_dir, my_dir, I_UTN_C, N_UTN_C);
	// )

	invalida_TLB();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//                              INITIALIZATION                                //
////////////////////////////////////////////////////////////////////////////////

/**
 * 1 MiB Heap Memory Space.
 */
const natq HEAP_START = 1*MiB;

/**
 * Heap memory space start address.
 */
extern "C" natq start;

/**
 * Heap memory space size.
 */
const natq HEAP_SIZE = (natq)&start - HEAP_START;

/**
 * Initial process.
 */
proc_elem init;

/**
 *
 */
extern "C" void end_program();

/**
 * Dummy process body.
 */
void dd(int i)
{
    // wait until there is only one active user process (the dummy process)
    while (user_processes != 1)
    {}

    // shutdown
    end_program();
}

/**
 * Creates the dummy process. Called at the end of initialization in  cmain().
 *
 * @return the dummy process id.
 */
natl create_dummy()
{
    // create dummy process
    proc_elem* dummy_elem = crea_processo(dd, 0, DUMMY_PRIORITY, LEV_SYSTEM, true);
    
    // check if the dummy process was correctly created
    if (dummy_elem == 0)
    {
        // otherwise log error message
        flog(LOG_ERR, "Unable to create dummy process.");

        // return error code
        return 0xFFFFFFFF;
    }

    // insert dummy process in the processes list
    list_insert(ready_proc, dummy_elem);

    // increment user processes counter
    user_processes++;

    // return dummy process id
    return dummy_elem->id;
}

/**
 *
 */
void main_sistema(int n);

/**
 *
 */
natl crea_main_sistema()
{
    proc_elem* m = crea_processo(main_sistema, 0, MAX_PRIORITY, LEV_SYSTEM, false);

    if (m == 0)
    {
        flog(LOG_ERR, "Impossibile creare il processo main_sistema");
        return 0xFFFFFFFF;
    }

    list_insert(ready_proc, m);

    user_processes++;

    return m->id;
}

// ( [P_EXTERN_PROC]
// Registrazione processi esterni
proc_elem* const ESTERN_BUSY = (proc_elem*)1;

/**
 * Sends the EOI (end of interrupt) to the APIC and schedules a new process.
 */
extern "C" void wfi();

// associa il processo esterno puntato da "p" all'interrupt "irq".
// Fallisce se un processo esterno era gia' stato associato a
// quello stesso interrupt
bool aggiungi_pe(proc_elem *p, natb irq)
{
	if (irq >= MAX_IRQ) {
		flog(LOG_WARN, "irq %d non valido (max %d)", irq, MAX_IRQ);
		return false;
	}
	if (a_p[irq]) {
		flog(LOG_WARN, "irq %d occupato", irq);
		return false;
	}

	a_p[irq] = p;
	apic_set_MIRQ(irq, false);
	return true;
}

/**
 * Creates a new process for an external PCI device. The process is also
 * associated to the interrupts coming from the external device.
 *
 * @param  f     
 * @param  a     
 * @param  prio  process priority to be used;
 * @param  liv   
 * @param  type  
 */
extern "C" void c_activate_pe(void f(int), int a, natl prio, natl liv, natb type)
{
    // proc_elem for the new process
    proc_elem	*p;

    // retrieve current process descriptor
    des_proc *self = des_p(execution->id);

    // the given process priority can not be higher than the calling process
    if (prio < MIN_PRIORITY)
    {
        // print warning log message
        flog(LOG_WARN, "Invalid process priority: %d", prio);

        // abort current process
        c_abort_p();

        // just return to the calling function
        return;
    }

    // create new process with the given parameters: IF = true
    p = crea_processo(f, a, prio, liv, true);

    // check if the processo was correctly created
    if (p == 0)
    {
        goto error1;
    }

    // associate external device interrupts
    if ( !aggiungi_pe(p, type) )
    {
        // in case of failure go to error #2
        goto error2;
    }

    // log data of the newly created process for the external PCI device
    flog(LOG_INFO, "estern=%d entry=%p(%d) prio=%d liv=%d type=%d", p->id, f, a, prio, liv, type);

    self->context[I_RAX] = p->id;
	return;

error2:
    destroy_process(p);
error1:
	self->context[I_RAX] = 0xFFFFFFFF;
	return;
}

bool is_accessible(vaddr a)
{
	for (int i = 4; i > 0; i--) {
		natq d = get_des(execution->id, i, a);
		bool bitP = extr_P(d);
		if (!bitP)
			return false;
	}
	return true;
}

// indirizzo del primo byte che non contiene codice di sistema (vedi "sistema.s")
extern "C" addr fine_codice_sistema;

/**
 * Dumps the information related to the given log to console output.
 *
 * @param  id       the id of the process to be dumped;
 * @param  log_sev  the log priority to be used when logging process info.
 */
void process_dump(natl id, log_sev sev)
{
    // check if the given process id is valid
    if (!id)
    {
        flog(sev, "Invalid process id: %d.", id);
        return;
    }

    // retrieve process to be dumped
    des_proc *p = des_p(id);

    // check if the retrieved process is valid
    if (!p)
    {
        flog(sev, "Process %d not found.", id);
        return;
    }

    // retrieve process stack pointer
    natq *pila = (natq*)p->context[I_RSP];

    // log process stack
    flog(sev, "  RIP=%lx CPL=%s", pila[0], pila[1] == SEL_CODICE_UTENTE ? "LEV_USER" : "LEV_SYSTEM");

    // retrieve process rflags register
    natq rflags = pila[2];

    // log process rflags register content
    flog(sev, "  RFLAGS=%lx [%s %s %s %s %s %s %s %s %s %s, IOPL=%s]",
		rflags,
		(rflags & 1U << 14) ? "NT" : "nt",
		(rflags & 1U << 11) ? "OF" : "of",
		(rflags & 1U << 10) ? "DF" : "df",
		(rflags & 1U << 9)  ? "IF" : "if",
		(rflags & 1U << 8)  ? "TF" : "tf",
		(rflags & 1U << 7)  ? "SF" : "sf",
		(rflags & 1U << 6)  ? "ZF" : "zf",
		(rflags & 1U << 4)  ? "AF" : "af",
		(rflags & 1U << 2)  ? "PF" : "pf",
		(rflags & 1U << 0)  ? "CF" : "cf",
		(rflags & 0x3000) == 0x3000 ? "USER" : "SYSTEM");

    // log %rax, %rbx, %rcx, %rdx registers content
    flog(sev, "  RAX=%lx RBX=%lx RCX=%lx RDX=%lx",
            p->context[I_RAX],
            p->context[I_RBX],
            p->context[I_RCX],
            p->context[I_RDX]);

    // log %rdi, %rsi, %rbp, %rsp registers content
    flog(sev, "  RDI=%lx RSI=%lx RBP=%lx RSP=%lx",
            p->context[I_RDI],
            p->context[I_RSI],
            p->context[I_RBP],
            pila[3] + 8);

    // log %r8, %r9, %r10, %r11 registers content
    flog(sev, "  R8 =%lx R9 =%lx R10=%lx R11=%lx",
            p->context[I_R8],
            p->context[I_R9],
            p->context[I_R10],
            p->context[I_R11]);
    
    // log %r12, %r13, %r14, %r15 registers content
    flog(sev, "  R12=%lx R13=%lx R14=%lx R15=%lx",
            p->context[I_R12],
            p->context[I_R13],
            p->context[I_R14],
            p->context[I_R15]);

    // log process backtrace
    flog(sev, "  backtrace:");

    natq rbp = p->context[I_RBP];

    for (;;)
    {
        natq* acsite = ((natq *)rbp + 1);
        
        if (((natq)acsite & 0x7) || !is_accessible((vaddr)acsite))
        {
            flog(sev, "  ! %lx", rbp);
            break;
        }
        
        addr csite = (addr)(*acsite - 5);
        
        if (csite < &start || csite >= fine_codice_sistema)
        {
            break;
        }

        flog(sev, "  > %lx", *((natq *)rbp + 1) - 5);
        
        rbp = *(natq *)rbp;
    }
}

/**
 *
 */
extern "C" void c_panic(const char *msg)
{
	static int in_panic = 0;

	if (in_panic) {
		flog(LOG_ERR, "panic ricorsivo. STOP");
		end_program();
	}
	in_panic = 1;

	flog(LOG_ERR, "PANIC: %s", msg);
	if (execution->id)
		process_dump(execution->id, LOG_ERR);
	flog(LOG_ERR, "  active user processes: %d", user_processes - 1);
	for (natl id = MIN_PROC_ID; id < MAX_PROC_ID; id += 16) {
		des_proc *p = des_p(id);
		if (p && p->cpl == LEV_USER) {
			vaddr v_eip = fin_sis_p - 5 * sizeof(natq);
			natq dp = get_des(id, 1, v_eip);
			natq ind_fis_pag = (natq)extr_IND_FISICO(dp);
			addr f_eip = (addr)(ind_fis_pag | (v_eip & 0xFFF));
			flog(LOG_ERR, "    *) proc=%d RIP=%p", id, *(natq*)f_eip);
		}
	}
	end_program();
}

// se riceviamo un non-maskerable-interrupt, fermiamo il sistema
extern "C" void c_nmi()
{
	panic("INTERRUZIONE FORZATA");
}

// restituisce l'indirizzo fisico che corrisponde a ind_virt nello
// spazio di indirizzamento del processo corrente.
extern "C" faddr c_trasforma(vaddr ind_virt)
{
	natq d;
	for (int liv = 4; liv > 0; liv--) {
		d = get_des(execution->id, liv, ind_virt);
		if (!extr_P(d)) {
			flog(LOG_WARN, "impossibile trasformare %lx: non presente a livello %d",
				ind_virt, liv);
			return 0;
		}
		if (extr_PS(d)) {
			// pagina di grandi dimensioni
			natq mask = (1UL << ((liv - 1) * 9 + 12)) - 1;
			return norm((d & ~mask) | (ind_virt & mask));
		}
	}
	return extr_IND_FISICO(d) | (ind_virt & 0xfff);
}

/**
 * Sets the interrupt type for the first 23 pins of the APIC.
 */
void apic_fill()
{
    apic_set_VECT(0, VETT_0);
    apic_set_VECT(1, VETT_1);
    apic_set_VECT(2, VETT_2);
    apic_set_VECT(3, VETT_3);
    apic_set_VECT(4, VETT_4);
    apic_set_VECT(5, VETT_5);
    apic_set_VECT(6, VETT_6);
    apic_set_VECT(7, VETT_7);
    apic_set_VECT(8, VETT_8);
    apic_set_VECT(9, VETT_9);
    apic_set_VECT(10, VETT_10);
    apic_set_VECT(11, VETT_11);
    apic_set_VECT(12, VETT_12);
    apic_set_VECT(13, VETT_13);
    apic_set_VECT(14, VETT_14);
    apic_set_VECT(15, VETT_15);
    apic_set_VECT(16, VETT_16);
    apic_set_VECT(17, VETT_17);
    apic_set_VECT(18, VETT_18);
    apic_set_VECT(19, VETT_19);
    apic_set_VECT(20, VETT_20);
    apic_set_VECT(21, VETT_21);
    apic_set_VECT(22, VETT_22);
    apic_set_VECT(23, VETT_23);
}

/**
 *
 */
extern "C" void attiva_timer(natl count);

/**
 *
 */
const natl DELAY = 59659;

/**
 * Initializes the GDT.
 */
extern "C" void init_gdt();

/**
 *
 */
extern "C" void salta_a_main();

/**
 * C++ STARTUP.
 * Called after the execution of _start:, start: defined in system.s.
 * The stack and the IDT have been initialized at this point.
 */
extern "C" void cmain()
{
    natl mid;

    // even though the first initial process is not yet completely initialized
    // we set its ID and priority in order to be able to identify it
    init.id = 0xFFFFFFFF;
    init.priority = MAX_PRIORITY;

    // set current process in execution
    execution = &init;

    // print welcome log message
    flog(LOG_INFO, "QEMU Linux Kernel, v5.12.6");

    // initialize GDT
    init_gdt();

    // log gdt initialized
    flog(LOG_INFO, "GDT Initialized.");

    // initialize system heap memory space
    heap_init((addr)HEAP_START, HEAP_SIZE);

    // log heap memory space initialized
    flog(LOG_INFO, "System heap: %x B @%x", HEAP_SIZE, HEAP_START);

    // 
    init_des_frame();
    flog(LOG_INFO, "Physical pages: %d", N_DF);

    // log system initialized parameters
    flog(LOG_INFO, "sis/cond [%p, %p)", ini_sis_c, fin_sis_c);
    flog(LOG_INFO, "sis/priv [%p, %p)", ini_sis_p, fin_sis_p);
    flog(LOG_INFO, "io /cond [%p, %p)", ini_mio_c, fin_mio_c);
    flog(LOG_INFO, "usr/cond [%p, %p)", ini_utn_c, fin_utn_c);
    flog(LOG_INFO, "usr/priv [%p, %p)", ini_utn_p, fin_utn_p);

    faddr inittab4 = crea_tab4();

    if (!crea_finestra_FM(inittab4))
        goto error;

    loadCR3(inittab4);
    flog(LOG_INFO, "Caricato CR3");

    // [libqlk]: 
    apic_init();

    // [libqlk]: 
    apic_reset();

    // 
    apic_fill();

    flog(LOG_INFO, "APIC Initialized.");

    // ( inizializzazione dello swap, che comprende la lettura
    //   degli entry point di start_io e start_utente
    if (!swap_init())
        goto error;

    flog(LOG_INFO, "sb: blocks = %d", swap_dev.sb.blocks);
    flog(LOG_INFO, "sb: user   = %p/%p", swap_dev.sb.user_entry, swap_dev.sb.user_end);
    flog(LOG_INFO, "sb: io     = %p/%p", swap_dev.sb.io_entry, swap_dev.sb.io_end);

    mid = crea_main_sistema();
    if (mid == 0xFFFFFFFF)
        goto error;

    flog(LOG_INFO, "Creato il processo main_sistema (id = %d)", mid);

    // create dummy process
    dummy_proc = create_dummy();

    // check if the dummy process was corretly allocated
    if (dummy_proc == 0xFFFFFFFF)
    {
        // otherwise go to error
        goto error;
    }

    // log dummy process id
    flog(LOG_INFO, "Dummy processo created: (id = %d)", dummy_proc);

    // schedule next process
    schedule();

    salta_a_main();

error:
    c_panic("Initialization Error.");
}

/**
 *
 */
void gdb_breakpoint() {}

/**
 *
 */
extern "C" natl activate_p(void f(int), int a, natl prio, natl liv);

/**
 *
 */
extern "C" void terminate_p();

/**
 *
 */
void main_sistema(int n)
{
    natl sync_io;

    // ( caricamento delle tabelle e pagine residenti degli spazi condivisi ()
    flog(LOG_INFO, "creazione o lettura delle tabelle e pagine residenti condivise...");
    
    if (!crea_spazio_condiviso())
    {
        goto error;
    }
 	// )

	gdb_breakpoint();

	// ( inizializzazione del modulo di io
	flog(LOG_INFO, "creazione del processo main I/O...");
	sync_io = sem_ini(0);
	if (sync_io == 0xFFFFFFFF) {
		flog(LOG_ERR, "Impossibile allocare il semaforo di sincr per l'IO");
		goto error;
	}
	// occupiamo l'entrata del timer
	aggiungi_pe(ESTERN_BUSY, 2);
	if (activate_p(swap_dev.sb.io_entry, sync_io, MAX_PRIORITY, LEV_SYSTEM) == 0xFFFFFFFF) {
		flog(LOG_ERR, "impossibile creare il processo main I/O");
		goto error;
	}
	flog(LOG_INFO, "attendo inizializzazione modulo I/O...");
	sem_wait(sync_io);
	// )

	// ( creazione del processo start_utente
	flog(LOG_INFO, "creazione del processo start_utente...");
	if (activate_p(swap_dev.sb.user_entry, 0, MAX_PRIORITY, LEV_USER) == 0xFFFFFFFF) {
		flog(LOG_ERR, "impossibile creare il processo main utente");
		goto error;
	}
	// )
	// (* attiviamo il timer
	attiva_timer(DELAY);
	flog(LOG_INFO, "Timer initialized (DELAY=%d)", DELAY);
	// *)
	// ( terminazione
	flog(LOG_INFO, "Switching to use process.");
	terminate_p();
	// )
error:
	panic("Errore di inizializzazione");
}

// ( [P_SWAP]
// lo swap e' descritto dalla struttura des_swap, che specifica il canale
// (primario o secondario) il drive (primario o master) e il numero della
// partizione che lo contiene. Inoltre, la struttura contiene una mappa di bit,
// utilizzata per l'allocazione dei blocchi in cui lo swap e' suddiviso, e un
// "super blocco".  Il contenuto del super blocco e' copiato, durante
// l'inizializzazione del sistema, dal primo settore della partizione di swap,
// e deve contenere le seguenti informazioni:
// - magic (un valore speciale che serve a riconoscere la partizione, per
// evitare di usare come swap una partizione utilizzata per altri scopi)
// - bm_start: il primo blocco, nella partizione, che contiene la mappa di bit
// (lo swap, inizialmente, avra' dei blocchi gia' occupati, corrispondenti alla
// parte utente/condivisa dello spazio di indirizzamento dei processi da
// creare: e' necessario, quindi, che lo swap stesso memorizzi una mappa di
// bit, che servira' come punto di partenza per le allocazioni dei blocchi
// successive)
// - blocks: il numero di blocchi contenuti nella partizione di swap (esclusi
// quelli iniziali, contenenti il superblocco e la mappa di bit)
// - directory: l'indice del blocco che contiene la tabella di livello 4
// - l'indirizzo virtuale dell'entry point del programma contenuto nello swap
// (l'indirizzo di main)
// - l'indirizzo virtuale successivo all'ultima istruzione del programma
// contenuto nello swap
// - l'indirizzo virtuale dell'entry point del modulo io contenuto nello swap
// - l'indirizzo virtuale successivo all'ultimo byte occupato dal modulo io
// - checksum: somma dei valori precedenti (serve ad essere ragionevolmente
// sicuri che quello che abbiamo letto dall'hard disk e' effettivamente un
// superblocco di questo sistema, e che il superblocco stesso non e' stato
// corrotto)
//

// usa l'istruzione macchina BSF (Bit Scan Forward) per trovare in modo
// efficiente il primo bit a 1 in v
extern "C" int trova_bit(natl v);
void scrivi_swap(addr src, natl blocco);
void leggi_swap(addr dest, natl blocco);

natl ceild(natl v, natl q)
{
	return v / q + (v % q != 0 ? 1 : 0);
}

natq alloca_blocco()
{
	natl i = 0;
	natq risu = 0;
	natq vecsize = ceild(swap_dev.sb.blocks, sizeof(natl) * 8);

	// saltiamo le parole lunghe che contengono solo zeri (blocchi tutti occupati)
	while (i < vecsize && swap_dev.free[i] == 0) i++;
	if (i < vecsize) {
		natl pos = __builtin_ffs(swap_dev.free[i]) - 1;
		swap_dev.free[i] &= ~(1UL << pos);
		risu = pos + sizeof(natl) * 8 * i;
	}
	return risu;
}

void dealloca_blocco(natl blocco)
{
	if (blocco == 0)
		return;
	swap_dev.free[blocco / 32] |= (1UL << (blocco % 32));
}



// legge dallo swap il blocco il cui indice e' passato come primo parametro,
// copiandone il contenuto a partire dall'indirizzo "dest"
void leggi_swap(addr dest, natl blocco)
{
	natl sector = blocco * 8;

	hdd_read(sector, 8, static_cast<natw*>(dest));
}

void scrivi_swap(addr src, natl blocco)
{
	natl sector = blocco * 8;

	hdd_write(sector, 8, static_cast<natw*>(src));
}

// inizializzazione del descrittore di swap
natw read_buf[256];
bool swap_init()
{
	// lettura del superblocco
	flog(LOG_DEBUG, "lettura del superblocco dall'area di swap...");
	hdd_read(1, 1, read_buf);

	swap_dev.sb = *reinterpret_cast<superblock_t*>(read_buf);

	// controlliamo che il superblocco contenga la firma di riconoscimento
	for (int i = 0; i < 8; i++)
		if (swap_dev.sb.magic[i] != "CE64SWAP"[i]) {
			flog(LOG_ERR, "Firma errata nel superblocco");
			return false;
		}

	// controlliamo il checksum
	int *w = (int*)&swap_dev.sb, sum = 0;
	for (natl i = 0; i < sizeof(swap_dev.sb) / sizeof(int); i++)
		sum += w[i];

	if (sum != 0) {
		flog(LOG_ERR, "Checksum errato nel superblocco");
		return false;
	}

	flog(LOG_DEBUG, "lettura della bitmap dei blocchi...");

	// calcoliamo la dimensione della mappa di bit in pagine/blocchi
	natl pages = ceild(swap_dev.sb.blocks, DIM_PAGINA * 8);

	// quindi allochiamo in memoria un buffer che possa contenerla
	swap_dev.free = static_cast<natl*>(alloca((pages * DIM_PAGINA)));
	if (swap_dev.free == 0) {
		flog(LOG_ERR, "Impossibile allocare la bitmap dei blocchi");
		return false;
	}
	// infine, leggiamo la mappa di bit dalla partizione di swap
	hdd_read(swap_dev.sb.bm_start * 8, pages * 8, reinterpret_cast<natw*>(swap_dev.free));
	return true;
}

/**
 * Semaphore are never deallocated and they can therefore be allocated
 * sequentially. In order to do so we will gave to keep track of the number
 * of allocated ones.
 */
natl allocated_sems = 0;

/**
 * Allocates a new semaphore and returns its number.
 *
 * @return  the number of the allocated semaphore.
 */
natl alloca_sem()
{
    natl i;

    // check if maximum number of allocable semaphore has been reached
    if (allocated_sems >= MAX_SEM)
    {
        return 0xFFFFFFFF;
    }

    // set semaphore number
    i = allocated_sems;

    // increment number of semaphores
    allocated_sems++;

    // return semaphore number
    return i;
}

// dal momento che i semafori non vengono mai deallocati,
// un semaforo e' valido se e solo se il suo indice e' inferiore
// al numero dei semafori allocat
/**
 * Checks if the given semaphore ID is valid: since semaphore are never
 * deallocated it only checks if the given id is lower than the number of
 * allocated semaphores.
 */
bool sem_valido(natl sem)
{
    return sem < allocated_sems;
}

/**
 * C++ body implementation for the a_sem_ini primitive.
 * Together they implement the sem_ini kernel primitive.
 */
extern "C" natl c_sem_ini(int val)
{
    // allocate semaphore
    natl i = alloca_sem();

    // check if there are still available semaphores
    if (i != 0xFFFFFFFF)
    {
        // set semaphore counter value
        array_dess[i].counter = val;
    }

    // return semaphore id
    return i;
}
// )
#ifdef AUTOCORR
int MAX_LOG = 4;
#else
int MAX_LOG = 5;
#endif

extern "C" void c_log(log_sev sev, const char* buf, natl quanti)
{
	do_log(sev, buf, quanti);
}



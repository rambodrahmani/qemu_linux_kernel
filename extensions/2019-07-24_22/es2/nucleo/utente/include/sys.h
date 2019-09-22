// sys.h
// header per l' interfaccia offerta dal nucleo e dal modulo di IO
//

#ifndef SYS_H_
#define SYS_H_

#include <libce.h>
#include <costanti.h>

extern "C" void do_log(log_sev sev, const char* buf, natl quanti);

extern "C" natl activate_p(void f(int), int a, natl prio, natl liv);
extern "C" void terminate_p();
extern "C" natl sem_ini(int val);
extern "C" void sem_wait(natl sem);
extern "C" void sem_signal(natl sem);
extern "C" void delay(natl n);

extern "C" void readse_n(natl serial, natb vetti[], natl quanti, natb &errore);
extern "C" void readse_ln(natl serial, natb vetti[], natl &quanti, natb &errore);
extern "C" void writese_0(natl serial, const natb vetto[], natl &quanti);
extern "C" void writese_n(natl serial, const natb vetto[], natl quanti);

extern "C" void iniconsole(natb cc);
extern "C" void readconsole(char* buff, natl& quanti);
extern "C" void writeconsole(const char* buff);

extern "C" void readhd_n(natw vetti[], natl primo, natb quanti, natb &errore);
extern "C" void writehd_n(const natw vetto[], natl primo, natb quanti, natb &errore);
extern "C" void dmareadhd_n(natw vetti[], natl primo, natb quanti, natb &errore);
extern "C" void dmawritehd_n(const natw vetto[], natl primo, natb quanti, natb& errore);
extern "C" natq getnframe();
extern "C" natl getpid();

// EXTENSION 2019-07-24

/**
 *
 */
typedef natq vaddr;

/**
 * Primitives declarations for the User Module: we will be providing to the
 * processes the ability to pause a certain process when it reaches a certain
 * instruction.
 */

/**
 * A master process can use to primitive to set a breakpoint (assembly
 * instruction int3, opcode 0xCC) at the given instruction address for the slave
 * process having the given id. If a process executes the int3 instruction
 * without being a slave it must be aborted.
 *
 * The specified process must not be a slave or a master process itself.
 *
 * In case of error the calling process must be aborted.
 *
 * @param  id   id of the slave process;
 * @param  rip  process instruction address: keep in mind that it must be in the
 *              interval defined by [ini_utn_c, fin_utn_c);
 *
 * @return  true if the breakpoint is successfully installed, false otherwise.
 */
extern "C" bool bpattach(natl id, vaddr rip);

/**
 * The master process can suspend its execution and wait until the breakpoint in
 * the slave process is reached. The calling process must be a master process.
 *
 * In case of error the calling process must be aborted.
 */
extern "C" void bpwait();

/**
 * Undoes the operation done by bpattach() and reschedules the slave process.
 * The calling process must be a master process.
 *
 * In case of error the calling process must be aborted.
 */
extern "C" void bpdetach();

// EXTENSION 2019-07-24

#endif


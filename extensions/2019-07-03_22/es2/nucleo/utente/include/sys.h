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

// EXTENSION 2019-07-03

/**
 * Virtual address definition for the User module.
 */
typedef natq vaddr;

/**
 * Primitives declarations for the User Module. We want to provide to the User
 * processes the ability to puase the execution of all processes which execute
 * a certain instruction.
 */

/**
 * Allows to install a breakpoint at the instruction having the provided virtual
 * address (keep in mind that the breakpoint exception has assembly instruction
 * int3 and opcode 0xCC). Once this method is called on a given address, all
 * processes execution will be paused and queued.
 *
 * For simplicity, at any given time, there is one and only one breakpoint
 * installed. If a process spontaneously calls the int3 instruction without
 * being added using this primitive, the calling process must be aborted.
 *
 * @param  rip  the virtual address of the instruction to be replaced with the
 *              breakpoit;
 */
extern "C" bool bpadd(vaddr rip);

/**
 * Can be used by a process to wait until a process reaches the breakpoint
 * instruction address. This primitive waits for one process to reach the
 * breakpoint instruction address and returns its id; it can be called multiple
 * times in order to retrieve the id of all the processes with the breakpoint
 * installed; it is an error if there not breakpoints installed in the system
 * global breakpoint descriptor.
 */
extern "C" natl bpwait();

/**
 * Removes the breakpoint and reschedules all paused process. The processes
 * rescheduled must execute without side effects.
 */
extern "C" void bpremove();

// EXTENSION 2019-07-03

#endif


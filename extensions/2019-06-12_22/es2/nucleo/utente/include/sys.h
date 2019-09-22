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

// EXTENSION 2019-06-12

/**
 * Virtual address definition for the User Module.
 */
typedef natq vaddr;

/**
 * We want to provide to the processes a mechanism to allow them to know if the
 * execution of one of the other processes reaches a certain instruction. Only
 * the first process reaching the breakpoint instruction must be intercepted.
 * All other processes should work seamlessly.
 */

/**
 * This primitive allows a user process to install a breakpoint (assembly
 * instruction int3, opcode 0xCC) at the address pointed by rip. The calling
 * process (P1) must be paused. When another process (P2) reaches the breakpoint
 * address P1 must be rescheduled. The P2 process is not paused, however it
 * might have to be rescheduled in order to guarantee processes priority.
 *
 * All other process executing the int3 instruction must be aborted.
 *
 * The rip address must belong to the process user/shared memory area. Otherwise
 * the calling process must be aborted.
 *
 * @param  rip  breakpoint instruction address.
 *
 * @return  the id of the intercepted process at the given address or 0xFFFFFFFF
 *          if another process has already requested a breakpoint (at any given
 *          address).
 */
extern "C" natl breakpoint(vaddr rip);

// EXTENSION 2019-06-12

#endif


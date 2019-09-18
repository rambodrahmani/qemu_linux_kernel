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

// EXTENSION 2016-09-20
/**
 *
 */
extern "C" enum broadcast_role
{
    B_BROADCASTER = 1,
    B_LISTENER
};

/**
 * Registers the current process as a listener of the global broadcast with the
 * given role.
 *
 * @param  role  the broadcast role to be used for the process registrations.
 */
extern "C" void reg(enum broadcast_role role);

/**
 *
 */
extern "C" natl listen();

/**
 *
 */
extern "C" void broadcast(natl msg);
// EXTENSION 2016-09-20


#endif


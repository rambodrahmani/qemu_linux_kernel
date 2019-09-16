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


// EXTENSION 2016-07-27
/**
 * This user primitive can be used to retrieve into the addressed buffer 'buf'
 * a maximum number of specified bytes (quanti) - as the CE device might contain
 * more available bytes which will therefore be lost - from the CE device having
 * the given id.
 *
 * @param  id      CE device id;
 * @param  quanti  number of bytes to be transferred;
 * @param  buf     destination buffer address;
 *
 * @return  true if the given buffer was big enough for all available bytes,
 *          false otherwise.
 */
extern "C" bool cedmaread(natl id, natl& quanti, char *buf);
extern "C" bool cedmaread2(natl id, natl* quanti, char *buf);
// EXTENSION 2016-07-27

#endif


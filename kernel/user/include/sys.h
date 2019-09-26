/**
 * File: sys.h
 *       Contains method declarations for the System and I/O modules interface
 *       primitives.
 *
 *       The Kernel provides to the I/O and User modules some primitives to be
 *       used to carry on operations (under control) that are not allowed at
 *       the user level. The basic idea behind this is that no user program
 *       must be trusted to work properly and therefore the system must not
 *       relay on them. The systme must guarantee that the processes, the memory
 *       and the I/O device will work correctly but the same time must allow the
 *       user programs to operate with them. In order to obtain such a result
 *       the system follows the following guidelines:
 *          - all user programs must be executed with user level privileges;
 *          - all critical data structures are hidden at user level privilege;
 *          - the system developer provides utility functions to the user in
 *            order to be able to execute system level operations (e.g. creating
 *            a process);
 *          - these functions can be access from the user module only from the
 *            IDT gates (the interrupt mechanism) which move the CPU to operate
 *            at system level.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/09/2019.
 */

#ifndef SYS_H_
#define SYS_H_

#include <libqlk.h>
#include <constants.h>
#include <log.h>

/**
 *
 */
extern "C" void do_log(log_sev sev, const char* buf, natl quanti);

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
extern "C" natl sem_ini(int val);

/**
 *
 */
extern "C" void sem_wait(natl sem);

/**
 *
 */
extern "C" void sem_signal(natl sem);

/**
 *
 */
extern "C" void delay(natl n);

/**
 *
 */
extern "C" void readse_n(natl serial, natb vetti[], natl quanti, natb &errore);

/**
 *
 */
extern "C" void readse_ln(natl serial, natb vetti[], natl &quanti, natb &errore);

/**
 *
 */
extern "C" void writese_0(natl serial, const natb vetto[], natl &quanti);

/**
 *
 */
extern "C" void writese_n(natl serial, const natb vetto[], natl quanti);

/**
 *
 */
extern "C" void iniconsole(natb cc);

/**
 *
 */
extern "C" void readconsole(char* buff, natl& quanti);

/**
 *
 */
extern "C" void writeconsole(const char* buff);

/**
 *
 */
extern "C" void readhd_n(natw vetti[], natl primo, natb quanti, natb &errore);

/**
 *
 */
extern "C" void writehd_n(const natw vetto[], natl primo, natb quanti, natb &errore);

/**
 *
 */
extern "C" void dmareadhd_n(natw vetti[], natl primo, natb quanti, natb &errore);

/**
 *
 */
extern "C" void dmawritehd_n(const natw vetto[], natl primo, natb quanti, natb& errore);

// USER-PRIMITIVE-EXAMPLE

/**
 * Returns the ID of the process under execution.
 */
extern "C" natl getid();

// USER-PRIMITIVE-EXAMPLE

#endif


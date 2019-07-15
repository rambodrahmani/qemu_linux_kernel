/**
 * File: do_log.cpp
 *       Writes the given log message to the serial interface COM1.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"

extern int MAX_LOG;

/**
 *
 */
struct proc_elem
{
    natl id;
};

extern proc_elem *esecuzione;

extern "C" void do_log(log_sev sev, const char* buf, natl quanti)
{
    const char* lev[] = { "DBG", "INF", "WRN", "ERR", "USR" };

    if (sev > MAX_LOG)
    {
        flog(LOG_WARN, "Livello di log errato: %d", sev);
    }

    const natb* l = (const natb*)lev[sev];

    while (*l)
    {
        serial1_o(*l++);
    }

    serial1_o((natb)'\t');

    natb idbuf[10];
    
    snprintf((char*)idbuf, 10, "%d", esecuzione->id);
    
    l = idbuf;

    while (*l)
    {
        serial1_o(*l++);
    }

    serial1_o((natb)'\t');

    for (natl i = 0; i < quanti; i++)
    {
        serial1_o(buf[i]);
    }

    serial1_o((natb)'\n');
}


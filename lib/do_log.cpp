/**
 * File: do_log.cpp
 *       Writes the given log message to the serial interface COM1.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "log.h"

extern int MAX_LOG;

/**
 *
 */
struct proc_elem
{
    natl id;
};

/**
 *
 */
extern proc_elem *process;

/**
 *
 */
extern "C" void do_log(log_sev sev, const char* message, natl length)
{
    // log severity level constant chars
    const char* lev[] = { "DBG", "INF", "WRN", "ERR", "USR" };

    // check if the given log severity level is correct
    if (sev > MAX_LOG)
    {
        flog(LOG_WARN, "Wrong log severity level: %d", sev);
    }

    // cast the log severity level chars to bytes
    const natb* l = (const natb*)lev[sev];

    // write the lov severity level chars to the serial interface COM1
    while (*l)
    {
        serial1_o(*l++);
    }

    // write a tabulation char to the serial interface COM1
    serial1_o((natb)'\t');

    // format and store the process ID in idbuf
    natb idbuf[10];

    // the snprintf() function formats and stores a series of characters and
    // values in the array buffer.
    snprintf((char*)idbuf, 10, "%d", process->id);
    
    // write the process id to the serial interface COM1
    l = idbuf;
    while (*l)
    {
        serial1_o(*l++);
    }

    // write a tabulation char to the serial interface COM1
    serial1_o((natb)'\t');

    // write the log message buffer to the COM1 serial interface
    for (natl i = 0; i < length; i++)
    {
        serial1_o(message[i]);
    }

    // write a new line char to the serial interface COM1
    serial1_o((natb)'\n');
}


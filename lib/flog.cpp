/**
 * File: flog.cpp
 *       Formats the given log message and writes it to the serial interface
 *       COM1 using the do_log utility function.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "log.h"

extern "C" void do_log(log_sev sev, const char* buf, natl size);

extern "C" void flog(log_sev sev, const char* fmt, ...)
{
    va_list ap;

    // maximum log message buffer size
    const natl LOG_MSG_SIZE = 256;

    // log message buffer
    char buf[LOG_MSG_SIZE];

    va_start(ap, fmt);

    int l = vsnprintf(buf, LOG_MSG_SIZE, fmt, ap);
    
    va_end(ap);

    if (l > 1)
    {
        do_log(sev, buf, l - 1);
    }
}


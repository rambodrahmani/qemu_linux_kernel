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

/**
 * Writes the given log message to the serial interface COM1.
 * [do_log.cpp]
 */
extern "C" void do_log(log_sev sev, const char* buf, natl size);

/**
 * When using this method, keep in mind that % is a special character.
 */
extern "C" void flog(log_sev sev, const char* fmt, ...)
{
    // variable argument list
    va_list ap;

    // maximum log message buffer size
    const natl LOG_MSG_SIZE = 256;

    // log message buffer
    char buf[LOG_MSG_SIZE];

    // initialize variable argument list 'ap'
    va_start(ap, fmt);

    // convert the given c++ formatted string to plain string, [0]
    int l = vsnprintf(buf, LOG_MSG_SIZE, fmt, ap);

    // end using variable argument list
    va_end(ap);

    // check if at least one char has to be written
    if (l > 1)
    {
        do_log(sev, buf, l - 1);
    }
}

/**
 * The reason for this conversion comes from the fact that we will be writing
 * the log message to the serial interface COM1 and to do so we will have to
 * write each char (byte) separately to its THR register.
 */


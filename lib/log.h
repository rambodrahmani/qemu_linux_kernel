/**
 * File: log.h
 *       Defines logging related constants and functions.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "type.h"

/**
 * This enumeration contains the severity which can be used for a log.
 */
enum log_sev
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_USR
};

/**
 * Formats the given log message and writes it to the serial interface COM1
 * using the do_log utility function.
 *
 * @param   sev   log severity level;
 * @param   fmt   log message char array.
 */
extern "C" void flog(log_sev sev, const char* fmt, ...);

/**
 * Writes the given log message to the serial interface COM1.
 *
 * @param   sev       log severity level;
 * @param   message   log message buffer;
 * @param   length    log message buffer length.
 */
extern "C" void do_log(log_sev sev, const char* message, natl length);


/**
 * File: handle_exception.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "log.h"

/**
 * Writes a log message containing information relative to the exception.
 */
extern "C" void handle_exception(line type, line err, void* eip)
{
    flog(LOG_WARN, "Exception %ld, error = %lx, EIP = %p\n", type, err, eip);
}


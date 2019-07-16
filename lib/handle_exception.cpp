/**
 * File: handle_exception.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

extern "C" void handle_exception(line tipo, line err, void* eip)
{
    flog(LOG_WARN, "Eccezione %ld, err=%lx, EIP=%p\n", tipo,  err, eip);
}


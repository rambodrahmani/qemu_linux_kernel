/**
 * File: gestore_eccezioni.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

extern "C" void gestore_eccezioni(line tipo, line err, void* eip)
{
    flog(LOG_WARN, "Eccezione %ld, err=%lx, EIP=%p\n", tipo,  err, eip);
}

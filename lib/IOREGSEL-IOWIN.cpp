/**
 * File: IOREGSEL-IOWIN.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "type.h"

natl* IOREGSEL = reinterpret_cast<natl*>(0xFEC0000);

natl* IOWIN = reinterpret_cast<natl*>(0xFEC0010);

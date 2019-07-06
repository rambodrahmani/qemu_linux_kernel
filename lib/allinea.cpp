/**
 * File: allinea.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

 #include "internal.h"

// restituisce il valore di "v" allineato a un multiplo di "a"
size_t allinea(size_t v, size_t a)
{
    return (v % a == 0 ? v : ((v + a - 1) / a) * a);
}

/**
 * File: dealloca.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "heap.h"

// "dealloca" deve essere usata solo con puntatori restituiti da "alloca", e rende
// nuovamente libera la zona di memoria di indirizzo iniziale "p".
void dealloca(void* p)
{
    // e' normalmente ammesso invocare "dealloca" su un puntatore nullo.
    // In questo caso, la funzione non deve fare niente.
    if (p == 0) return;

    // recuperiamo l'indirizzo del descrittore della zona
    des_mem* des = reinterpret_cast<des_mem*>(p) - 1;

    // se non troviamo questo valore, vuol dire che un qualche errore grave
    // e' stato commesso (free su un puntatore non restituito da malloc,
    // doppio free di un puntatore, sovrascrittura del valore per accesso
    // al di fuori di un array, ...)
    if (des->next != reinterpret_cast<des_mem*>(0xdeadbeef))
        panic("free() errata");

    // la zona viene liberata tramite la funzione "internal_free", che ha
    // bisogno dell'indirizzo di partenza e della dimensione della zona
    // comprensiva del suo descrittore
    internal_free(des, des->size + sizeof(des_mem));
}


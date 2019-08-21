/**
 * File: heap.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

// Lo heap e' composto da zone di memoria libere e occupate. Ogni zona di memoria
// e' identificata dal suo indirizzo di partenza e dalla sua dimensione.
// Ogni zona di memoria contiene, nei primi byte, un descrittore di zona di
// memoria (di tipo des_mem), con un campo "dimensione" (dimensione in byte
// della zona, escluso il descrittore stesso) e un campo "next", significativo
// solo se la zona e' libera, nel qual caso il campo punta alla prossima zona
// libera. Si viene quindi a creare una lista delle zone libere, la cui testa
// e' puntata dalla variabile "memlibera" (allocata staticamente). La lista e'
// mantenuta ordinata in base agli indirizzi di partenza delle zone libere.

// descrittore di zona di memoria: descrive una zona di memoria nello heap di
// sistema
struct des_mem
{
    size_t dimensione;
    des_mem* next;
};

extern des_mem* memlibera;

/**
 *
 */
void free_interna(addr indirizzo, size_t quanti);


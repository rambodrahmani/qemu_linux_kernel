/**
 * File: internal_free.cpp
 *       Frees the memory space identified by the provided startind address and
 *       size (in bytes). It will also take care of creating a new block
 *       descriptor for the newly freed memory space and to provide a contiguous
 *       allocation in order to avoid any false fragmentation problem (many
 *       single contiguous free blocks which can not be used to allocate objects
 *       bigger than the size of each of the blocks).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "heap.h"
#include "log.h"

void internal_free(addr address, size_t size)
{
    // size can be 0, in which case no allocate will take place
    if (size == 0) return;

    // address, on the other hand, can not be zero
    if (address == 0) panic("free_interna(0)");

    // the newly freed memory space must be inserted in the available free
    // heap memory space list (ordered by starting address)
    des_mem *prec = 0;
    des_mem *scorri = free_heap;
    while (scorri != 0 && scorri < address)
    {
        prec = scorri;
        scorri = scorri->next;
    }
    // assert(scorri == 0 || scorri >= address)

    // in some cases we can find double free errors: 'address' must not be equal
    // to the address of a free space memory
    if (scorri == address)
    {
        // log double free error
        flog(LOG_ERR, "address = 0x%x", address);
		panic("double free error");
	}
	// assert(scorri == 0 || scorri > address)

	// verifichiamo che la zona possa essere unificata alla zona che la
	// precede.  Cio' e' possibile se tale zona esiste e il suo ultimo byte
	// e' contiguo al primo byte della nuova zona
	if (prec != 0 && (natb*)(prec + 1) + prec->size == address) {

		// controlliamo se la zona e' unificabile anche alla eventuale
		// zona che la segue
		if (scorri != 0 && static_cast<natb*>(address) + size == (addr)scorri) {
			
			// in questo caso le tre zone diventano una sola, di
			// dimensione pari alla somma delle tre, piu' il
			// descrittore della zona puntata da scorri (che ormai
			// non serve piu')
			prec->size += size + sizeof(des_mem) + scorri->size;
			prec->next = scorri->next;

		} else {

			// unificazione con la zona precedente: non creiamo una
			// nuova zona, ma ci limitiamo ad aumentare la
			// dimensione di quella precedente
			prec->size += size;
		}

	} else if (scorri != 0 && static_cast<natb*>(address) + size == (addr)scorri) {

		// la zona non e' unificabile con quella che la precede, ma e'
		// unificabile con quella che la segue. L'unificazione deve
		// essere eseguita con cautela, per evitare di sovrascrivere il
		// descrittore della zona che segue prima di averne letto il
		// contenuto
		
		// salviamo il descrittore della zona seguente in un posto
		// sicuro
		des_mem salva = *scorri; 
		
		// allochiamo il nuovo descrittore all'inizio della nuova zona
		// (se size < sizeof(des_mem), tale descrittore va a
		// sovrapporsi parzialmente al descrittore puntato da scorri)
		des_mem* nuovo = reinterpret_cast<des_mem*>(address);

		// quindi copiamo il descrittore della zona seguente nel nuovo
		// descrittore. Il campo next del nuovo descrittore e'
		// automaticamente corretto, mentre il campo dimensione va
		// aumentato di "size"
		*nuovo = salva;
		nuovo->size += size;

		// infine, inseriamo "nuovo" in lista al posto di "scorri"
		if (prec != 0) 
			prec->next = nuovo;
		else
			free_heap = nuovo;

	} else if (size >= sizeof(des_mem)) {

		// la zona non puo' essere unificata con nessun'altra.  Non
		// possiamo, pero', inserirla nella lista se la sua dimensione
		// non e' tale da contenere il descrittore (nel qual caso, la
		// zona viene ignorata)

		des_mem* nuovo = reinterpret_cast<des_mem*>(address);
		nuovo->size = size - sizeof(des_mem);

		// inseriamo "nuovo" in lista, tra "prec" e "scorri"
		nuovo->next = scorri;
		if (prec != 0)
			prec->next = nuovo;
		else
			free_heap = nuovo;
	}
}


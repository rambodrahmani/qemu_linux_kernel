/**
 * File: interp.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 02/11/2019.
 */

#include "constants.h"
#include "interp.h"

/**
 *
 */
ListaInterpreti* ListaInterpreti::instance()
{
	if (!instance_) {
		instance_ = new ListaInterpreti();
	}
	return instance_;
}

/**
 *
 */
Interprete* ListaInterpreti::prossimo()
{
	Interprete *in = NULL;
	if (curr) {
		in = curr->in;
		curr = curr->next;
	}
	return in;
}

/**
 *
 */
Interprete::Interprete()
{
	ListaInterpreti::instance()->aggiungi(this);
}

/**
 *
 */
ListaInterpreti* ListaInterpreti::instance_ = NULL;


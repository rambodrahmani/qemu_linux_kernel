/**
 * File: puts.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

int puts(const char *buf)
{
    str_write(buf);
    char_write('\n');
    char_write('\r');
    
	return 1;
}

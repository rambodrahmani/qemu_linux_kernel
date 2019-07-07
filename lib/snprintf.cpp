/**
 * File: snprintf.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

int snprintf(char *buf, natl n, const char *fmt, ...)
{
        va_list ap;
        int l;

        va_start(ap, fmt);
        l = vsnprintf(buf, n, fmt, ap);
        va_end(ap);

        return l;
}

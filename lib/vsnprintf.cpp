/**
 * File: vsnprintf.cpp
 *       The vsnprintf() function formats and stores a series of characters and
 *       values in the buffer target-string. The vsnprintf() function works just
 *       like the snprintf() function, except that arg_ptr points to a list of
 *       arguments whose number can vary from call to call in the program. These
 *       arguments should be initialized by the va_start function for each call.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

#define DEC_BUFSIZE 12

/**
 * Write formatted data from variable argument list to sized buffer.
 * Composes a string with the same text that would be printed if fmt was used on
 * printf, but using the elements in the variable argument list identified by
 * ap instead of additional function arguments and storing the resulting content
 * as a C string in the buffer pointed by str (taking size as the maximum buffer
 * capacity to fill).
 *
 * @param  str   pointer to a buffer where the resulting C-string is stored;
 * @param  size  maximum number of bytes to be used in the buffer;
 * @param  fmt   C string that contains a format string that follows the same
 *               specifications as format in printf (see printf for details);
 * @param  ap    a value identifying a variable arguments list initialized with
 *               va_start;
 *
 * @return  the number of bytes that are written in the array, not counting the
 *          ending null character.
 */
int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    size_t in = 0, out = 0, tmp;

    char *aux, buf[DEC_BUFSIZE];

    void *addr;

    int cifre = -1;

    bool islong = false;

    while(out < size - 1 && fmt[in])
    {
        switch(fmt[in])
        {
            case '%':
                again:
                    switch(fmt[++in])
                    {
                        case '1':
                        case '2':
                        case '4':
                        case '8':
                            cifre = fmt[in] - '0';
                            goto again;
                        case 'p':
                            addr = va_arg(ap, void*);
                            cifre = sizeof(void*) * 2;
                            if (out > size - (cifre + 1))
                                goto end;
                            htostr(&str[out], (unsigned long long)addr, cifre);
                            out += cifre;
                            break;
                        case 'l':
                            islong = true;
                            if (cifre < 0)
                                cifre = 16;
                            goto again;
                        case 'd':
                            tmp = (islong ? va_arg(ap, long) : va_arg(ap, int));
                            itostr(buf, DEC_BUFSIZE, tmp);
                            if (strlen(buf) > size - out - 1)
                                goto end;
                            for (aux = buf; *aux; ++aux)
                                str[out++] = *aux;
                            break;
                        case 'x':
                            if (cifre < 0)
                                cifre = 8;
                            tmp = (islong ? va_arg(ap, long) : va_arg(ap, int));
                            if (out > size - (cifre + 1))
                                goto end;
                            htostr(&str[out], tmp, cifre);
                            out += cifre;
                            break;
                        case 's':
                            aux = va_arg(ap, char *);
                            while (out < size - 1 && *aux) 
                                str[out++] = *aux++;
                            break;  
                        case 'c':
                            tmp = va_arg(ap, int);
                            if (out < size - 1)
                                str[out++] = tmp;
                        }
                    ++in;
                    break;
                default:
                    str[out++] = fmt[in++];
                }
            }
        end:
            str[out++] = 0;

    return out;
}


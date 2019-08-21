/**
 * File: vsnprintf.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

#define DEC_BUFSIZE 12

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


/**
 * File: lib.cpp
 *       User Module library. The utility methods defined here will be executed
 *       at user level.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 31/08/2019.
 */

#include <lib.h>
#include <sys.h>

/**
 *
 */
int printf(const char *fmt, ...)
{
    va_list ap;
    char buf[1024];
    int l;

    va_start(ap, fmt);
    l = vsnprintf(buf, 1024, fmt, ap);
    va_end(ap);

    writeconsole(buf);

    return l;
}

/**
 *
 */
int sprintf(char *buf, natq size, const char *fmt, ...)
{
    va_list ap;
    int l;

    va_start(ap, fmt);
    l = vsnprintf(buf, size, fmt, ap);
    va_end(ap);

    return l;
}

/**
 *
 */
char pause_buf[1];

/**
 *
 */
natl pause_len = 1;

/**
 * Prints a pause message to the video output and waits for the ENTER key.
 */
void pause()
{
    #ifndef AUTOCORR
        writeconsole("Press ENTER to continue.");
        readconsole(pause_buf, pause_len);
    #endif
}

/**
 *
 */
extern "C" void panic(const char* msg)
{
    flog(LOG_WARN, "%s", msg);
    terminate_p();
}

/**
 * Memory mutex.
 */
natl mem_mutex;

/**
 *
 */
void* mem_alloc(natl dim)
{
    void *p;

    sem_wait(mem_mutex);
    p = alloca(dim);
    sem_signal(mem_mutex);

    return p;
}

/**
 *
 */
void mem_free(void* p)
{
    sem_wait(mem_mutex);
    dealloca(p);
    sem_signal(mem_mutex);
}

/**
 * User heap memory space end.
 */
extern "C" natl end;

/**
 *
 */
extern "C" void lib_init()
{
    // retrieve user heap memory space end address
    unsigned long long end_ = (unsigned long long)&end;

    // allocate memory mutex
    mem_mutex = sem_ini(1);

    //
    end_ = (end_ + DIM_PAGINA - 1) & ~(DIM_PAGINA - 1);

    // allocate user heap memory space
    heap_init((void *)end_, DIM_USR_HEAP);
}


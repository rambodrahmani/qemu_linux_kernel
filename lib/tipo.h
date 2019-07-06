typedef void* addr;
// indirizzo virtuale e indirizzo fisico nello spazio di memoria
typedef unsigned short ioaddr;
// indirizzo (fisico) nello spazio di I/O
typedef unsigned char natb;
typedef unsigned short natw;
typedef unsigned int natl;
typedef unsigned long natq;
typedef void* str;
typedef const void* cstr;
#ifdef X86_64
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif

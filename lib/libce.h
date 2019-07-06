#include "tipo.h"
// Funzioni per leggere da o scrivere in un registro dello spazio di I/O
extern "C" void inputb(ioaddr reg, natb &a);
extern "C" void outputb(natb a, ioaddr reg);
extern "C" void inputw(ioaddr reg, natw &a);
extern "C" void outputw(natw a, ioaddr reg);
extern "C" void inputl(ioaddr reg, natl &a);
extern "C" void outputl(natl a, ioaddr reg);
extern "C" void outputbw(natw vetto[], int quanti, ioaddr reg);
extern "C" void inputbw(ioaddr reg, natw vetti[], int quanti);
// Funzioni per leggere da tastiera
natb get_code();
char char_read();
char conv(natb c);
void pause();
// Funzione per configurare la modalità video
natb* bochsvga_config(natw max_screenx, natw max_screeny);
// Funzioni per scrivere sul video
void clear_screen(natb col);
void char_write(natb c);
void str_write(const char str[]);
// Funzioni per leggere da o scrivere sul primo hard disk
void leggisett(natl lba, natb quanti, natw vetti[]);
void scrivisett(natl lba, natb quanti, natw vetti[]);
// Funzioni per la porta seriale
void serial_o(natb c);
void serial2_o(natb c);
// Funzioni sulle stringhe
void *memset(void *dest, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
size_t strlen(const char str[]);
char* copy(const char src[], char dst[]);
char* natb_conv(natb n, char dst[]);
char* natw_conv(natw n, char dst[]);
char* natl_conv(natl n, char dst[]);
char* natq_conv(natq n, char dst[]);
char* int_conv(long n, char dst[]);
// Funzione di attesa
void pause();
// Funzioni relative al meccanismo di interruzione
extern "C" void gate_init(natl num, void routine());
extern "C" void trap_init(natl num, void routine());
extern "C" void apic_set_MIRQ(natl irq, bool enable);
extern "C" void apic_set_VECT(natl irq, natb vec);
// Funzioni per la sincronizzazione
extern "C" void sem_wait(natl sem);
extern "C" void sem_signal(natl sem);
// Funzione per realizzare un ritardo
extern "C" void delay(natl num);
// Funzioni per il bus PCI
natb pci_read_confb(natb bus, natb dev, natb fun, natb regn);
natw pci_read_confw(natb bus, natb dev, natb fun, natb regn);
natl pci_read_confl(natb bus, natb dev, natb fun, natb regn);
void pci_write_confb(natb bus, natb dev, natb fun, natb regn, natb data);
void pci_write_confw(natb bus, natb dev, natb fun, natb regn, natw data);
void pci_write_confl(natb bus, natb dev, natb fun, natb regn, natl data);
bool pci_find_dev(natb& bus, natb& dev, natb& fun, natw vendorID, natw deviceID);
bool pci_find_class(natb& bus, natb& dev, natb& fun, natb code[]);
bool pci_next(natb& bus, natb& dev,natb& fun);
// Funzioni per lo heap
size_t allinea(size_t v, size_t a);
void heap_init(void *start, size_t size);
void* alloca(size_t dim);
void dealloca(void *p);
size_t disponibile();

// Funzioni per il log
enum log_sev { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERR, LOG_USR };
extern "C" void flog(log_sev sev, const char* fmt, ...);
extern "C" void do_log(log_sev sev, const char* buf, natl quanti);

// Funzioni avanzate
#include <stdarg.h>
int printf(const char *fmt, ...);
int vsnprintf(char *str, size_t size, const char *fmt, va_list ap);
int snprintf(char *buf, natl n, const char *fmt, ...);
bool apic_init();
void apic_reset();

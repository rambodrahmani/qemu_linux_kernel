#line 1 "utente/prog/pce.in"
/*
 * Programma di test 2016-06-15
 */

#include <sys.h>
#include <lib.h>

const int MSG_SIZE = 60;
const int NMESG = 2;

extern int mailbox_piena;
#line 12 "utente/prog/pce.in"
extern int mailbox_vuota;
#line 14 "utente/prog/pce.in"

#line 15 "utente/prog/pce.in"

#line 16 "utente/prog/pce.in"

#line 17 "utente/prog/pce.in"

#line 18 "utente/prog/pce.in"

#line 19 "utente/prog/pce.in"

#line 20 "utente/prog/pce.in"

#line 22 "utente/prog/pce.in"
struct mess {
	int mittente;
	char corpo[MSG_SIZE];
};

mess mailbox;

struct mybuf {
	char b[MSG_SIZE * 100];
} __attribute__((aligned(4096)));

mybuf buf[2];

void err1(int a)
#line 36 "utente/prog/pce.in"
{
	natl quanti = 256;
	cedmaread(0, quanti, (char *)0x20000);
	printf("processo errato 1");

	terminate_p();
}
void err2(int a)
#line 43 "utente/prog/pce.in"
{
	natl quanti = 100;
	cedmaread(0, quanti, buf[0].b + 30);
	printf("processo errato 2");

	terminate_p();
}
void err3(int a)
#line 50 "utente/prog/pce.in"
{
	cedmaread2(0, (natl*)0x20100, buf[0].b);
	printf("processo errato 3");

	terminate_p();
}
void err4(int a)
#line 56 "utente/prog/pce.in"
{
	natl quanti = 4096 * 10;
	cedmaread(0, quanti, (char *)-4096);
	printf("processo errato 4");

	terminate_p();
}
void pms(int a)
#line 63 "utente/prog/pce.in"
{
	natl quanti;
	for (int i = 0; i < MSG_SIZE * 100; i++) {
		buf[a].b[i] = 'a' + a;
	}
	for (int i = 0; i < NMESG; i++) {
		quanti = MSG_SIZE * 100;
		if (!cedmaread(0, quanti, buf[a].b)) {
			printf("scrittore %d DMA incompleto", a);
		}
		flog(LOG_DEBUG, "%d: letti %d byte", a, quanti);
		sem_wait(mailbox_vuota);
		natl j, k;
		for (j = 0, k = 0; k < quanti; k += 100, j++) {
			mailbox.corpo[j] = buf[a].b[k];
		}
		mailbox.corpo[j] = '\0';
		mailbox.mittente = a;
		sem_signal(mailbox_piena);
	}
	printf("fine scrittore %d", a);

	terminate_p();
}
void pml(int a)
#line 87 "utente/prog/pce.in"
{
	char corpo[MSG_SIZE];
	int mittente;
	for (int i = 0; i < 2 * NMESG; i++) {
		sem_wait(mailbox_piena);
		mittente = mailbox.mittente;
		copy(mailbox.corpo, corpo);
		sem_signal(mailbox_vuota);
		printf("mittente=%d corpo=%s", mittente, corpo);
	}
	printf("fine lettore");
	pause();

	terminate_p();
}
int mailbox_piena;
int mailbox_vuota;
short errato1;
short errato2;
short errato3;
short errato4;
short scrittore1;
short scrittore2;
short lettore;
#line 131 "utente/utente.cpp"

int main()
{
	mailbox_piena = sem_ini(0);
	mailbox_vuota = sem_ini(1);
	errato1 = activate_p(err1, 1, 22, LIV_UTENTE);
	errato2 = activate_p(err2, 2, 21, LIV_UTENTE);
	errato3 = activate_p(err3, 3, 20, LIV_UTENTE);
	errato4 = activate_p(err4, 4, 20, LIV_UTENTE);
	scrittore1 = activate_p(pms, 0, 6, LIV_UTENTE);
	scrittore2 = activate_p(pms, 1, 7, LIV_UTENTE);
	lettore = activate_p(pml, 0, 5, LIV_UTENTE);

	terminate_p();
}

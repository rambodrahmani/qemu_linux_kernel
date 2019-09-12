#line 1 "utente/prog/pce.in"
/*
 * Programma di test 2016-07-06
 */

#include <sys.h>
#include <lib.h>

const int MSG_SIZE = 16;
const int NMESG = 1;

extern int mailbox_piena;
#line 12 "utente/prog/pce.in"
extern int mailbox_vuota;
#line 14 "utente/prog/pce.in"

#line 15 "utente/prog/pce.in"

#line 16 "utente/prog/pce.in"

#line 18 "utente/prog/pce.in"
struct mess {
	int mittente;
	char corpo[MSG_SIZE + 1];
};

mess mailbox;

char buf1[MSG_SIZE] __attribute__((aligned(sizeof(natl)))) = { 'a' };
natl canary1 = 0x0a0b0c0d;
char buf2[MSG_SIZE] __attribute__((aligned(sizeof(natl)))) = { 'b' };
natl canary2 = 0x0e0f0102;

char *bufs[] = { buf1, buf2 };

void pms(int a)
#line 33 "utente/prog/pce.in"
{
	for (int i = 0; i < MSG_SIZE; i++) {
		bufs[a][i] = 'a' + a;
	}
	for (int i = 0; i < NMESG; i++) {
		natl quanti = MSG_SIZE;
		ceread(0, bufs[a], quanti, 'H' + a);
		sem_wait(mailbox_vuota);
		for (natl i = 0; i < quanti; i++) {
			mailbox.corpo[i] = bufs[a][i];
		}
		mailbox.corpo[quanti] = '\0';
		mailbox.mittente = a;
		sem_signal(mailbox_piena);
	}
	printf("fine scrittore %d", a);

	terminate_p();
}
void pml(int a)
#line 52 "utente/prog/pce.in"
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
	printf("controllo1=%x", canary1);
	printf("controllo2=%x", canary2);
	pause();

	terminate_p();
}
int mailbox_piena;
int mailbox_vuota;
short scrittore1;
short scrittore2;
short lettore;
#line 82 "utente/utente.cpp"

int main()
{
	mailbox_piena = sem_ini(0);
	mailbox_vuota = sem_ini(1);
	scrittore1 = activate_p(pms, 0, 6, LIV_UTENTE);
	scrittore2 = activate_p(pms, 1, 7, LIV_UTENTE);
	lettore = activate_p(pml, 0, 5, LIV_UTENTE);

	terminate_p();
}

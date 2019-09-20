#line 1 "utente/prog/pce.in"
/*
 * Programma di test 2016-06-15
 */

#include <sys.h>
#include <lib.h>

const int MSG_SIZE = 60;
const int NMESG = 2;

struct count {
	natq nr;
	natq pf;
};

count get_count()
{
	count c;
	natq r = countres();
	c.nr = r & 0xffffffff;
	c.pf = (r >> 32);
	return c;
}

natl end[4];

extern int mailbox_piena;
#line 28 "utente/prog/pce.in"
extern int mailbox_vuota;
#line 29 "utente/prog/pce.in"
extern int proc_terminati;
#line 31 "utente/prog/pce.in"

#line 32 "utente/prog/pce.in"

#line 33 "utente/prog/pce.in"

#line 34 "utente/prog/pce.in"

#line 36 "utente/prog/pce.in"
struct mess {
	int mittente;
	char corpo[MSG_SIZE];
};

mess mailbox;

void err1(int a)
#line 44 "utente/prog/pce.in"
{
	cedmaread(0, 256, (char *)0x20000);
	printf("processo errato 1");

	terminate_p();
}
void err2(int a)
#line 50 "utente/prog/pce.in"
{
	cedmaread(0, 4096 * 10, (char *)-4096);
	printf("processo errato 4");

	terminate_p();
}
void pms(int a)
{
	const natl m = 1000;
	char buf[MSG_SIZE * m];

	for (int i = 0; i < NMESG; i++) {
		if (!cedmaread(0, MSG_SIZE * m, buf)) {
			printf("scrittore %d: errore nel DMA", a);
		}
		sem_wait(mailbox_vuota);
		natl j, k;
		for (j = 0, k = 0; j < MSG_SIZE - 1; k += m, j++) {
			mailbox.corpo[j] = buf[k];
		}
		mailbox.corpo[j] = '\0';
		mailbox.mittente = a;
		sem_signal(mailbox_piena);
	}
	printf("fine scrittore %d", a);
	sem_signal(proc_terminati);
	sem_wait(end[a]);
	terminate_p();
}

void pml(int a)
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
	sem_signal(proc_terminati);
	sem_wait(end[a]);
	terminate_p();
}

void last(int a)
#line 97 "utente/prog/pce.in"
{
	sem_wait(end[a]);
	pause();

	terminate_p();
}
void main_body(int a)
#line 103 "utente/prog/pce.in"
{
	count ci, cf;

	for (int i = 0; i < 4; i++)
		end[i] = sem_ini(0);
	activate_p(pms, 0, 6, LIV_UTENTE);
	activate_p(pms, 1, 7, LIV_UTENTE);
	activate_p(pml, 2, 5, LIV_UTENTE);
	ci = get_count();
	for (int i = 0; i < 3; i++)
		sem_wait(proc_terminati);
	cf = get_count();
	if (ci.nr != cf.nr)
		printf("conteggio pagine residenti errato %d != %d", ci.nr, cf.nr);
	for (int i = 0; i < 4; i++)
		sem_signal(end[i]);

	terminate_p();
}
int mailbox_piena;
int mailbox_vuota;
int proc_terminati;
short last_proc;
short errato1;
short errato2;
short pce;
#line 143 "utente/utente.cpp"

int main()
{
	mailbox_piena = sem_ini(0);
	mailbox_vuota = sem_ini(1);
	proc_terminati = sem_ini(0);
	last_proc = activate_p(last, 3, 1, LIV_UTENTE);
	errato1 = activate_p(err1, 1, 22, LIV_UTENTE);
	errato2 = activate_p(err2, 2, 21, LIV_UTENTE);
	pce = activate_p(main_body, 0, 20, LIV_UTENTE);

	terminate_p();
}

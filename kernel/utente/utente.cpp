#line 1 "utente/prog/mailbox-dyn.in"
/*
 * Mailbox con memoria dinamica
 */

#include <sys.h>
#include <lib.h>

const int NMESG = 5;
const int MSG_SIZE = 100;

extern int mailbox_piena;
#line 12 "utente/prog/mailbox-dyn.in"
extern int mailbox_vuota;
#line 14 "utente/prog/mailbox-dyn.in"

#line 15 "utente/prog/mailbox-dyn.in"

#line 16 "utente/prog/mailbox-dyn.in"

#line 18 "utente/prog/mailbox-dyn.in"
struct mess {
	int mittente;
	char corpo[MSG_SIZE];
};

mess* mailbox;

void pms(int a)
#line 26 "utente/prog/mailbox-dyn.in"
{
	mess *ptr;
	for (int i = 0; i < NMESG; i++) {
		ptr = (mess*)mem_alloc(sizeof(mess));
		if (!ptr) {
			flog(LOG_WARN, "memoria esaurita");
			break;
		}	
		ptr->mittente = a;
		sprintf(ptr->corpo, MSG_SIZE, "Messaggio numero %d", i);
		sem_wait(mailbox_vuota);
		mailbox = ptr;
		sem_signal(mailbox_piena);
		delay(20);
	}
	printf("fine scrittore");

	terminate_p();
}
void pml(int a)
#line 45 "utente/prog/mailbox-dyn.in"
{
	mess *ptr;
	for (int i = 0; i < 2 * NMESG; i++) {
		sem_wait(mailbox_piena);
		ptr = mailbox;
		sem_signal(mailbox_vuota);
		printf("messaggio %d da %d: %s",
			i, ptr->mittente, ptr->corpo);
		mem_free(ptr);
		ptr = 0;
	}
	printf("fine lettore");
	pause();

	terminate_p();
}
int mailbox_piena;
int mailbox_vuota;
short scrittore1;
short scrittore2;
short lettore;
#line 74 "utente/utente.cpp"

int main()
{
	mailbox_piena = sem_ini(0);
	mailbox_vuota = sem_ini(1);
	scrittore1 = activate_p(pms, 1, 5, LEV_USER);
	scrittore2 = activate_p(pms, 2, 5, LEV_USER);
	lettore = activate_p(pml, 0, 5, LEV_USER);

	terminate_p();
}

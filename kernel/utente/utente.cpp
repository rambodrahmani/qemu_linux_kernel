#line 1 "utente/prog/mailbox.in"
/*
 * Mailbox
 */

#include <sys.h>
#include <lib.h>

const int NMESG = 5;
const int MSG_SIZE = 100;

extern int mailbox_piena;
#line 12 "utente/prog/mailbox.in"
extern int mailbox_vuota;
#line 14 "utente/prog/mailbox.in"

#line 15 "utente/prog/mailbox.in"

#line 16 "utente/prog/mailbox.in"

#line 18 "utente/prog/mailbox.in"
struct mess {
	int mittente;
	char corpo[MSG_SIZE];
};

mess mailbox;

void pms(int a)
#line 26 "utente/prog/mailbox.in"
{
	char buf[MSG_SIZE];
	char *ptr;
	for (int i = 0; i < NMESG; i++) {
		ptr = copy("Messaggio numero ", buf);
		int_conv(i, ptr);
		sem_wait(mailbox_vuota);
		mailbox.mittente = a;
		copy(buf, mailbox.corpo);
		sem_signal(mailbox_piena);
		delay(20);
	}
	ptr = copy("fine scrittore", buf);
	int_conv(a, ptr);
	writeconsole(buf);

	terminate_p();
}
void pml(int a)
#line 44 "utente/prog/mailbox.in"
{
	char buf[100 + MSG_SIZE], *ptr;
	char corpo[MSG_SIZE];
	int mittente;
	for (int i = 0; i < 2 * NMESG; i++) {
		sem_wait(mailbox_piena);
		mittente = mailbox.mittente;
		copy(mailbox.corpo, corpo);
		sem_signal(mailbox_vuota);
		ptr = copy("mittente=", buf);
		ptr = int_conv(mittente, ptr);
		ptr = copy(" corpo=", ptr);
		copy(corpo, ptr);
		writeconsole(buf);
	}
	writeconsole("fine lettore");
	pause();

	terminate_p();
}
int mailbox_piena;
int mailbox_vuota;
short scrittore1;
short scrittore2;
short lettore;
#line 77 "utente/utente.cpp"

int main()
{
	mailbox_piena = sem_ini(0);
	mailbox_vuota = sem_ini(1);
	scrittore1 = activate_p(pms, 1, 5, LIV_UTENTE);
	scrittore2 = activate_p(pms, 2, 5, LIV_UTENTE);
	lettore = activate_p(pml, 0, 5, LIV_UTENTE);

	terminate_p();
}

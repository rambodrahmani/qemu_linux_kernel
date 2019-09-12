#line 1 "utente/prog/pce.in"
/**
 * File: pce.in
 *       User test program for 2016-06-15_22.
 * 
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 02/09/2019
 * 
 */

#include <sys.h>
#include <lib.h>

const int MSG_SIZE = 60;
const int NMESG = 1;

extern int mailbox_piena;
#line 17 "utente/prog/pce.in"
extern int mailbox_vuota;
#line 19 "utente/prog/pce.in"

#line 20 "utente/prog/pce.in"

#line 21 "utente/prog/pce.in"

#line 23 "utente/prog/pce.in"
struct mess
{
    int mittente;
    char corpo[MSG_SIZE];
};

mess mailbox;

char buf[2][MSG_SIZE * 100];

void pms(int a)
#line 34 "utente/prog/pce.in"
{
    for (int i = 0; i < MSG_SIZE * 100; i++)
    {
        buf[a][i] = 'a' + a;
    }

    for (int i = 0; i < NMESG; i++)
    {
        cedmaread(0, buf[a], MSG_SIZE * 100);

        sem_wait(mailbox_vuota);

        for (int i = 0; i < MSG_SIZE - 1; i++)
        {
            mailbox.corpo[i] = buf[a][i * 100];
        }

        mailbox.corpo[MSG_SIZE - 1] = '\0';

        mailbox.mittente = a;

        sem_signal(mailbox_piena);
    }

    printf("fine scrittore %d", a);

	terminate_p();
}
void pml(int a)
#line 62 "utente/prog/pce.in"
{
    char corpo[MSG_SIZE];

    int mittente;

    for (int i = 0; i < 2 * NMESG; i++)
    {
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
short scrittore1;
short scrittore2;
short lettore;
#line 99 "utente/utente.cpp"

int main()
{
	mailbox_piena = sem_ini(0);
	mailbox_vuota = sem_ini(1);
	scrittore1 = activate_p(pms, 0, 6, LIV_UTENTE);
	scrittore2 = activate_p(pms, 1, 7, LIV_UTENTE);
	lettore = activate_p(pml, 0, 5, LIV_UTENTE);

	terminate_p();
}

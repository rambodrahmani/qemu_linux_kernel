#line 1 "utente/prog/pbroadcast.in"
#include <sys.h>
#include <lib.h>


#line 5 "utente/prog/pbroadcast.in"
extern int sync1;
#line 6 "utente/prog/pbroadcast.in"
extern int sync2;
#line 7 "utente/prog/pbroadcast.in"
extern int sync3;
#line 9 "utente/prog/pbroadcast.in"
void notreg(int a)
{
	listen();
	printf("processo errato %d", a);
	terminate_p();
}

void breg(int a)
{
	reg();
	broadcast(1);
	printf("processo errato %d", a);
	terminate_p();
}

void l1(int a)
{
	reg();
	sem_signal(sync1);
	natl m = listen();
	printf("processo %d mess %d", a, m);
	sem_signal(sync3);
	terminate_p();
}

void l2(int a)
{
	reg();
	sem_signal(sync2);
	natl m = listen();
	printf("processo %d mess %d", a, m);
	sem_signal(sync3);
	terminate_p();
}

void b3(int a)
{
	sem_wait(sync1);
	sem_wait(sync2);
	broadcast(a + 100);
	printf("processo %d", a);
	sem_signal(sync3);
	terminate_p();
}

void main_body(int a)
#line 55 "utente/prog/pbroadcast.in"
{
	activate_p(notreg, 1, 50, LIV_UTENTE);
	activate_p(breg, 1, 51, LIV_UTENTE);
	activate_p(l1, 1, 30, LIV_UTENTE);
	activate_p(l2, 2, 29, LIV_UTENTE);
	activate_p(b3, 3, 28, LIV_UTENTE);
	activate_p(l1, 4, 27, LIV_UTENTE);
	activate_p(b3, 5, 26, LIV_UTENTE);
	activate_p(l2, 6, 25, LIV_UTENTE);
	activate_p(b3, 7, 24, LIV_UTENTE);
	activate_p(l1, 8, 23, LIV_UTENTE);
	activate_p(l2, 9, 22, LIV_UTENTE);
	for (int i = 0; i < 9; i++)
		sem_wait(sync3);
	pause();

	terminate_p();
}
short pbarrier;
int sync1;
int sync2;
int sync3;
#line 83 "utente/utente.cpp"

int main()
{
	pbarrier = activate_p(main_body, 0, 100, LIV_UTENTE);
	sync1 = sem_ini(0);
	sync2 = sem_ini(0);
	sync3 = sem_ini(0);

	terminate_p();
}

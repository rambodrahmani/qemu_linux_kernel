#line 1 "utente/prog/pbroadcast.in"
#include <sys.h>
#include <lib.h>


#line 5 "utente/prog/pbroadcast.in"
extern int sync3;
#line 6 "utente/prog/pbroadcast.in"
natl sems[5];

void notreg(int a)
{
	listen();
	printf("processo errato %d", a);
	terminate_p();
}

void breg(int a)
{
	reg(B_LISTENER);
	broadcast(1);
	printf("processo errato %d", a);
	terminate_p();
}

void l1(int a)
{
	reg(B_LISTENER);
	for (int i = 0; i < 3; i++) {	
		sem_wait(sems[a]);
		natl m = listen();
		printf("processo %d mess %d", a, m);
	}
	terminate_p();
}


void b3(int a)
{
	sem_wait(sems[a]);
	reg(B_BROADCASTER);
	broadcast(a + 100);
	printf("processo %d", a);
	terminate_p();
}

void coordinator(int a)
{
	sem_signal(sems[0]);
	sem_signal(sems[1]);
	sem_signal(sems[2]);

	sem_signal(sems[0]);
	sem_signal(sems[3]);
	sem_signal(sems[1]);

	sem_signal(sems[4]);
	sem_signal(sems[0]);
	sem_signal(sems[1]);

	pause();
	terminate_p();
}

void main_body(int a)
#line 63 "utente/prog/pbroadcast.in"
{
	for (int i = 0; i < 5; i++)
		sems[i] = sem_ini(0);
	activate_p(notreg, 1, 50, LIV_UTENTE);
	activate_p(breg, 1, 51, LIV_UTENTE);
	activate_p(l1, 0, 30, LIV_UTENTE);
	activate_p(l1, 1, 10, LIV_UTENTE);
	activate_p(b3, 2, 28, LIV_UTENTE);
	activate_p(b3, 3, 26, LIV_UTENTE);
	activate_p(b3, 4, 24, LIV_UTENTE);
	activate_p(coordinator, 0, 10, LIV_UTENTE);

	terminate_p();
}
short pbarrier;
int sync3;
#line 84 "utente/utente.cpp"

int main()
{
	pbarrier = activate_p(main_body, 0, 100, LIV_UTENTE);
	sync3 = sem_ini(0);

	terminate_p();
}

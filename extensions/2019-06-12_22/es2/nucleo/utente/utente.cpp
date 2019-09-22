#line 1 "utente/prog/pbreak.in"
/*
 * Mailbox
 */

#include <sys.h>
#include <lib.h>


#line 9 "utente/prog/pbreak.in"

#line 10 "utente/prog/pbreak.in"

#line 11 "utente/prog/pbreak.in"

#line 12 "utente/prog/pbreak.in"

#line 13 "utente/prog/pbreak.in"

#line 14 "utente/prog/pbreak.in"

#line 15 "utente/prog/pbreak.in"

#line 16 "utente/prog/pbreak.in"

#line 17 "utente/prog/pbreak.in"

#line 18 "utente/prog/pbreak.in"

#line 20 "utente/prog/pbreak.in"
extern int sync;
#line 22 "utente/prog/pbreak.in"
void bad(int a)
#line 23 "utente/prog/pbreak.in"
{
	asm("int3");
	printf("processo errato %d", a);

	terminate_p();
}
void catch_me(int a)
{
	printf("proc%d: eseguo funzione", a);
}

vaddr bad_addr[] = { 1000, 0xffffc00000000000 };

void badb(int a)
#line 36 "utente/prog/pbreak.in"
{
	breakpoint(bad_addr[a - 3]);
	printf("processo errato %d", a);

	terminate_p();
}
void usr(int a)
#line 42 "utente/prog/pbreak.in"
{
	if (a % 2 == 0) {
		sem_wait(sync);
	}
	printf("proc%d: prima della funzione", a);
	catch_me(a);
	printf("proc%d: dopo la funzione", a);

	terminate_p();
}
void debugger(int a)
#line 52 "utente/prog/pbreak.in"
{
	printf("debugger %d: chiamo breakpoint", a);
	natl proc = breakpoint(reinterpret_cast<natq>(catch_me));
	if (proc == 0xFFFFFFFF) {
		printf("debugger %d: occupato", a);
	} else {
		sem_signal(sync);
		printf("debugger %d: breapoint intercettato, processo: %d", a, proc);
	}

	terminate_p();
}
void last_body(int a)
#line 64 "utente/prog/pbreak.in"
{
	pause();

	terminate_p();
}
short bad1;
short bad2;
short bad4;
short bad5;
short usr1;
short usr2;
short usr3;
short dbg1;
short dbg2;
short dbg3;
short last;
int sync;
#line 102 "utente/utente.cpp"

int main()
{
	bad1 = activate_p(bad, 1, 20, LIV_UTENTE);
	bad2 = activate_p(bad, 2, 8, LIV_UTENTE);
	bad4 = activate_p(badb, 3, 21, LIV_UTENTE);
	bad5 = activate_p(badb, 4, 22, LIV_UTENTE);
	usr1 = activate_p(usr, 1, 5, LIV_UTENTE);
	usr2 = activate_p(usr, 2, 15, LIV_UTENTE);
	usr3 = activate_p(usr, 3, 3, LIV_UTENTE);
	dbg1 = activate_p(debugger, 0, 10, LIV_UTENTE);
	dbg2 = activate_p(debugger, 1, 11, LIV_UTENTE);
	dbg3 = activate_p(debugger, 2, 4, LIV_UTENTE);
	last = activate_p(last_body, 0, 1, LIV_UTENTE);
	sync = sem_ini(0);

	terminate_p();
}

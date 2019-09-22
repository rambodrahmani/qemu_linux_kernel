#line 1 "utente/prog/pbreak.in"
/*
 * Mailbox
 */

#include <sys.h>
#include <lib.h>


#line 9 "utente/prog/pbreak.in"
extern short bad1;
#line 9 "utente/prog/pbreak.in"

#line 10 "utente/prog/pbreak.in"
extern short bad2;
#line 10 "utente/prog/pbreak.in"

#line 11 "utente/prog/pbreak.in"
extern short usr1;
#line 11 "utente/prog/pbreak.in"

#line 12 "utente/prog/pbreak.in"
extern short usr2;
#line 12 "utente/prog/pbreak.in"

#line 13 "utente/prog/pbreak.in"
extern short usr3;
#line 13 "utente/prog/pbreak.in"

#line 14 "utente/prog/pbreak.in"
extern short dbg1;
#line 14 "utente/prog/pbreak.in"

#line 15 "utente/prog/pbreak.in"
extern short dbg2;
#line 15 "utente/prog/pbreak.in"

#line 17 "utente/prog/pbreak.in"
extern short last;
#line 17 "utente/prog/pbreak.in"
extern int sync;
#line 18 "utente/prog/pbreak.in"
extern int sync2;
#line 20 "utente/prog/pbreak.in"
natq nframes;

#define MSG(fmt, ...)	do { printf("proc %d: " fmt, getpid(), ## __VA_ARGS__); } while (0)

void bad(int a)
#line 25 "utente/prog/pbreak.in"
{
	asm("int3");
	MSG("processo errato");

	terminate_p();
}
void catch_me(int a)
{
	MSG("eseguo catch_me(%d)", a);
}

vaddr bad_addr[] = { 1000, 0xffffc00000000000 };

void badb(int a)
#line 38 "utente/prog/pbreak.in"
{

	terminate_p();
}
void usr(int a)
#line 42 "utente/prog/pbreak.in"
{
	for (int i = 0; i < a; i++) {
		MSG("prima di catch_me()");
		catch_me(i);
		MSG("dopo catch_me()");
	}

	terminate_p();
}
void debugger(int a)
#line 51 "utente/prog/pbreak.in"
{
	MSG("debugger di %d: chiamo bpadd", a);
	if (!bpattach(a, reinterpret_cast<natq>(catch_me))) {
		MSG("debugger di %d: occupato", a);
	} else {
		bpwait();
		MSG("debugger di %d: breapoint intercettato", a);
		bpdetach();
	}

	terminate_p();
}
void last_body(int a)
#line 63 "utente/prog/pbreak.in"
{
	printf("frame liberi: %d", getnframe());
	pause();

	terminate_p();
}
short bad1;
short bad2;
short usr1;
short usr2;
short usr3;
short dbg1;
short dbg2;
short last;
int sync;
int sync2;
#line 114 "utente/utente.cpp"

int main()
{
	bad1 = activate_p(bad, 1, 40, LIV_UTENTE);
	bad2 = activate_p(bad, 2, 16, LIV_UTENTE);
	usr1 = activate_p(usr, 1, 10, LIV_UTENTE);
	usr2 = activate_p(usr, 1, 15, LIV_UTENTE);
	usr3 = activate_p(usr, 2, 6, LIV_UTENTE);
	dbg1 = activate_p(debugger, usr1, 20, LIV_UTENTE);
	dbg2 = activate_p(debugger, usr3, 22, LIV_UTENTE);
	last = activate_p(last_body, 0, 1, LIV_UTENTE);
	sync = sem_ini(0);
	sync2 = sem_ini(0);

	terminate_p();
}

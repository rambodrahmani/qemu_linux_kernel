/**
 * File: zero-division.cpp
 *       This example show how the kernel handles exceptions. Keep in mind that
 *       you must compile this with compiler optimization disabled (default mode
 *       for our scripts) other gcc might replace the zero division with an
 *       arbitrary peace code to avoid the exception happening.
 *
 *       Compile with: place the source file in the user folder and use
 *         make
 *         make swap
 *         ./run
 *
 *       Expected outout:
 *         INF	0	proc=7 entry=start [utente.s:9](0) prio=268435455 liv=3
 *         INF	0	Timer initialized (DELAY=59659)
 *         INF	0	Switching to use process.
 *         INF	0	Processo 0 aborted
 *         WRN	0	Exception 0, error = 0000000000000000, EIP = main [user.cpp:21]
 *
 *         WRN	0	  RIP=0000000000000141 CPL=LEV_USER
 *         WRN	0	  RFLAGS=0000000000000282 [nt of df IF tf SF zf af pf cf, IOPL=SYSTEM]
 *         WRN	0	  RAX=0000000000000003 RBX=0000000000000000 RCX=00000000000000a0 RDX=0000000000000000
 *         WRN	0	  RDI=0000000000004000 RSI=0000000000100000 RBP=00000000fffffff0 RSP=00000000ffffffd8
 *         WRN	0	  R8 =0000000000000000 R9 =0000000000000000 R10=0000000000000000 R11=0000000000000000
 *         WRN	0	  R12=0000000000000000 R13=0000000000000000 R14=0000000000000000 R15=0000000000000000
 *         WRN	0	  backtrace:
 *         WRN	0	Process 7 Current execution process aborted.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 31/08/2019.
 */

/**
 * Developer harness test.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    int x = 3;
    int y = 0;

    return x/y;
}


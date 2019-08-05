/**
 * File: main.e
 *       Output for
 *          g++ -E main.cpp >> main.e
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 03/08/2019.
 */

# 1 "main.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "main.cpp"
# 20 "main.cpp"
# 1 "lib.h" 1
# 21 "lib.h"
extern "C" long foo(long);
# 21 "main.cpp" 2

long var1 = 8;
long var2 = 4;

int main()
{


    var1 = foo(var2);

    return var1;
}

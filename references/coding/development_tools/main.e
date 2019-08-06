/**
 * File: main.e
 *       Output for:
 *          g++ -E main.cpp >> main.e
 *
 *       As you can see, the include directive resulted in the content of lib.h
 *       being placed inside the source file. New lines have been added at the
 *       beginning of the new file in order to allow for the compiler to show
 *       detailed compilation errors. The line containing the comment inside the
 *       main() function has been removed.
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

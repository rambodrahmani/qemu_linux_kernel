/**
 * File: main.cpp
 *       In order to obtain an executable program, we have to go through a
 *       series of steps and use different tools. We will be relaying on the
 *       tools provided by the GNU Compiler Collection (GCC).
 *       In order to be able to exemplify the usage of such tools, we will be
 *       using a main C++ file (main.cpp), a library file (lib.h) and an
 *       assembly file (foo.s).
 *
 *       Preprocess using:
 *          g++ -E main.cpp >> main.e
 *
 *       Compile using:
 *          g++ main.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 03/08/2019.
 */

#include "lib.h"        // [0]

long var1 = 8;
long var2 = 4;

int main()
{
    // un commento
    
    var1 = foo(var2);

    return var1;
}

/**
 * [0]
 * 1. #include <filename>
 * 2. #include "filename"
 *
 * Includes source file, identified by filename into the current source file at
 * the line immediately after the directive. In the case the file is not found,
 * program is ill-formed.
 *
 * For #include <filename> the preprocessor searches in an implementation
 * dependent manner, normally in search directories pre-designated by the
 * compiler/IDE. This method is normally used to include standard library header
 * files.
 *
 * For #include "filename" the preprocessor searches first in the same directory
 * as the file containing the directive, and then follows the search path used
 * for the #include <filename> form. This method is normally used to include
 * programmer-defined header files.
 */


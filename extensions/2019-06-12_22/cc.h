/**
 * File: cc.h
 *       Contains the declaration for the data structures used in the exercise.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 17/07/2019.
 */

#include <iostream>

using namespace std;

struct st1
{
    char vc[4];
};

class cl
{
    long v[4]; 
    st1 s;

public:
    cl(char c, st1 s2);

    void elab1(st1& s1);

    void stampa()
    {
        for (int i = 0; i < 4 ;i++)
        {
            cout << s.vc[i] << ' ';
        }
        cout << endl;

        for (int i = 0; i < 4; i++)
        {
            cout << v[i] << ' ';
        }
        cout << endl << endl;
    }
};


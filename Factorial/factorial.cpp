#include "factorial.h"
using namespace std;

factorial::factorial()
{
    current = 0;
};

int factorial::fact()
{
    if (current != 0)
    {
        factor = factor * current;
    }
    return factor;
};

int factorial::pow()
{
    int power = current / 2;
    power = power ^ current;
    return power;
};

void factorial::graph(int last)
{
    int x[last + 1];
    int f[last + 1];
    int p[last + 1];
    for (current = 0; current <= last; current ++)
    {
        x[current] = current;
        f[current] = factorial::fact();
        p[current] = factorial::pow();
    }
    
};
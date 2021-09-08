#include "fib.h"

int fib(int n) {
    if(n==0)
    {
        return 1;
    }
    if(n==-1)
    {
        return 0;
    }
    int c=fib(n-1)+fib(n-2);
    return c;
}
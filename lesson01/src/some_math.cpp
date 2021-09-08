#include "some_math.h"
#include <float.h>
#include <cmath>
int fibbonachiRecursive(int n) {
    if(n<3)
        return 1;
    return fibbonachiRecursive(n-1) + fibbonachiRecursive(n-2);
}

int fibbonachiFast(int n) {
    std::vector<int> fibo(n);
    if(n<3)
        return 1;
    fibo[0]=fibo[1]=1;
    for(int i = 2;i<n;i++)
        fibo[i]=fibo[i-1]+fibo[i-2];
    return fibo[n-1];
}

double solveLinearAXB(double a, double b) {
    if(a==0&&b!=0)
        return DBL_MAX;
    if(a==0)
        return -DBL_MAX;

    return -b/a;
}

std::vector<double> solveSquare(double a, double b, double c) {
    std::vector<double> results;
    double D = b*b-4*a*c;
    if(a==0)
        results.push_back(solveLinearAXB(b,c));
    else if(abs(D)<0.0001)
        results.push_back(-b/2/a);
    else
        results.push_back((-b-sqrt(D))/2/a);
        results.push_back((-b+sqrt(D))/2/a);
    return results;
}

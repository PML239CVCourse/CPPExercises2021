#include "some_math.h"
#include <bits/stdc++.h>
using namespace std;

int fibbonachiRecursive(int n) {
    vector<int> fib;
    fib.push_back(0);
    fib.push_back(1);


    int x;
    for (int i = 2; i < n+1; i++) {
        x = fib[fib.size() - 1] + fib[fib.size() - 2];
        fib.push_back(x);
    }

    return fib[n];
}

int fibbonachiFast(int n) {
    vector<int> fib;
    fib.push_back(0);
    fib.push_back(1);


    int x;
    for (int i = 2; i < n+1; i++) {
        x = fib[fib.size() - 1] + fib[fib.size() - 2];
        fib.push_back(x);
    }

    return fib[n];
}

double solveLinearAXB(double a, double b) {
    //  10 решите линейное уравнение a*x+b=0 а если решения нет - верните наибольшее значение double - найдите как это сделать в интернете по запросу "so cpp double max value" (so = stackoverflow = сайт где часто можно найти ответы на такие простые запросы), главное НЕ КОПИРУЙТЕ ПРОСТО ЧИСЛО, ПОЖАЛУЙСТААаа
    // если решений сколь угодно много - верните максимальное значение со знаком минус
    const double md = std::numeric_limits<double>::max(); //???
    if (a == 0 && b == 0) return (-md);
    if (a == 0) return md;
    return (-b / a);
}

std::vector<double> solveSquare(double a, double b, double c) {
    //  20 решите квадратное уравнение вида a*x^2+b*x+c=0
    // если корня два - они должны быть упорядочены по возрастанию
    std::vector<double> res;
    // чтобы добавить в вектор элемент - нужно вызвать у него метод push_back:
    if(a==0)
    {
        if(b==0) return res;
        else return {-c/b};
    }
    if (b * b == (4 * a * c)) {
        res.push_back((-b) / (2 * a));
        return res;
    }
    if (b * b < (4 * a * c)) {
        return res;
    }
    res.push_back((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
    res.push_back((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
    sort(res.begin(), res.end());
    return res;
}

#include "some_math.h"
#include <float.h>
#include <cmath>

int fibbonachiRecursive(int n) {
    // TODO 03 реализуйте функцию которая считает числа Фибоначчи - https://ru.wikipedia.org/wiki/%D0%A7%D0%B8%D1%81%D0%BB%D0%B0_%D0%A4%D0%B8%D0%B1%D0%BE%D0%BD%D0%B0%D1%87%D1%87%D0%B8
    if (n == 1 || n == 2){
        return 1;
    }
    if (n == 0){
        return 0;
    }
    return fibbonachiRecursive(n-1) + fibbonachiRecursive(n-2);
}

int fibbonachiFast(int n) {
    // TODO 04 реализуйте быструю функцию Фибоначчи с использованием std::vector
    std::vector<int> fs;
    for (int i = 0; i < n; ++i) {
        if (i == 1 || i == 0){
            fs.push_back(i);
        }
        else{
            fs.push_back(fs[i-1]+fs[i-2]);
        }
    }
    return fs[n-1];
}

double solveLinearAXB(double a, double b) {
    // TODO 10 решите линейное уравнение a*x+b=0 а если решения нет - верните наибольшее значение double - найдите как это сделать в интернете по запросу "so cpp double max value" (so = stackoverflow = сайт где часто можно найти ответы на такие простые запросы), главное НЕ КОПИРУЙТЕ ПРОСТО ЧИСЛО, ПОЖАЛУЙСТААаа
    // если решений сколь угодно много - верните максимальное значение со знаком минус
    double x = 0.0;
    if (a == 0){
        if (b == 0){
            return -DBL_MAX;
        }
        else{
            return DBL_MAX;
        }
    }
    else{
        x = -b/a;
    }
    return x;
}

std::vector<double> solveSquare(double a, double b, double c) {
    // TODO 20 решите квадратное уравнение вида a*x^2+b*x+c=0
    // если корня два - они должны быть упорядочены по возрастанию
    std::vector<double> results;
    if (a == 0){
        results.push_back(solveLinearAXB(b,c));
        //results.push_back(solveLinearAXB(b,c));
    }
    else{
        if (b*b-4*a*c >= 0){
            results.push_back((-b-sqrt(b*b-4*a*c))/(2*a));
            results.push_back((-b+sqrt(b*b-4*a*c))/(2*a));
        }
        else{
            results.push_back(DBL_MAX);
            //results.push_back(DBL_MAX);
        }
    }
    return results;
}

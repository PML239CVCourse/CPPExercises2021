#include "some_math.h"

int fibbonachiRecursive(int n) {
    // TODO 03 реализуйте функцию которая считает числа Фибоначчи - https://ru.wikipedia.org/wiki/%D0%A7%D0%B8%D1%81%D0%BB%D0%B0_%D0%A4%D0%B8%D0%B1%D0%BE%D0%BD%D0%B0%D1%87%D1%87%D0%B8
    int value = 0;

    if(n < 1) return 0;

    if(n == 1) return 1;

    return fibbonachiRecursive(n-1) + fibbonachiRecursive(n - 2);
}

int fibbonachiFast(int n) {
    std::vector<int> fib;
    if (n == 2 || n == 1) return 1;

    fib.push_back(1);
    fib.push_back(1);
    for (int i = 2; i < n; i++){
        fib.push_back(fib[fib.size() - 1] + fib[fib.size() - 2]);
    }
    return fib[fib.size()];
    }

double solveLinearAXB(double a, double b) {
    // TODO 10 решите линейное уравнение a*x+b=0 а если решения нет - верните наибольшее значение double - найдите как это сделать в интернете по запросу "so cpp double max value" (so = stackoverflow = сайт где часто можно найти ответы на такие простые запросы), главное НЕ КОПИРУЙТЕ ПРОСТО ЧИСЛО, ПОЖАЛУЙСТААаа
    // если решений сколь угодно много - верните максимальное значение со знаком минус
    double x = 0.0;
    return x;
}

std::vector<double> solveSquare(double a, double b, double c) {
    // TODO 20 решите квадратное уравнение вида a*x^2+b*x+c=0
    // если корня два - они должны быть упорядочены по возрастанию
    std::vector<double> results;
    // чтобы добавить в вектор элемент - нужно вызвать у него метод push_back:
    results.push_back(23.9);
    return results;
}

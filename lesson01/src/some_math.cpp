#include "some_math.h"

int fibbonachiRecursive(int n) {
    // TODO 03 реализуйте функцию которая считает числа Фибоначчи - https://ru.wikipedia.org/wiki/%D0%A7%D0%B8%D1%81%D0%BB%D0%B0_%D0%A4%D0%B8%D0%B1%D0%BE%D0%BD%D0%B0%D1%87%D1%87%D0%B8
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return fibbonachiRecursive(n - 1) + fibbonachiRecursive(n - 2);
}

int fibbonachiFast(int n) {
    // TODO 04 реализуйте быструю функцию Фибоначчи с использованием std::vector
    std::vector<double> vector;
    vector.push_back(0);
    vector.push_back(1);
    if (n == 0)
        return vector[0];
    if (n == 1)
        return vector[1];
    for (int i = 2; i <= n; ++i) {
        vector.push_back(vector[i - 1] + vector[i - 2]);
    }
    return vector[n];
}

double solveLinearAXB(double a, double b) {
    // TODO 10 решите линейное уравнение a*x+b=0 а если решения нет - верните наибольшее значение double - найдите как это сделать в интернете по запросу "so cpp double max value" (so = stackoverflow = сайт где часто можно найти ответы на такие простые запросы), главное НЕ КОПИРУЙТЕ ПРОСТО ЧИСЛО, ПОЖАЛУЙСТААаа
    // если решений сколь угодно много - верните максимальное значение со знаком минус
    if (b == 0)
        return -DBL_MAX;
    if (a == 0)
        return DBL_MAX;
    double x = (-b) / a;
    return x;
}

std::vector<double> solveSquare(double a, double b, double c) {
    // TODO 20 решите квадратное уравнение вида a*x^2+b*x+c=0
    // если корня два - они должны быть упорядочены по возрастанию
    std::vector<double> results;
    if (a == 0) {
        results.push_back(solveLinearAXB(b, c));
    } else {
        double disc = b*b - 4*a*c;
        if (disc > 0) {
            double x1 = (-b - sqrt(disc))/(2*a);
            double x2 = (-b + sqrt(disc))/(2*a);
            results.push_back(std::min(x1, x2));
            results.push_back(std::max(x1, x2));
        } else if (disc == 0) {
            results.push_back((-b)/(2*a));
        } else {
            results.push_back(DBL_MAX);
        }
    }

    // чтобы добавить в вектор элемент - нужно вызвать у него метод push_back:

    return results;
}

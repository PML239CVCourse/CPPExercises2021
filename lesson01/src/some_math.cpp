#include "some_math.h"


int fibbonachiRecursive(int n) {
    if(n == 0)
        return 0;
    if(n == 1)
        return 1;
    return (fibbonachiRecursive(n - 1));
    // TODO 03 реализуйте функцию которая считает числа Фибоначчи - https://ru.wikipedia.org/wiki/%D0%A7%D0%B8%D1%81%D0%BB%D0%B0_%D0%A4%D0%B8%D0%B1%D0%BE%D0%BD%D0%B0%D1%87%D1%87%D0%B8
}

int fibbonachiFast(int n) {
    using namespace std;
    vector <int> fib(n + 1, 0);
    fib[0] = 0;
    fib[1] = 1;
    for(int i = 2; i < n; i++){
        fib[i + 1] = fib[i] + fib[i - 1];
    }
    // TODO 04 реализуйте быструю функцию Фибоначчи с использованием std::vector
    return fib[n];
}

double solveLinearAXB(double a, double b) {
    // TODO 10 решите линейное уравнение a*x+b=0 а если решения нет - верните наибольшее значение double - найдите как это сделать в интернете по запросу "so cpp double max value" (so = stackoverflow = сайт где часто можно найти ответы на такие простые запросы), главное НЕ КОПИРУЙТЕ ПРОСТО ЧИСЛО, ПОЖАЛУЙСТААаа
    // если решений сколь угодно много - верните максимальное значение со знаком минус

    using namespace std;
    double x = 0.0;
    if(a == 0.0){
        if(b == 0.0)
            x = -DBL_MAX;
        else
            x =  DBL_MAX;
    }
    else{
        /*double ans = (-b) / a;
        cout << ans << "\n";*/
        x = (-b) / a;
    }

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

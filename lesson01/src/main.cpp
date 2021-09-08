#include <iostream> // таким образом подключаются системные библиотеки (эта нужна для вывода в консоль)
#include <vector> // подключаем библиотеку для поддержки вектора (массива динамического размера)
using namespace std;

// таким образом подключаются наши функции
#include "simple_sum.h"
#include "fib.h"

int main() {

    cout << "Hello World!" << endl;
    int a = 10;
    cout << "Please enter b=";
    int b;
    cin >> b; // считываем из консоли переменную
    cout << "b=" << b << endl;

    // TODO 01 чтобы телепортироваться внутрь функции - попробуйте удерживая CTRL кликнуть мышкой по функции, например по sum (она засветится как портал)
    int res = sum(a, b);

    cout << "a+b=" << a << "+" << b << "=" << res << endl;

    // TODO 06 выведите в консоль чему равно fibbonachiFast(b), не забудьте что нужно добавить не хватающий инклюд - some_math.h в которой объявлена эта функция
     cout << "fib(b)=" << fib(b)<<endl;

    vector<double> values;
    cout << "values size: " << values.size() << endl;
    values.push_back(10);
    cout << "values size after push_back: " << values.size() << endl;
    values.push_back(20);
    values.push_back(35);
    values.push_back(4);
    cout << "values size after more push_back: " << values.size() << endl;

    // TODO 07 выведите в консоль каждый элемент из динамического массива
    for (int i = 0; i < values.size(); ++i) {
        double x = values[i];
        cout << x << " ";
    }
    int s=0;
    // TODO 08 считывайте числа из консоли (и добавляйте их в вектор) до тех пор пока не будет введен ноль, после чего просуммируйте считанные числа и выведите сумму
     while (true) {
         int i;
         cin >> i;
         s=s+i;
         values.push_back(i);
        if (i==0) {
             break;
        }
     }
    cout << s;
    return 0;
}

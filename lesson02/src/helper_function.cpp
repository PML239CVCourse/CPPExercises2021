#include "helper_function.h"

#include <iostream>


void print2DArray(std::vector<std::vector<int>> array2d) {
    for (int j = 0; j < array2d.size(); ++j) { // пробегаем по каждой строке

        int n = array2d[j].size(); // узнаем число элементов в текущей строке
        // TODO 24 выведите все элементы этой строчки в одну строку в консоли через пробел
        for (int i = 0; i < n; ++i) {
            int value = array2d[j][i]; // взяли j-ую строку, а из нее - i-ый элемент
            // выведите в консоль это число
        }

        // TODO 25 не забудьте что надо делать перевод строки после каждого выведенного ряда чисел
    }
}
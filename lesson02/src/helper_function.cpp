#include "helper_function.h"

#include <iostream>


void print2DArray(std::vector<std::vector<int>> array2d) {
    for (int j = 0; j < array2d.size(); ++j) { // пробегаем по каждой строкe
        int n = array2d[j].size(); // узнаем число элементов в текущей строке

        for (int i = 0; i < n; ++i) {
            std::cout << array2d[j][i] << ' ';
        }

//        // TODO 24 выведите все элементы этой строчки в одну строку в консоли через пробел
//        for (int i = 0; i < n; ++i) {
//            int value = array2d[j][i]; // взяли j-ую строку, а из нее - i-ый элемент
//            // выведите в консоль это число
//            std::cout << array2d[j][i] << std::endl;
//        }

        // TODO 25 не забудьте что надо делать перевод строки после каждого выведенного ряда чисел
    }
}
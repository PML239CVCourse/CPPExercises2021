#include "helper_function.h"

#include <iostream>


void print2DArray(std::vector<std::vector<int>> array2d) {
    for (int j = 0; j < array2d.size(); ++j) { // пробегаем по каждой строке

        int n = array2d[j].size(); // узнаем число элементов в текущей строке
        for (int i = 0; i < n; ++i) {
            int value = array2d[j][i]; // взяли j-ую строку, а из нее - i-ый элемент
            std::cout<<value<<' ';
            // выведите в консоль это число
        }
        std::cout<<'\n';
    }
}
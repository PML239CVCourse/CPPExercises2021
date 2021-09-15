#include "helper_function.h"

#include <iostream>


void print2DArray(const std::vector<std::vector<int>>& array2d) {
    for (int j = 0; j < array2d.size(); ++j) { // пробегаем по каждой строке

        int n = array2d[j].size(); // узнаем число элементов в текущей строке
        // TODO 24 выведите все элементы этой строчки в одну строку в консоли через пробел
        for (int i = 0; i < n; ++i) {
            int value = array2d[j][i]; // взяли j-ую строку, а из нее - i-ый элемент
            std::cout << value << " ";
        }

        // TODO 25 не забудьте что надо делать перевод строки после каждого выведенного ряда чисел
        std::cout << std::endl;
    }
}

bool checkRows(const std::vector<std::vector<int>>& array2d) {
    for (int j = 0; j < array2d.size(); ++j) { // пробегаем по каждой строке

        int n = array2d[j].size(); // узнаем число элементов в текущей строке
        // TODO 24 выведите все элементы этой строчки в одну строку в консоли через пробел
        bool flag = true;
        for (int i = 0; i < n; ++i) {
            int value = array2d[j][i]; // взяли j-ую строку, а из нее - i-ый элемент
            flag = flag && value == 1;
        }

        // TODO 25 не забудьте что надо делать перевод строки после каждого выведенного ряда чисел
        if(flag)
            return true;
    }
    return false;
}

bool checkColumns(const std::vector<std::vector<int>>& array2d) {

    for (int i = 0; i < array2d.front().size(); ++i) {
        // TODO 24 выведите все элементы этой строчки в одну строку в консоли через пробел
        bool flag = true;
        for (int j = 0; j < array2d.size(); ++j) { // пробегаем по каждой строке
            if(array2d[j].size() <= i)
                return false;
            int value = array2d[j][i]; // взяли j-ую строку, а из нее - i-ый элемент
            flag = flag && value == 1;
        }

        // TODO 25 не забудьте что надо делать перевод строки после каждого выведенного ряда чисел
        if(flag)
            return true;
    }
    return false;
}
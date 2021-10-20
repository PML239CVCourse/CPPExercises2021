#include <iostream>
#include <libutils/rasserts.h>
#include "disjoint_set.h"

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"


DisjointSet::DisjointSet(int size)
{
    s = size;
    value = std::vector<int>(size);
    parents = std::vector<int>(size);
    ranks = std::vector<int>(size);
    sizes = std::vector<int>(size);
    for (int i = 0; i < size; ++i) {
        parents[i] = ROOT;
        ranks[i] = 0;
        sizes[i] = 1;
    }
    // TODO - заполните вектора так чтобы на этапе конструирования эта система непересекающихся множеств состояла из:
    // size элементов, каждый из которых состоит в своем множестве (а значит ссылка на родителя у него - на корень, т.е. на ROOT, а какой тогда ранк и размер у множества каждого элемента?)
    // TODO заполнить parents
    // TODO заполнить ranks
    // TODO заполнить sizes
}

std::vector<int> DisjointSet::GetParents(){
    return parents;
}

int	DisjointSet::get_set(int element)
{
    // TODO по номеру элемента нужно переходя по ссылкам на родителя дойти до самого верхнего элемента,
    // номер этого корневого элемента - номер множества на данный момент (кто вверху тот и главный, множество названо в его честь)
//    int first_of_their_kind = element;
//    int a = element;
//    while (true){
//        if (a == ROOT){
//            return first_of_their_kind;
//        }
//        else{
//            first_of_their_kind = a;
//            a = parents[first_of_their_kind];
//            parents[first_of_their_kind] = get_set(a);
//            std::cout << a << std::endl;
//        }
//    }
//    for (int i = 0; i < parents.size(); ++i) {
//        std::cout << parents[i]<< " ";
//    }
//    std::cout << parents[element] << "-" << element << std::endl;
    if (parents[element] != ROOT){
        parents[element] = get_set(parents[element]);
        return parents[element];
    }
    return element;
}

int DisjointSet::count_differents() const
{
    // TODO посчитать сколько разных множеств (подсказка: в каждом множестве ровно один корень, а корень - это тот у кого родитель = ROOT)
    int count = 0;
    for (size_t i = 0; i < this->parents.size(); i++) {
        if (parents[i] == ROOT){
            count++;
        }
    }
    return count;
}

int DisjointSet::get_set_size(int element)
{
//    std::cout << sizes[get_set(element)] << "+" << std::endl;
    // TODO сообщить сколько элементов в множестве, которому принадлежит данный элемент (да, это очень просто)
    return sizes[get_set(element)];
}

int	DisjointSet::union_sets(int element0, int element1)
{
    int size = get_set_size(element0) + get_set_size(element1);
//    std::cout << size;
    // TODO узнать корневые элементы этих двух элементов и посмотрев на них - решить,
    // кого из них подвесить к другому (тем самым объединить два множества)
    // при этом стоит подвешивать менее высокое дерево к более высокому (т.е. учитывая ранк),
    // а так же важно не забыть после переподвешивания у корневого элемента обновить ранк и размер множества
    rassert(element1 < s, element1)
    rassert(element0 < s, element0)
    if (get_set(element1) != get_set(element0)){
        if (ranks[get_set(element0)] > ranks[get_set(element1)]){
            parents[get_set(element1)] = get_set(element0);
        }
        else if (ranks[get_set(element0)] < ranks[get_set(element1)]){
            parents[get_set(element0)] = get_set(element1);
        }
        else{
            if (get_set_size(element0) > get_set_size(element1)){
                parents[get_set(element0)] = get_set(element1);
            }
            else{
                parents[get_set(element1)] = get_set(element0);
            }
            ranks[get_set(element1)]++;
        }
        sizes[get_set(element1)] = size;
        //    std::cout << size << "SEF:LJSEF" << std::endl;
        return 0;
    }
    return 1;



     // вернуть номер объединенного множества
}

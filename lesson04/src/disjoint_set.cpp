#include "disjoint_set.h"

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"


DisjointSet::DisjointSet(int size)
{
    parents = std::vector<int>(size);
    ranks = std::vector<int>(size);
    sizes = std::vector<int>(size);
    // TODO - заполните вектора так чтобы на этапе конструирования эта система непересекающихся множеств состояла из:
    // size элементов, каждый из которых состоит в своем множестве (а значит ссылка на родителя у него - на корень, т.е. на ROOT, а какой тогда ранк и размер у множества каждого элемента?)
    // TODO заполнить parents
    // TODO заполнить ranks
    // TODO заполнить sizes
    for(size_t i = 0; i < size; i++){
        parents[i] = ROOT;
        ranks[i] = 1;
        sizes[i] = 1;
    }
}

int	DisjointSet::get_set(int element)
{
    // TODO по номеру элемента нужно переходя по ссылкам на родителя дойти до самого верхнего элемента,
    // номер этого корневого элемента - номер множества на данный момент (кто вверху тот и главный, множество названо в его честь)
    int first_of_their_kind = 0;

    if(this->parents[element] == -1)
        first_of_their_kind = element;
    else
        first_of_their_kind = get_set(this->parents[element]);

    return first_of_their_kind;
}

int DisjointSet::count_differents() const
{
    // TODO посчитать сколько разных множеств (подсказка: в каждом множестве ровно один корень, а корень - это тот у кого родитель = ROOT)
    int count = 0;
    for (size_t i = 0; i < this->parents.size(); i++) {
        if(this->parents[i] == -1)
            count++;
    }
    return count;
}

int DisjointSet::get_set_size(int element)
{
    // TODO сообщить сколько элементов в множестве, которому принадлежит данный элемент (да, это очень просто)
    int size_of_elements = this->sizes[get_set(element)];
    return size_of_elements;
}

int	DisjointSet::union_sets(int element0, int element1)
{
    // TODO узнать корневые элементы этих двух элементов и посмотрев на них - решить,
    // кого из них подвесить к другому (тем самым объединить два множества)
    // при этом стоит подвешивать менее высокое дерево к более высокому (т.е. учитывая ранк),
    // а так же важно не забыть после переподвешивания у корневого элемента обновить ранк и размер множества

    int num_elem0 = get_set(element0);
    int num_elem1 = get_set(element1);

    int fin_v_elem = 0;
    if(this->ranks[num_elem0] > this->ranks[num_elem1]){
        fin_v_elem = num_elem0;
        this->parents[num_elem1] = fin_v_elem;
        this->ranks[num_elem1] = 0;
        this->sizes[num_elem1] = 0;
    }
    else{
        fin_v_elem = num_elem1;
        this->parents[num_elem0] = fin_v_elem;
        this->ranks[num_elem0] = 0;
        this->sizes[num_elem0] = 0;
    }


    return fin_v_elem; // вернуть номер объединенного множества
}

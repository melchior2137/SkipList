#include "SkipList.h"

void insert_and_contains_test(){
    SkipList<int> lista(3, 0.5);
    lista.insert(2);
    lista.insert(6);
    lista.insert(7);
    lista.insert(25);
    lista.insert(3);
    lista.insert(13);
    lista.insert(-5);
    lista.insert(9);

    assert(lista.contains(13));
    assert(!lista.contains(14));
    assert(lista.contains(2));
    assert(lista.contains(7));
    assert(!lista.contains(99));
    assert(lista.contains(-5));

}

void remove_test(){
    srand((unsigned)time(NULL));
    SkipList<int> lista(3, 0.5);
    lista.insert(25);
    lista.insert(62);
    lista.insert(7);
    lista.insert(25);
    lista.insert(3);
    lista.insert(13);
    lista.insert(91);
    // lista.display();

    assert(lista.remove(62) == 1 && !lista.contains(6));
    assert(lista.remove(8) == -1 && !lista.contains(8));
    assert(lista.remove(9) == -1 && !lista.contains(9));
    assert(lista.remove(56) == -1 && !lista.contains(56));
    assert(lista.remove(25) == 1 && !lista.contains(25));
    assert(lista.remove(7) == 1 && !lista.contains(7));

}

void display_test(){
    SkipList<double> lista(3, 0.5);
    lista.insert(2.1);
    lista.insert(6.1);
    lista.insert(7.1);
    lista.insert(25.1);
    lista.insert(3.1);
    lista.insert(13.1);
    lista.insert(9);
    lista.insert(-5);


    lista.display();
}

int main()
{
    srand((unsigned)time(NULL));
    insert_and_contains_test();
    remove_test();
    display_test();
}
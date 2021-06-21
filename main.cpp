#include <iostream>
#include "balansirajuce.h"
#include "balansirajuce.cpp"

int main() {
    BalansirajuceStablo<int> stablo;

    stablo.dodaj(10);
    stablo.dodaj(6);
    stablo.dodaj(13);
    stablo.dodaj(8);
    stablo.dodaj(4);
    stablo.dodaj(20);
    stablo.dodaj(11);
    stablo.dodaj(16);
    stablo.dodaj(26);
    stablo.dodaj(14);
    stablo.dodaj(18);

    stablo.izbrisi(20);
    stablo.izbrisi(18);

    std::cout << stablo;
    return 0;
}


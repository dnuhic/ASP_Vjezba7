#include <iostream>
#include "NizMapa/NizMapa.h"

int main() {
    NizMapa<int, int> m;
    m[1] = 1;
    m[2] = 2;
    m[3] = 3;
    m[4] = 4;
    m[5] = 5;
    m[6] = 6;
    m[7] = 7;
    m[8] = 8;
    MapaIterator<int, int> it(m);
    for(int i = 0; i < m.brojElemenata(); i++) {
        std::cout << it.trenutniKljuc() << " - " << it.trenutnaVrijednost() << std::endl;
        it.sljedeci();
    }
    return 0;
}

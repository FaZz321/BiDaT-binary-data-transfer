// BiDaT includes:
#include "BiDaT/BiRecord.h"

////////////////
// DEBUG ONLY //
////////////////

#include <iostream>
#include <fstream>
using namespace std;

/*
void nodesTest() {
    BiReal* n1 = new BiReal(1.2);
    BiReal* n2 = new BiReal(2.3);
    BiInteger* n3 = new BiInteger(5);

    BiList na;

    na.pushFront(n1);
    na.pushFront(n2);
    na.pushFront(n3);

    BiReal* n11 = (BiReal*) na.byIndex(0);
    BiReal* n12 = (BiReal*) na.byIndex(1);
    BiInteger* n13 = (BiInteger*) na.byIndex(2);

    double v1 = n11->getValue();
    double v2 = n12->getValue();
    int v3 = n13->getValue();

    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << v3 << std::endl;

    system("pause");
}
*/

void accessModeTests() {
    ifstream test_file;
    test_file.open("test_cases/SingleString.bdt", ios::binary | ios::ate);

    streampos size = test_file.tellg();
    char* memblock = new char[size];
    test_file.seekg(0, ios::beg);

    test_file.read(memblock, size);

    test_file.close();

    BiRecord record((unsigned char*) memblock);
    record.moderateMode();

    delete[] memblock;
}


int main() {
    accessModeTests();

	return 0;
}

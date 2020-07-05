#include "BiDaT.h"

////////////////
// DEBUG ONLY //
////////////////

#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

////////////////

/*
    ifstream test_file;
    test_file.open("test_cases/SingleString.bdt", ios::binary | ios::ate);

    streampos size = test_file.tellg();
    char* memblock = new char[size];
    test_file.seekg(0, ios::beg);

    test_file.read(memblock, size);

    test_file.close();

    BiRecord record((unsigned char*) memblock);
    record.moderateMode();

    record.get_root();

    delete[] memblock;
*/

/*
void readTest() {
    // read message from file:
    ifstream test_file;
    test_file.open("test_cases/SingleReal.bdt", ios::binary | ios::ate);

    streampos size = test_file.tellg();
    char* memblock = new char[size];
    test_file.seekg(0, ios::beg);

    test_file.read(memblock, size);

    test_file.close();

    // do stuff:
    BiRecord record(memblock);
    record.moderateMode();
}
*/

void nodesTest() {
    // scalar types:
    BiReal* n1 = new BiReal();
    BiInteger* n2 = new BiInteger();
    BiBool* n3 = new BiBool();

    // complex scalar types:
    BiString* n4 = new BiString();
    BiBinary* n5 = new BiBinary();

    // list types:
    BiList* iList = new BiList();
    BiNamedList* nList = new BiNamedList();
    BiList* nestedList = new BiList();

    iList->pushBack(n4);
    iList->pushBack(n2);
    iList->pushBack(n1);

    nList->push("VAR1", n5);
    nList->push("VAR2", n3);

    nestedList->pushBack(nList);
    nestedList->pushBack(iList);

    // init:
    n1->setValue(3.14);
    n2->setValue(314);
    n3->setValue(true);
    n4->setValue("hello world!");
    n5->setValue("0123456789", 10);

    // output:
    std::cout << "SCALAR TYPES:" << std::endl;
    std::cout << n1->getValue() << std::endl;
    std::cout << n2->getValue() << std::endl;
    std::cout << n3->getValue() << std::endl;

    std::cout << "COMPLEX SCALAR TYPES:" << std::endl;
    const char* str = n4->getValue();
    unsigned int size = n4->getSize();
    std::cout << str << " (length = "<< size << " characters)" << std::endl;

    const char* bytes = n5->getValue();
    size = n5->getSize();
    for (unsigned int i = 0; i < size; i++) {
        std::cout << bytes[i] << ' ';
    }
    std::cout << "(length = " << size << " bytes)" << std::endl;

    std::cout << "LIST TYPES:" << std::endl;
    std::cout << "  indexed list:" << std::endl;
    size = iList->getSize();
    std::cout << "    number of values:" << size << std::endl;
    for (unsigned int i = 0; i < size; i++) {
        BiNode* node = (*iList)[i];

        if (node->getType() == BI_REAL) {
            BiReal* n_real = (BiReal*) node;
            std::cout << "    " << i << ": " << n_real->getValue() << std::endl;
        }

        if (node->getType() == BI_INTEGER) {
            BiInteger* n_int = (BiInteger*) node;
            std::cout << "    " << i << ": " << n_int->getValue() << std::endl;
        }

        if (node->getType() == BI_STRING) {
            BiString* n_str = (BiString*) node;
            unsigned int str_size = n_str->getSize();
            std::cout << "    " << i << ": " << n_str->getValue() << " (length = "<< str_size << " characters)" << std::endl;
        }
    }

    std::cout << "  named list:" << std::endl;
    size = nList->getSize();
    std::cout << "    number of key-value tuples: " << size << std::endl;
    for (unsigned int i = 0; i < size; i++) {
        const char* name = nList->getName(i);
        BiNode* node = (*nList)[name];

        if (node->getType() == BI_BOOL) {
            BiBool* n_bool = (BiBool*) node;
            std::cout << "    " << name << ": " << n_bool->getValue() << std::endl;
        }

        if (node->getType() == BI_BINARY) {
            BiBinary* n_binary = (BiBinary*) node;

            std::cout << "    " << name << ": ";
            unsigned int binary_size = n_binary->getSize();
            const char* binary_bytes = n_binary->getValue();
            for (unsigned int i = 0; i < binary_size; i++) {
                std::cout << binary_bytes[i] << ' ';
            }
            std::cout << "(length = " << size << " bytes)" << std::endl;
        }
    }

    std::cout << "  nested list:" << std::endl;
    size = nestedList->getSize();
    for (unsigned int i = 0; i < size; i++) {
        BiNode* node = (*nestedList)[i];

        if (node->getType() == BI_LIST) {
            BiList* n_ilist = (BiList*) node;
            std::cout << "    indexed list with size of " << n_ilist->getSize() << std::endl;
        }

        if (node->getType() == BI_NAMED_LIST) {
            BiNamedList* n_nlist = (BiNamedList*) node;
            std::cout << "    named list with size of " << n_nlist->getSize() << std::endl;
        }
    }

     delete nestedList;
}

void speedTest() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    BiList* i_list = new BiList();
    BiReal* node;
    for (unsigned int i = 0; i < 100000; i++) {
        node = new BiReal();
        node->setValue(i);
        i_list->pushBack(node);
    }
    delete i_list;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (10.0*10*10*10*10*10) << "[s.]" << std::endl;
}


int main() {
    // nodesTest(); success!
    speedTest();

	return 0;
}

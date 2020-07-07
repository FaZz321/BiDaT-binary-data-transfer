#include "BiDaT.h"

////////////////
// DEBUG ONLY //
////////////////

#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

////////////////

void printNode(BiNode* node) {
    BI_NODE_TYPE type = node->getType();
    switch(type) {
        case BI_NODE_TYPE_INT: {
            BiInteger* n = (BiInteger*) node;
            std::cout << "Integer: " << n->getValue() << std::endl;
            break;
        } case BI_NODE_TYPE_REAL: {
            BiReal* n = (BiReal*) node;
            std::cout << "Real: " << n->getValue() << std::endl;
            break;
        } case BI_NODE_TYPE_BOOL: {
            BiBool* n = (BiBool*) node;
            std::cout << "Bool: " << n->getValue() << std::endl;
            break;
        } case BI_NODE_TYPE_STR: {
            BiString* n = (BiString*) node;
            size_t size = n->getSize();
            std::cout << "String (size = " << size << " characters): " << n->getValue() << std::endl;
            break;
        } case BI_NODE_TYPE_BINARY: {
            BiBinary* n = (BiBinary*) node;
            size_t size = n->getSize();
            const BI_BYTE_T* bytes = n->getValue();
            std::cout << "Binary (size = " << size << " bytes): ";
            for(unsigned int i = 0; i < size; i++)
                std::cout << (int)bytes[i] << ", ";
            std::cout << std::endl;
            break;
        } case BI_NODE_TYPE_LIST: {
            BiList* n = (BiList*) node;
            size_t size = n->getSize();
            std::cout << "LIST (" << size << " items): " << std::endl;
            for (unsigned int i = 0; i < size; i++) {
                std::cout << "["<< i << "] ";
                BiNode& subNode = (*n)[i];
                printNode(&subNode);
            }
            std::cout << "END OF LIST" << std::endl;
            break;
        } case BI_NODE_TYPE_NAMED_LIST: {
            BiNamedList* n = (BiNamedList*) node;
            size_t size = n->getSize();
            std::cout << "NAMED LIST (" << size << " items): " << std::endl;
            const char* name;
            for (unsigned int i = 0; i < size; i++) {
                name = n->getName(i);
                std::cout << "[\""<< name << "\"] ";
                BiNode& subNode = (*n)[name];
                printNode(&subNode);
            }
            std::cout << "END OF NAMED LIST" << std::endl;
            break;
        }
    }
}

void nodesTest() {
    BiInteger n_int(1235711);
    BiReal n_real(3.14);
    BiBool n_bool(true);
    BiString n_string("hello world!");
    unsigned char* bytes = new unsigned char[10];
    for (int i = 0; i < 10; i++) {
        bytes[i] = i;
    }
    BiBinary n_binary(bytes, 10);
    delete bytes;

    BiList indexed_list = BiList();
    indexed_list.pushBack(1);
    indexed_list.pushBack(2);
    indexed_list.pushBack(3);

    BiNamedList named_list = BiNamedList();
    named_list.push("VAR1", false);
    named_list.push("VAR2", "some_string");
    named_list.push("VAR3", 3.1415);

    BiList nested_list = BiList();
    nested_list.pushBackNewNode(new BiList());
    nested_list.pushBackNewNode(new BiList());
    nested_list.pushBackNewNode(new BiList());

    BiList* cur_list = (BiList*) &nested_list[0];
    cur_list->pushBack(1);
    cur_list->pushBack(2);
    cur_list->pushBack(3);

    cur_list = (BiList*) &nested_list[1];
    cur_list->pushBack("string");
    cur_list->pushBack(true);
    bytes = new unsigned char[11];
    for (int i = 0; i < 11; i++) {
        bytes[i] = i;
    }
    cur_list->pushBack(bytes, 11);
    delete bytes;

    cur_list = (BiList*) &nested_list[2];

    BiNamedList* newList = new BiNamedList();
    newList->pushNewNode("MY_AGE(or_somth)", new BiInteger(27));
    cur_list->pushBackNewNode(newList);

    std::cout << "SIMPLE SCALAR:" << std::endl;
    printNode(&n_int);
    printNode(&n_real);
    printNode(&n_bool);
    std::cout << " \n" << "COMPLEX SCALAR:" << std::endl;
    printNode(&n_string);
    printNode(&n_binary);
    std::cout << " \n" << "LISTS:" << std::endl;
    printNode(&indexed_list);
    printNode(&named_list);
    std::cout << " \n" << "NESTED LIST:" << std::endl;
    printNode(&nested_list);
}

void speedTest() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    BiList* i_list = new BiList();
    for (unsigned int i = 0; i < 1000000; i++) {
        i_list->pushBack(3.14);
    }

    delete i_list;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (10.0*10*10*10*10*10) << "[s.]" << std::endl;
}

void memoryTest() {
    // integer (success):
    BiInteger* int_node = new BiInteger();
    delete int_node;
    int_node = new BiInteger(100);
    delete int_node;

    // real (success):
    BiReal* real_node = new BiReal();
    delete real_node;
    real_node = new BiReal(50.0);
    delete real_node;

    // string (success):
    BiString* string_node = new BiString();
    delete string_node;
    string_node = new BiString("hello");
    delete string_node;

    // bool (success):
    BiBool* bool_node = new BiBool();
    delete bool_node;
    bool_node = new BiBool(true);
    delete bool_node;

    // binary (success):
    BiBinary* binary_node = new BiBinary();
    delete binary_node;
    BI_BYTE_T* bytes = new BI_BYTE_T[11];
    for (int i = 0; i < 11; i++) {
        bytes[i] = i;
    }
    binary_node = new BiBinary(bytes, 11);
    delete bytes;
    delete binary_node;

    // indexed list 1 (success):
    BiList* indexed_list = new BiList();
    indexed_list->pushBack(3);
    indexed_list->pushBack(2);
    indexed_list->pushBack(1);
    indexed_list->pushFront(7);
    indexed_list->pushFront(8);
    indexed_list->pushFront(9);
    indexed_list->insert(4,3);
    indexed_list->insert(5,4);
    indexed_list->insert(6,5);
    delete indexed_list;

    // indexed list 2 (success):
    indexed_list = new BiList();
    indexed_list->pushBackNewNode(new BiInteger(1));
    indexed_list->pushFrontNewNode(new BiInteger(3));
    indexed_list->insertNewNode(new BiInteger(2),1);
    delete indexed_list;

    // named list 1 (success):
    BiNamedList* named_list = new BiNamedList();
    named_list->push("VAR1", 1);
    named_list->push("VAR2", "string");
    bytes = new BI_BYTE_T[6];
    for (int i = 0; i < 6; i++) {
        bytes[i] = i;
    }
    binary_node = new BiBinary(bytes, 11);
    named_list->push("VAR3", bytes, 6);
    delete bytes;
    delete named_list;

    // named list 2:
    named_list = new BiNamedList();
    named_list->pushNewNode("VAR1", new BiInteger(1));
    named_list->pushNewNode("VAR2", new BiReal(2.25));
    named_list->pushNewNode("VAR3", new BiString("hello world"));
    delete named_list;

    // nested lists:
    BiList* rootList = new BiList();

    named_list = new BiNamedList();
    named_list->pushNewNode("sublist1", new BiList());
    named_list->pushNewNode("sublist2", new BiNamedList());
    rootList->pushBackNewNode(named_list);

    indexed_list = new BiList();
    indexed_list->pushBackNewNode(new BiList());
    indexed_list->pushBackNewNode(new BiNamedList());
    rootList->pushBackNewNode(indexed_list);
    delete rootList;
}

void readTest() {
    ifstream test_file;
    test_file.open("test_cases/NestedArray4.bdt", ios::binary | ios::ate);

    streampos size = test_file.tellg();
    char* memblock = new char[size];
    test_file.seekg(0, ios::beg);

    test_file.read(memblock, size);
    test_file.close();

    BiRecord rec;
    BiParser parser = BiParser((BI_BYTE_T*)memblock);
    parser.parseMessage(rec);

    printNode(rec.getRoot());
    delete memblock;
}

void writeTest() {
    // TOFIX: implement
}


int main() {
    // nodesTest(); success!
    // speedTest(); success!
    // memoryTest(); success!
    // readTest();  success!
    writeTest();

	return 0;
}

#include "BiDemo.h"

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
            if (n->getValue())
                std::cout << "Bool: true" << std::endl;
            else
                std::cout << "Bool: false" << std::endl;
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
    // integer:
    BiInteger n_int(1235711);

    printNode(&n_int);

    // real:
    BiReal n_real(3.14);

    printNode(&n_real);

    // bool:
    BiBool n_bool(true);

    printNode(&n_bool);

    // string:
    BiString n_string("hello world!");

    printNode(&n_string);

    // binary:
    unsigned char* bytes = new unsigned char[3];
    for (int i = 1; i < 4; i++) {
        bytes[i-1] = i*2;
    }
    BiBinary n_binary(bytes, 3);
    delete bytes;

    printNode(&n_binary);

    // indexed list:
    BiList indexed_list = BiList();
    indexed_list.pushBack(1);
    indexed_list.pushBack(2);
    indexed_list.pushBack(3);

    printNode(&indexed_list);

    // named list:
    BiNamedList named_list = BiNamedList();
    named_list.push("VAR1", false);
    named_list.push("VAR2", "some_string");
    named_list.push("VAR3", 3.1415);

    printNode(&named_list);

    // nested lists:
    BiList nested_list = BiList();
    nested_list.pushBackNewNode(new BiList());
    nested_list.pushBackNewNode(new BiList());
    nested_list.pushBackNewNode(new BiList());

    // sublist 1:
    BiList* cur_list = (BiList*) &nested_list[0];
    cur_list->pushBack(1);
    cur_list->pushBack(2);
    cur_list->pushBack(3);

    // sublist 2:
    cur_list = (BiList*) &nested_list[1];
    cur_list->pushBack("string");
    cur_list->pushBack(true);
    bytes = new unsigned char[5];
    for (int i = 0; i < 5; i++) {
        bytes[i] = i;
    }
    cur_list->pushBack(bytes, 5);
    delete bytes;

    // sublist 3:
    cur_list = (BiList*) &nested_list[2];

    BiNamedList* newList = new BiNamedList();
    newList->pushNewNode("MY_AGE", new BiInteger(25));
    cur_list->pushBackNewNode(newList);

    printNode(&nested_list);
}



void speedTest() {
    // test 1:

    std::cout << "Adding 100 000 integer values to a indexed list:" << std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    BiList* i_list = new BiList();
    for (int i = 0; i < 100000; i++) {
        i_list->pushBack(i);
    }

    delete i_list;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (10.0*10*10*10*10*10) << " seconds" << std::endl;

    // test 2:

    std::cout << "Creating 1000 x 1000 multiplication table:" << std::endl;
    begin = std::chrono::steady_clock::now();

    i_list = new BiList();
    for (int i = 1000; i > 0; i--) {
        BiList* temp = new BiList();
        for (int j = 1000; j > 0; j--) {
            temp->pushBack(i*j);
        }
        i_list->pushBackNewNode(temp);
    }

    end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (10.0*10*10*10*10*10) << " seconds" << std::endl;

    // test 3:

    std::cout << "Zeroing multiplication table:" << std::endl;
    begin = std::chrono::steady_clock::now();

    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            BiNode& cur_node = (*i_list)[i][j];
            BiInteger* cur_integer = (BiInteger*) &cur_node;
            cur_integer->setValue(0);
        }
    }
    delete i_list;

    end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (10.0*10*10*10*10*10) << " seconds" << std::endl;
}



void memoryTest() {
    // integer:
    BiInteger* int_node = new BiInteger();
    delete int_node;
    int_node = new BiInteger(100);
    delete int_node;

    // real:
    BiReal* real_node = new BiReal();
    delete real_node;
    real_node = new BiReal(50.0);
    delete real_node;

    // string:
    BiString* string_node = new BiString();
    delete string_node;
    string_node = new BiString("hello");
    delete string_node;

    // bool:
    BiBool* bool_node = new BiBool();
    delete bool_node;
    bool_node = new BiBool(true);
    delete bool_node;

    // binary:
    BiBinary* binary_node = new BiBinary();
    delete binary_node;
    BI_BYTE_T* bytes = new BI_BYTE_T[5];
    for (int i = 0; i < 5; i++) {
        bytes[i] = i;
    }
    binary_node = new BiBinary(bytes, 5);
    delete bytes;
    delete binary_node;

    // indexed list 1:
    BiList* indexed_list = new BiList();
    indexed_list->pushBack(3);
    indexed_list->pushFront(1);
    indexed_list->insert(2,1);
    delete indexed_list;

    // indexed list 2:
    indexed_list = new BiList();
    indexed_list->pushBackNewNode(new BiInteger(1));
    indexed_list->pushFrontNewNode(new BiInteger(3));
    indexed_list->insertNewNode(new BiInteger(2),1);
    delete indexed_list;

    // named list 1:
    BiNamedList* named_list = new BiNamedList();
    named_list->push("VAR1", 1);
    named_list->push("VAR2", "string");
    bytes = new BI_BYTE_T[3];
    for (int i = 0; i < 3; i++) {
        bytes[i] = i;
    }
    named_list->push("VAR3", bytes, 3);
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



void readTest(const char* file_name) {
    std::ifstream test_file;
    test_file.open(file_name, std::ios::binary | std::ios::ate);

    if (!test_file)
        throw 1;  // ERROR: CAN'T READ A FILE
    std::streampos size = test_file.tellg();
    char* memblock = new char[size];
    test_file.seekg(0, std::ios::beg);

    test_file.read(memblock, size);
    test_file.close();

    BiRecord rec;
    BiParser parser = BiParser((BI_BYTE_T*)memblock);
    parser.parse(rec);

    printNode(rec.getRoot());
    delete memblock;
}



void writeTest(const char* file_name) {
    std::ofstream test_file;
    test_file.open(file_name, std::ios::binary);

    BiRecord rec = BiRecord(new BiInteger());
    BiInteger* root = (BiInteger*) rec.getRoot();

    root->setValue(15);

    rec.encode();
    printNode(rec.getRoot());

    test_file.write((const char*) rec.getMessage(), rec.getMessageSize());
    test_file.close();
}

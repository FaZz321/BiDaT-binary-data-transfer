#include "BiLists.h"

/////////////////
// BiAbstrList //
/////////////////

BiAbstrList::BiAbstrList(BI_TYPE type, unsigned int size): BiNode(type) {
    this->size = size;
    if (this->size) {
        BiNode** zero_values = new BiNode*[this->size];
        for (unsigned int i = 0; i < this->size; i++) {
            zero_values[i] = 0;
        }
        this->values = zero_values;
    }
}

BiAbstrList::~BiAbstrList() {
    if (this->values) {
        for (unsigned int i = 0; i < this->size; i++) {
            if (values[i]) {
                delete values[i];
            }
        }
    }
}

////////////
// BiList //
////////////

BiList::BiList():BiAbstrList(BI_LIST) {}

void BiList::insertNode(unsigned int index, BiNode* new_node) {
    if (index > this->size)
        throw 1;  // TOFIX: make BI_ERR_* enum

    BiNode** new_values = new BiNode*[this->size+1];
    unsigned int ptr_size = sizeof(BiNode*);

    if (index == 0) {
        memcpy(new_values+1, this->values, ptr_size*this->size);
        new_values[0] = new_node;
    } else if (index == this->size) {
        memcpy(new_values, this->values, ptr_size*this->size);
        new_values[this->size] = new_node;
    } else {
        memcpy(new_values, this->values, ptr_size*index);
        new_values[index] = new_node;
        memcpy(new_values+index+1, this->values+index, ptr_size*(this->size-index));
    }

    this->values = new_values;
    this->size += 1;
}

void BiList::pushBack(BiNode* newNode) {
    this->insertNode(0, newNode);
}

void BiList::pushFront(BiNode* newNode) {
    this->insertNode(this->size, newNode);
}

BiNode* BiList::byIndex(unsigned int index) {
    return this->values[index];
}

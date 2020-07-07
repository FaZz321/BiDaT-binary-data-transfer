#include "BiRecord.h"

//////////////
// BiRecord //
//////////////

BiRecord::BiRecord() {
    this->root = 0;
}

BiRecord::~BiRecord() {
    if (this->root)
        delete this->root;
}

void BiRecord::setRoot(BiNode* newRoot) {
    if (this->root)
        delete this->root;

    this->root = newRoot;
}

BiNode* BiRecord::getRoot() {
    return this->root;
}


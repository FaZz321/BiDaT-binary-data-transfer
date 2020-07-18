#include "BiRecord.h"

///////////////////
// BiRecordChunk //
///////////////////;

BiRecordChunk::BiRecordChunk(unsigned int size) {
    this->size = size;
    this->data = new unsigned char[size];
}

BiRecordChunk::~BiRecordChunk() {
    delete this->data;
}

//////////////
// BiRecord //
//////////////

BiRecord::BiRecord() {
    this->message_size = 0;
    this->message = 0;
    this->root = 0;
}

BiRecord::BiRecord(BiNode* root) {
    this->message_size = 0;
    this->message = 0;
    this->root = root;
}

BiRecord::~BiRecord() {
    if (this->message)
        delete this->message;
    if (this->root)
        delete this->root;

    size_t size = this->chunks.size();
    for (unsigned int i = 0; i < size; i++) {
        delete this->chunks[i];
    }
}

void BiRecord::encode() {
    BiRecordChunk* leftBracket = new BiRecordChunk(1);
    *leftBracket->data = 0x00;
    this->chunks.push_back(leftBracket);

    this->_encodeNode(this->root);

    BiRecordChunk* rightBracket = new BiRecordChunk(1);
    *rightBracket->data = 0xFF;
    this->chunks.push_back(rightBracket);

    // put chunks together:
    unsigned int sum_size = 0;
    unsigned int n_chunks = this->chunks.size();
    for(unsigned int i = 0; i < n_chunks; i++)
        sum_size += this->chunks[i]->size;

    unsigned char* result = new unsigned char[sum_size];
    unsigned int cursor = 0;
    unsigned int size;
    for (unsigned int i = 0; i < n_chunks; i++) {
        size = this->chunks[i]->size;
        memcpy(result+cursor, this->chunks[i]->data, size);
        cursor += size;
    }

    this->message = result;
    this->message_size = sum_size;
}

void BiRecord::_encodeNode(BiNode* node) {
    BI_NODE_TYPE type = node->getType();
    switch(type) {
        case BI_NODE_TYPE_INT:
            this->_encodeInt((BiInteger*) node);
            break;
        case BI_NODE_TYPE_REAL:
            this->_encodeReal((BiReal*) node);
            break;
        case BI_NODE_TYPE_BOOL:
            this->_encodeBool((BiBool*) node);
            break;
        case BI_NODE_TYPE_STR:
            this->_encodeStr((BiString*) node);
            break;
        case BI_NODE_TYPE_BINARY:
            this->_encodeBinary((BiBinary*) node);
            break;
        case BI_NODE_TYPE_LIST:
            this->_encodeList((BiList*) node);
            break;
        case BI_NODE_TYPE_NAMED_LIST:
            this->_encodeNamedList((BiNamedList*) node);
            break;
    }
}

void BiRecord::_encodeInt(BiInteger* node) {
    BiRecordChunk* result = new BiRecordChunk(5);
    result->data[0] = 0x01;
    *((BI_INT32_T*) (result->data+1)) = node->getValue();
    this->chunks.push_back(result);
}

void BiRecord::_encodeReal(BiReal* node) {
    BiRecordChunk* result = new BiRecordChunk(9);
    result->data[0] = 0x02;
    *((BI_FLOAT64_T*) (result->data+1)) = node->getValue();
    this->chunks.push_back(result);
}

void BiRecord::_encodeBool(BiBool* node) {
    BiRecordChunk* result = new BiRecordChunk(2);
    result->data[0] = 0x03;
    *((BI_BOOL_T*) (result->data+1)) = node->getValue();
    this->chunks.push_back(result);
}

void BiRecord::_encodeStr(BiString* node) {
    BI_UINT32_T size = node->getSize() + 1;
    BiRecordChunk* result = new BiRecordChunk(size+1);  // (string length + \0) + type
    result->data[0] = 0x04;
    memcpy(result->data+1, node->getValue(), size);
    this->chunks.push_back(result);
}

void BiRecord::_encodeBinary(BiBinary* node) {
    BI_UINT32_T size = node->getSize();
    if (size > 255) {
        BiRecordChunk* result = new BiRecordChunk(size+5);
        result->data[0] = 0x17;
        *((BI_UINT32_T*) (result->data+1)) = size;
        memcpy(result->data+5, node->getValue(), size);
        this->chunks.push_back(result);
    } else {
        BiRecordChunk* result = new BiRecordChunk(size+2);
        result->data[0] = 0x07;
        *((BI_UINT8_T*) (result->data+1)) = (BI_UINT8_T) size;
        memcpy(result->data+2, node->getValue(), size);
        this->chunks.push_back(result);
    }
}

void BiRecord::_encodeList(BiList* node) {
    BI_UINT32_T size = node->getSize();
    if (size > 255) {
        BiRecordChunk* result = new BiRecordChunk(5);
        result->data[0] = 0x15;
        *((BI_UINT32_T*) (result->data+1)) = size;
        this->chunks.push_back(result);
    } else {
        BiRecordChunk* result = new BiRecordChunk(2);
        result->data[0] = 0x05;
        *((BI_UINT8_T*) (result->data+1)) = size;
        this->chunks.push_back(result);
    }
    for(unsigned int i = 0; i < size; i++)
        this->_encodeNode(&(*node)[i]);
}

void BiRecord::_encodeNamedList(BiNamedList* node) {
    BI_UINT32_T size = node->getSize();
    if (size > 255) {
        BiRecordChunk* result = new BiRecordChunk(5);
        result->data[0] = 0x16;
        *((BI_UINT32_T*) (result->data+1)) = size;
        this->chunks.push_back(result);
    } else {
        BiRecordChunk* result = new BiRecordChunk(2);
        result->data[0] = 0x06;
        *((BI_UINT8_T*) (result->data+1)) = size;
        this->chunks.push_back(result);
    }

    size_t name_size;
    const BI_CHAR_T* name;
    for(unsigned int i = 0; i < size; i++) {
        name = node->getName(i);
        name_size = strlen(name)+1;
        BiRecordChunk* key = new BiRecordChunk(name_size);
        memcpy(key->data, name, name_size);
        this->chunks.push_back(key);
        this->_encodeNode(&(*node)[name]);
    }
}

void BiRecord::setRoot(BiNode* newRoot) {
    if (this->root)
        delete this->root;

    if (this->message)
        delete this->message;
    this->message = 0;

    size_t size = this->chunks.size();
    for (unsigned int i = 0; i < size; i++) {
        delete this->chunks[i];
    }
    this->chunks.clear();

    this->root = newRoot;
}

BiNode* BiRecord::getRoot() {
    return this->root;
}

const unsigned char* BiRecord::getMessage() {
    return this->message;
}

unsigned int BiRecord::getMessageSize() {
    return this->message_size;
}

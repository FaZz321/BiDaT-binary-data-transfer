#include "BiNodes.h"

////////////
// BiNode //
////////////

BiNode::BiNode(BI_TYPE type) {
    this->type = type;
}

BiNode::~BiNode() {}

BI_TYPE BiNode::getType() {
    return this->type;
}

BiNode& BiNode::operator[](const int index) {
    throw 1;  // ERROR: Out of bounds.
}

BiNode& BiNode::operator[](const char* name) {
    throw 1;  // ERROR: Out of bounds.
}

//////////////
// BiScalar //
//////////////

BiScalar::BiScalar(BI_TYPE type): BiNode(type) {
    this->_allocated = false;
}

BiScalar::~BiScalar() {}

void BiScalar::_setAsAllocated() {
    this->_allocated = true;
}

bool BiScalar::_isAllocated() {
    return this->_allocated;
}

///////////////
// BiInteger //
///////////////

BiInteger::BiInteger(): BiScalar(BI_INTEGER) {
    this->value = 0;
}

BiInteger::BiInteger(int initValue): BiScalar(BI_INTEGER) {
    this->_setAsAllocated();
    this->value = new int(initValue);
}

BiInteger::~BiInteger() {
    if (this->_isAllocated())
        delete this->value;
}

void BiInteger::_setReference(void* value) {
    this->value = (int*) value;
}

int BiInteger::getValue() {
    if (!this->value)
        throw 1;  // ERROR: value is not defined;
    return *this->value;
}

void BiInteger::setValue(int newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    this->value = new int(newValue);
}

////////////
// BiReal //
////////////

BiReal::BiReal(): BiScalar(BI_REAL) {
    this->value = 0;
}

BiReal::BiReal(double initValue): BiScalar(BI_REAL) {
    this->_setAsAllocated();
    this->value = new double(initValue);
}

BiReal::~BiReal() {
    if (this->_isAllocated())
        delete this->value;
}

void BiReal::_setReference(void* value) {
    this->value = (double*) value;
}

double BiReal::getValue() {
    if (!this->value)
        throw 1;  // ERROR: value is not defined;
    return *this->value;
}

void BiReal::setValue(double newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    this->value = new double(newValue);
}

////////////
// BiBool //
////////////

BiBool::BiBool(): BiScalar(BI_BOOL) {
    this->value = 0;
}

BiBool::BiBool(bool initValue): BiScalar(BI_BOOL) {
    this->_setAsAllocated();
    this->value = new bool(initValue);
}

BiBool::~BiBool() {
    if (this->_isAllocated())
        delete this->value;
}

void BiBool::_setReference(void* value) {
    this->value = (bool*) value;
}

bool BiBool::getValue() {
    if (!this->value)
        throw 1;  // ERROR: value is not defined;
    return *this->value;
}

void BiBool::setValue(bool newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    this->value = new bool(newValue);
}

///////////////
// BiComplex //
///////////////

BiComplex::BiComplex(BI_TYPE type): BiScalar(type) {}

BiComplex::~BiComplex() {}

//////////////
// BiString //
//////////////

BiString::BiString(): BiComplex(BI_STRING) {
    this->value = 0;
}

BiString::BiString(const char* initValue): BiComplex(BI_STRING) {
    this->_setAsAllocated();
    size_t size = strlen(initValue) + 1;
    this->value = new char[size];
    memcpy(this->value, initValue, size);
}

BiString::~BiString() {
    if (this->_isAllocated())
        delete this->value;
}

void BiString::_setReference(void* value) {
    this->value = (char*) value;
}

unsigned int BiString::getSize() {
    return strlen(this->value);
}

const char* BiString::getValue() {
    return this->value;
}

void BiString::setValue(const char* newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    unsigned int size = strlen(newValue) + 1;
    this->value = new char[size];
    memcpy(this->value, newValue, size);
}

//////////////
// BiBinary //
//////////////

BiBinary::BiBinary(): BiComplex(BI_BINARY) {
    this->size = 0;
    this->value = 0;
}

BiBinary::BiBinary(const char* initValue, unsigned int size): BiComplex(BI_BINARY) {
    this->_setAsAllocated();
    this->value = new char[size];
    this->size = size;
    memcpy(this->value, initValue, size);
}


BiBinary::~BiBinary() {
    if (this->_isAllocated())
        delete this->value;
}

void BiBinary::_setReference(void* value) {
    this->value = (char*) value;
}

unsigned int BiBinary::getSize() {
    return this->size;
}

const char* BiBinary::getValue() {
    return this->value;
}

void BiBinary::setValue(const char* newValue, unsigned int size) {
    if (this->_isAllocated()) {
        bool reallocate = (this->size != size);

        if (reallocate) {
            this->size = size;
            delete this->value;
            this->value = new char[size];
        }
    } else {
        this->size = size;
        this->_setAsAllocated();
        this->value = new char[size];
    }

    memcpy(this->value, newValue, size);
}

/////////////////
// BiAbstrList //
/////////////////

BiAbstrList::BiAbstrList(BI_TYPE type): BiNode(type) {}

BiAbstrList::~BiAbstrList() {}

void BiAbstrList::_setReference(void* value) {
    throw 1;  // ERROR: internal error. List cannot be a reference.
}

////////////
// BiList //
////////////

BiList::BiList(): BiAbstrList(BI_LIST) {}

BiList::~BiList() {
    size_t size = this->values.size();
    for (unsigned int i = 0; i < size; i++)
        delete this->values[i];
}

// BiList::pushBack
void BiList::pushBack(int integer_value) {
    BiInteger* new_node = new BiInteger(integer_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(double real_value) {
    BiReal* new_node = new BiReal(real_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(bool bool_value) {
    BiBool* new_node = new BiBool(bool_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(const char* string_value) {
    BiString* new_node = new BiString(string_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(const char* binary_value, unsigned int size) {
    BiBinary* new_node = new BiBinary(binary_value, size);
    this->values.push_back(new_node);
}
void BiList::pushBackNewNode(BiNode* new_node) {
    this->values.push_back(new_node);
}

// BiList::pushFront
void BiList::pushFront(int integerValue) {
    BiInteger* new_node = new BiInteger(integerValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(double realValue) {
    BiReal* new_node = new BiReal(realValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(bool boolValue) {
    BiBool* new_node = new BiBool(boolValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(const char* stringValue) {
    BiString* new_node = new BiString(stringValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(const char* binaryValue, unsigned int size) {
    BiBinary* new_node = new BiBinary(binaryValue, size);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFrontNewNode(BiNode* new_node) {
    this->values.insert(this->values.begin(), new_node);
}

// BiList::insert
void BiList::insert(int integerValue, unsigned int index) {
    BiInteger* new_node = new BiInteger(integerValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(double realValue, unsigned int index) {
    BiReal* new_node = new BiReal(realValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(bool boolValue, unsigned int index) {
    BiBool* new_node = new BiBool(boolValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(const char* stringValue, unsigned int index) {
    BiString* new_node = new BiString(stringValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(const char* binaryValue, unsigned int size, unsigned int index) {
    BiBinary* new_node = new BiBinary(binaryValue, size);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insertNewNode(BiNode* new_node, unsigned int index) {
    this->values.insert(this->values.begin()+index, new_node);
}

void BiList::destroy(unsigned int index) {
    this->values.erase(this->values.begin()+index);
}

BiNode& BiList::operator[] (const int index) {
    return *this->values.at(index);
}

unsigned int BiList::getSize() {
    return this->values.size();
}

/////////////////////
// BiNameNodeTuple //
/////////////////////

BiNameNodeTuple::BiNameNodeTuple(const char* name, BiNode* node) {
    if (!node || !name)
        throw 1;  // ERROR: internal error. node is not set
    this->name = name;
    this->node = node;
}

BiNameNodeTuple::~BiNameNodeTuple() {
    delete this->name;
    delete this->node;
}

BiNode* BiNameNodeTuple::getNode() {
    return this->node;
}

const char* BiNameNodeTuple::getName() {
    return this->name;
}

////////////////
// BiHashList //
////////////////

BiHashList::~BiHashList() {
    size_t size = this->items.size();
    for (unsigned int i = 0; i < size; i++)
        delete this->items[i];
}

BiNameNodeTuple* BiHashList::find(const char* name) {
    unsigned int size = this->items.size();

    BiNameNodeTuple* tuple;
    for (unsigned int i = 0; i < size; i++) {
        tuple = this->items.at(i);
        if ( !strcmp(tuple->getName(), name) )
            return tuple;
    }

    return 0;
}

void BiHashList::destroy(const char* name) {
    unsigned int size = this->items.size();

    BiNameNodeTuple* tuple;
    for (unsigned int i = 0; i < size; i++) {
        tuple = this->items.at(i);
        if ( !strcmp(tuple->getName(), name) ) {
            delete tuple;
            this->items.erase(this->items.begin()+i);
            break;
        }
    }

    throw 1;  // ERROR: out of bounds error. No value with such a name
}

void BiHashList::push(BiNameNodeTuple* newValue) {
    unsigned int size = this->items.size();

    BiNameNodeTuple* tuple;
    for (unsigned int i = 0; i < size; i++) {
        tuple = this->items.at(i);
        if ( !strcmp(tuple->getName(), newValue->getName()) ) {
            delete tuple;
            this->items[i] = newValue;
            break;
        }
    }

    this->items.push_back(newValue);
}

unsigned int BiHashList::getSize() {
    return this->items.size();
}

BiNameNodeTuple* BiHashList::get(unsigned int index) {
    return this->items[index];
}

/////////////////
// BiHashTable //
/////////////////

unsigned int BiHashTable::_calcNameHash(const char* name) {
    unsigned int name_size = strlen(name);

    unsigned int result = 0;
    for (unsigned int i = 0; i < name_size; i++)
        result += name[i];
    result = abs(result);
    result %= this->table_size;

    return result;
}

BiNode* BiHashTable::find(const char* name) {
    unsigned int hash = this->_calcNameHash(name);

    BiNameNodeTuple* tuple = this->table[hash].find(name);
    if (!tuple)
        throw 1;  // ERROR: out of bounds error. There is no value with such name

    return tuple->getNode();
}

void BiHashTable::push(const char* name, BiNode* new_node) {
    unsigned int hash = this->_calcNameHash(name);
    this->table[hash].push(new BiNameNodeTuple(name, new_node));
}

void BiHashTable::destroy(const char* name) {
    unsigned int hash = this->_calcNameHash(name);

    this->table[hash].destroy(name);
}

unsigned int BiHashTable::getSize() {
    unsigned int size = 0;

    for (unsigned int i = 0; i  < this->table_size; i++) {
        size += this->table[i].getSize();
    }

    return size;
}

const char* BiHashTable::getName(unsigned int index) {
    unsigned int size = 0;
    BiNameNodeTuple* tuple = 0;

    for (unsigned int i = 0; i < this->table_size; i++) {
        size += this->table[i].getSize();
        if (index >= size)
            continue;
        tuple = this->table[i].get(index - (size-1));

        return tuple->getName();
    }

    throw 1;  // ERROR: out of bounds. index is refering to non existent key
}

/////////////////
// BiNamedList //
/////////////////

BiNamedList::BiNamedList(): BiAbstrList(BI_NAMED_LIST) {}

BiNamedList::~BiNamedList() {}

// BiNamedList::push
void BiNamedList::push(const char* name, int integer_value) {
    BiInteger* new_node = new BiInteger(integer_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const char* name, double real_value) {
    BiReal* new_node = new BiReal(real_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const char* name, bool bool_value) {
    BiBool* new_node = new BiBool(bool_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const char* name, const char* string_value) {
    BiString* new_node = new BiString(string_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const char* name, const char* binary_value, unsigned int size) {
    BiBinary* new_node = new BiBinary(binary_value, size);
    this->hash_table.push(name, new_node);
}

void BiNamedList::pushNewNode(const char* name, BiNode* new_node) {
    this->hash_table.push(name, new_node);
}

void BiNamedList::destroy(const char* name) {
    this->hash_table.destroy(name);
}

BiNode& BiNamedList::operator[] (const char* name) {
    return *this->hash_table.find(name);
}

unsigned int BiNamedList::getSize() {
    return this->hash_table.getSize();
}

const char* BiNamedList::getName(unsigned int index) {
    return this->hash_table.getName(index);
}

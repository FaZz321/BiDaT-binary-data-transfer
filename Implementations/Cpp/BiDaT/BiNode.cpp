#include "BiNode.h"



////////////
// BiNode //
////////////

BiNode::BiNode(BI_NODE_TYPE type) {
    this->type = type;
}

BiNode::~BiNode() {}

BI_NODE_TYPE BiNode::getType() {
    return this->type;
}

BiNode& BiNode::operator[](const BI_UINT32_T index) {
    throw BI_ERROR_OUT_OF_RANGE;
}

BiNode& BiNode::operator[](const BI_CHAR_T* name) {
    throw BI_ERROR_OUT_OF_RANGE;
}



//////////////
// BiScalar //
//////////////

BiScalar::BiScalar(BI_NODE_TYPE type): BiNode(type) {
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

BiInteger::BiInteger(): BiScalar(BI_NODE_TYPE_INT) {
    this->value = 0;
}

BiInteger::BiInteger(BI_INT32_T initValue): BiScalar(BI_NODE_TYPE_INT) {
    this->_setAsAllocated();
    this->value = new BI_INT32_T(initValue);
}

BiInteger::~BiInteger() {
    if (this->_isAllocated())
        delete this->value;
}

void BiInteger::_setReference(void* value) {
    this->value = (BI_INT32_T*) value;
}

BI_INT32_T BiInteger::getValue() {
    if (!this->value)
        throw BI_ERROR_NOT_INITIALIZED;
    return *this->value;
}

void BiInteger::setValue(BI_INT32_T newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    this->value = new BI_INT32_T(newValue);
}



////////////
// BiReal //
////////////

BiReal::BiReal(): BiScalar(BI_NODE_TYPE_REAL) {
    this->value = 0;
}

BiReal::BiReal(BI_FLOAT64_T initValue): BiScalar(BI_NODE_TYPE_REAL) {
    this->_setAsAllocated();
    this->value = new BI_FLOAT64_T(initValue);
}

BiReal::~BiReal() {
    if (this->_isAllocated())
        delete this->value;
}

void BiReal::_setReference(void* value) {
    this->value = (BI_FLOAT64_T*) value;
}

BI_FLOAT64_T BiReal::getValue() {
    if (!this->value)
        throw BI_ERROR_NOT_INITIALIZED;
    return *this->value;
}

void BiReal::setValue(BI_FLOAT64_T newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    this->value = new BI_FLOAT64_T(newValue);
}



////////////
// BiBool //
////////////

BiBool::BiBool(): BiScalar(BI_NODE_TYPE_BOOL) {
    this->value = 0;
}

BiBool::BiBool(BI_BOOL_T initValue): BiScalar(BI_NODE_TYPE_BOOL) {
    this->_setAsAllocated();
    this->value = new BI_BOOL_T(initValue);
}

BiBool::~BiBool() {
    if (this->_isAllocated())
        delete this->value;
}

void BiBool::_setReference(void* value) {
    this->value = (BI_BOOL_T*) value;
}

BI_BOOL_T BiBool::getValue() {
    if (!this->value)
        throw BI_ERROR_NOT_INITIALIZED;
    return *this->value;
}

void BiBool::setValue(BI_BOOL_T newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    this->value = new BI_BOOL_T(newValue);
}



///////////////
// BiComplex //
///////////////

BiComplex::BiComplex(BI_NODE_TYPE type): BiScalar(type) {}

BiComplex::~BiComplex() {}



//////////////
// BiString //
//////////////

BiString::BiString(): BiComplex(BI_NODE_TYPE_STR) {
    this->value = 0;
}

BiString::BiString(const BI_CHAR_T* initValue): BiComplex(BI_NODE_TYPE_STR) {
    this->_setAsAllocated();
    unsigned int size = strlen(initValue) + 1;
    this->value = new BI_CHAR_T[size];
    memcpy(this->value, initValue, size);
}

BiString::~BiString() {
    if (this->_isAllocated())
        delete this->value;
}

void BiString::_setReference(void* value) {
    this->value = (BI_CHAR_T*) value;
}

unsigned int BiString::getSize() {
    return strlen(this->value);
}

const BI_CHAR_T* BiString::getValue() {
    return this->value;
}

void BiString::setValue(const BI_CHAR_T* newValue) {
    if (this->_isAllocated())
        delete this->value;
    else
        this->_setAsAllocated();

    unsigned int size = strlen(newValue) + 1;
    this->value = new BI_CHAR_T[size];
    memcpy(this->value, newValue, size);
}



//////////////
// BiBinary //
//////////////

BiBinary::BiBinary(): BiComplex(BI_NODE_TYPE_BINARY) {
    this->size = 0;
    this->value = 0;
}

BiBinary::BiBinary(const BI_BYTE_T* initValue, BI_UINT32_T size): BiComplex(BI_NODE_TYPE_BINARY) {
    this->_setAsAllocated();
    this->value = new BI_BYTE_T[size];
    this->size = size;
    memcpy(this->value, initValue, size);
}


BiBinary::~BiBinary() {
    if (this->_isAllocated())
        delete this->value;
}

void BiBinary::_setReference(void* value) {
    this->value = (BI_BYTE_T*) value;
}

void BiBinary::_setSize(BI_UINT32_T newSize) {
    this->size = newSize;
}

BI_UINT32_T BiBinary::getSize() {
    return this->size;
}

const BI_BYTE_T* BiBinary::getValue() {
    return this->value;
}

void BiBinary::setValue(const BI_BYTE_T* newValue, BI_UINT32_T size) {
    if (this->_isAllocated()) {
        bool reallocate = (this->size != size);

        if (reallocate) {
            this->size = size;
            delete this->value;
            this->value = new BI_BYTE_T[size];
        }
    } else {
        this->size = size;
        this->_setAsAllocated();
        this->value = new BI_BYTE_T[size];
    }

    memcpy(this->value, newValue, size);
}



/////////////////
// BiAbstrList //
/////////////////

BiAbstrList::BiAbstrList(BI_NODE_TYPE type): BiNode(type) {}

BiAbstrList::~BiAbstrList() {}

void BiAbstrList::_setReference(void* value) {
    throw BI_ERROR_INTERNAL;  // List cannot be a reference
}



////////////
// BiList //
////////////

BiList::BiList(): BiAbstrList(BI_NODE_TYPE_LIST) {}

BiList::~BiList() {
    size_t size = this->values.size();
    for (BI_UINT32_T i = 0; i < size; i++)
        delete this->values[i];
}

// BiList::pushBack
void BiList::pushBack(BI_INT32_T integer_value) {
    BiInteger* new_node = new BiInteger(integer_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(BI_FLOAT64_T real_value) {
    BiReal* new_node = new BiReal(real_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(BI_BOOL_T bool_value) {
    BiBool* new_node = new BiBool(bool_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(const BI_CHAR_T* string_value) {
    BiString* new_node = new BiString(string_value);
    this->values.push_back(new_node);
}
void BiList::pushBack(const BI_BYTE_T* binary_value, BI_UINT32_T size) {
    BiBinary* new_node = new BiBinary(binary_value, size);
    this->values.push_back(new_node);
}
void BiList::pushBackNewNode(BiNode* new_node) {
    this->values.push_back(new_node);
}

// BiList::pushFront
void BiList::pushFront(BI_INT32_T integerValue) {
    BiInteger* new_node = new BiInteger(integerValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(BI_FLOAT64_T realValue) {
    BiReal* new_node = new BiReal(realValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(BI_BOOL_T boolValue) {
    BiBool* new_node = new BiBool(boolValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(const BI_CHAR_T* stringValue) {
    BiString* new_node = new BiString(stringValue);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFront(const BI_BYTE_T* binaryValue, BI_UINT32_T size) {
    BiBinary* new_node = new BiBinary(binaryValue, size);
    this->values.insert(this->values.begin(), new_node);
}
void BiList::pushFrontNewNode(BiNode* new_node) {
    this->values.insert(this->values.begin(), new_node);
}

// BiList::insert
void BiList::insert(BI_INT32_T integerValue, BI_UINT32_T index) {
    BiInteger* new_node = new BiInteger(integerValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(BI_FLOAT64_T realValue, BI_UINT32_T index) {
    BiReal* new_node = new BiReal(realValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(BI_BOOL_T boolValue, BI_UINT32_T index) {
    BiBool* new_node = new BiBool(boolValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(const BI_CHAR_T* stringValue, BI_UINT32_T index) {
    BiString* new_node = new BiString(stringValue);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insert(const BI_BYTE_T* binaryValue, BI_UINT32_T size, BI_UINT32_T index) {
    BiBinary* new_node = new BiBinary(binaryValue, size);
    this->values.insert(this->values.begin()+index, new_node);
}
void BiList::insertNewNode(BiNode* new_node, BI_UINT32_T index) {
    this->values.insert(this->values.begin()+index, new_node);
}

void BiList::destroy(const BI_UINT32_T index) {
    this->values.erase(this->values.begin()+index);
}

BiNode& BiList::operator[] (const BI_UINT32_T index) {
    size_t size = this->values.size();

    if (index >= size)
        throw BI_ERROR_OUT_OF_RANGE;

    return *this->values[index];
}

BI_UINT32_T BiList::getSize() {
    return this->values.size();
}



/////////////////////
// BiNameNodeTuple //
/////////////////////

BiNameNodeTuple::BiNameNodeTuple(const BI_CHAR_T* name, BiNode* node) {
    if (!node || !name)
        throw BI_ERROR_INTERNAL;  // Node is not set
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

const BI_CHAR_T* BiNameNodeTuple::getName() {
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

BiNameNodeTuple* BiHashList::find(const BI_CHAR_T* name) {
    unsigned int size = this->items.size();

    BiNameNodeTuple* tuple;
    for (unsigned int i = 0; i < size; i++) {
        tuple = this->items.at(i);
        if ( !strcmp(tuple->getName(), name) )
            return tuple;
    }

    return 0;
}

void BiHashList::destroy(const BI_CHAR_T* name) {
    size_t size = this->items.size();

    BiNameNodeTuple* tuple;
    for (unsigned int i = 0; i < size; i++) {
        tuple = this->items.at(i);
        if ( !strcmp(tuple->getName(), name) ) {
            delete tuple;
            this->items.erase(this->items.begin()+i);
            break;
        }
    }

    throw BI_ERROR_OUT_OF_RANGE;  // No value with such a name
}

void BiHashList::push(BiNameNodeTuple* newValue) {
    size_t size = this->items.size();

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

unsigned int BiHashTable::_calcNameHash(const BI_CHAR_T* name) {
    size_t name_size = strlen(name);
    unsigned int result = 0;
    for (unsigned int i = 0; i < name_size; i++)
        result += name[i];
    result = abs(result);
    result %= BI_HASH_TABLE_SIZE;

    return result;
}

BiNode* BiHashTable::find(const BI_CHAR_T* name) {
    unsigned int hash = this->_calcNameHash(name);

    BiNameNodeTuple* tuple = this->table[hash].find(name);
    if (!tuple)
        throw BI_ERROR_OUT_OF_RANGE;  // There is no value with such name

    return tuple->getNode();
}

void BiHashTable::push(const BI_CHAR_T* name, BiNode* new_node) {
    unsigned int hash = this->_calcNameHash(name);

    this->table[hash].push(
        new BiNameNodeTuple(name, new_node)
    );
}

void BiHashTable::destroy(const BI_CHAR_T* name) {
    unsigned int hash = this->_calcNameHash(name);

    this->table[hash].destroy(name);
}

BI_UINT32_T BiHashTable::getSize() {
    BI_UINT32_T size = 0;

    for (unsigned int i = 0; i  < BI_HASH_TABLE_SIZE; i++) {
        size += this->table[i].getSize();
    }

    return size;
}

const BI_CHAR_T* BiHashTable::getName(BI_UINT32_T index) {
    unsigned int size = 0;
    BiNameNodeTuple* tuple = 0;

    for (unsigned int i = 0; i < BI_HASH_TABLE_SIZE; i++) {
        size += this->table[i].getSize();
        if (index >= size)
            continue;
        tuple = this->table[i].get(index - (size-1));

        return tuple->getName();
    }

    throw BI_ERROR_OUT_OF_RANGE;  // Index referring to non existent key
}



/////////////////
// BiNamedList //
/////////////////

BiNamedList::BiNamedList(): BiAbstrList(BI_NODE_TYPE_NAMED_LIST) {}

BiNamedList::~BiNamedList() {}

// BiNamedList::push
void BiNamedList::push(const BI_CHAR_T* name, BI_INT32_T integer_value) {
    BiInteger* new_node = new BiInteger(integer_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const BI_CHAR_T* name, BI_FLOAT64_T real_value) {
    BiReal* new_node = new BiReal(real_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const BI_CHAR_T* name, BI_BOOL_T bool_value) {
    BiBool* new_node = new BiBool(bool_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const BI_CHAR_T* name, const BI_CHAR_T* string_value) {
    BiString* new_node = new BiString(string_value);
    this->hash_table.push(name, new_node);
}
void BiNamedList::push(const BI_CHAR_T* name, const BI_BYTE_T* binary_value, BI_UINT32_T size) {
    BiBinary* new_node = new BiBinary(binary_value, size);
    this->hash_table.push(name, new_node);
}

void BiNamedList::pushNewNode(const BI_CHAR_T* name, BiNode* new_node) {
    this->hash_table.push(name, new_node);
}

void BiNamedList::destroy(const BI_CHAR_T* name) {
    this->hash_table.destroy(name);
}

BiNode& BiNamedList::operator[] (const BI_CHAR_T* name) {
    return *this->hash_table.find(name);
}

BI_UINT32_T BiNamedList::getSize() {
    return this->hash_table.getSize();
}

const BI_CHAR_T* BiNamedList::getName(BI_UINT32_T index) {
    return this->hash_table.getName(index);
}

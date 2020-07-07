#ifndef __BINODES_H__
#define __BINODES_H__

// dependencies:
#include <cstring>
#include <vector>
#include <cmath>

////////////
// CONSTS //
////////////

enum BI_NODE_TYPE {
    BI_NODE_TYPE_INT,
    BI_NODE_TYPE_REAL,
    BI_NODE_TYPE_STR,
    BI_NODE_TYPE_BOOL,
    BI_NODE_TYPE_LIST,
    BI_NODE_TYPE_NAMED_LIST,
    BI_NODE_TYPE_BINARY
};

static const unsigned int BI_HASH_TABLE_SIZE = 7;

// BiDaT types (can be platform dependent to some degree)
typedef unsigned char BI_BYTE_T;
typedef char BI_CHAR_T;
typedef int BI_INT32_T;
typedef unsigned char BI_UINT8_T;
typedef unsigned int BI_UINT32_T;
typedef double BI_FLOAT64_T;
typedef bool BI_BOOL_T;

///////////
// NODES //
///////////


class BiNode {
    private:
        BI_NODE_TYPE type;

    public:
        BiNode(BI_NODE_TYPE type);
        virtual ~BiNode();

        BI_NODE_TYPE getType();
        virtual BiNode& operator [] (const BI_UINT32_T);
        virtual BiNode& operator [] (const BI_CHAR_T*);

        virtual void _setReference(void* value) = 0;
};

//////////////////
// Scalar types //
//////////////////

class BiScalar: public BiNode {
    private:
        bool _allocated;

    protected:
        BiScalar(BI_NODE_TYPE type);
        virtual ~BiScalar();

        void _setAsAllocated();
        bool _isAllocated();
};

class BiInteger: public BiScalar {
    private:
        BI_INT32_T* value;

    public:
        BiInteger();
        BiInteger(BI_INT32_T initValue);
        ~BiInteger();

        BI_INT32_T getValue();
        void setValue(BI_INT32_T newValue);

        virtual void _setReference(void* value);
};

class BiReal: public BiScalar {
    private:
        BI_FLOAT64_T* value;

    public:
        BiReal();
        BiReal(BI_FLOAT64_T initValue);
        ~BiReal();

        BI_FLOAT64_T getValue();
        void setValue(BI_FLOAT64_T newValue);

        virtual void _setReference(void* value);
};

class BiBool: public BiScalar {
    private:
        BI_BOOL_T* value;

    public:
        BiBool();
        BiBool(BI_BOOL_T initValue);
        ~BiBool();

        BI_BOOL_T getValue();
        void setValue(BI_BOOL_T newValue);

        virtual void _setReference(void* value);
};

///////////////
// BiComplex //
///////////////

class BiComplex: public BiScalar {
    public:
        BiComplex(BI_NODE_TYPE type);
        virtual ~BiComplex();

        virtual BI_UINT32_T getSize() = 0;
};

class BiString: public BiComplex {
    private:
        BI_CHAR_T* value;

    public:
        BiString();
        BiString(const BI_CHAR_T* initValue);
        ~BiString();

        virtual BI_UINT32_T getSize();
        const BI_CHAR_T* getValue();
        void setValue(const BI_CHAR_T* newValue);

        virtual void _setReference(void* value);
};

class BiBinary: public BiComplex {
    private:
        BI_BYTE_T* value;
        BI_UINT32_T size;

    public:
        BiBinary();
        BiBinary(const BI_BYTE_T* initValue, BI_UINT32_T size);
        virtual ~BiBinary();

        BI_UINT32_T getSize();
        void setValue(const BI_BYTE_T* newValue, BI_UINT32_T size);
        const BI_BYTE_T* getValue();

        virtual void _setReference(void* value);
        void _setSize(BI_UINT32_T newSize);  // used only when setting a reference;
};

/////////////
// BiLists //
/////////////

class BiAbstrList: public BiNode {
    protected:
        virtual void _setReference(void* value);

    public:
        BiAbstrList(BI_NODE_TYPE type);
        virtual ~BiAbstrList();

        virtual BI_UINT32_T getSize() = 0;
};

class BiList: public BiAbstrList {
    protected:
        std::vector<BiNode*> values;

    public:
        BiList();
        virtual ~BiList();

        void pushBack(BI_INT32_T integerValue);
        void pushBack(BI_FLOAT64_T realValue);
        void pushBack(BI_BOOL_T boolValue);
        void pushBack(const BI_CHAR_T* stringValue);
        void pushBack(const BI_BYTE_T* binaryValue, BI_UINT32_T size);
        void pushBackNewNode(BiNode* node);

        void pushFront(BI_INT32_T integerValue);
        void pushFront(BI_FLOAT64_T realValue);
        void pushFront(BI_BOOL_T boolValue);
        void pushFront(const BI_CHAR_T* stringValue);
        void pushFront(const BI_BYTE_T* binaryValue, BI_UINT32_T size);
        void pushFrontNewNode(BiNode* node);

        void insert(BI_INT32_T integerValue, BI_UINT32_T index);
        void insert(BI_FLOAT64_T realValue, BI_UINT32_T index);
        void insert(BI_BOOL_T boolValue, BI_UINT32_T index);
        void insert(const BI_CHAR_T* stringValue, BI_UINT32_T index);
        void insert(const BI_BYTE_T* binaryValue, BI_UINT32_T size, BI_UINT32_T index);
        void insertNewNode(BiNode* node, BI_UINT32_T index);

        void destroy(BI_UINT32_T index);

        virtual BiNode& operator[] (const BI_UINT32_T index);
        virtual BI_UINT32_T getSize();
};

struct BiNameNodeTuple {
    private:
        const BI_CHAR_T* name;
        BiNode* node;

    public:
        BiNameNodeTuple(const BI_CHAR_T* name, BiNode* node);
        ~BiNameNodeTuple();

        const BI_CHAR_T* getName();
        BiNode* getNode();
};

class BiHashList {
    private:
        std::vector<BiNameNodeTuple*> items;

    public:
        ~BiHashList();

        BiNameNodeTuple* get(BI_UINT32_T index);
        BiNameNodeTuple* find(const BI_CHAR_T* name);
        void push(BiNameNodeTuple* newValue);
        void destroy(const BI_CHAR_T* name);

        BI_UINT32_T getSize();
};

class BiHashTable {
    private:
        BiHashList table[BI_HASH_TABLE_SIZE];

    protected:
        unsigned int _calcNameHash(const BI_CHAR_T* name);

    public:
        BiNode* find(const BI_CHAR_T* name);
        void push(const BI_CHAR_T* name, BiNode* new_node);
        void destroy(const BI_CHAR_T* name);

        BI_UINT32_T getSize();
        const BI_CHAR_T* getName(BI_UINT32_T index);
};

class BiNamedList: public BiAbstrList {
    protected:
        BiHashTable hash_table;

    public:
        BiNamedList();
        virtual ~BiNamedList();

        void push(const BI_CHAR_T* name, BI_INT32_T integer_value);
        void push(const BI_CHAR_T* name, BI_FLOAT64_T real_value);
        void push(const BI_CHAR_T* name, BI_BOOL_T bool_value);
        void push(const BI_CHAR_T* name, const BI_CHAR_T* string_value);
        void push(const BI_CHAR_T* name, const BI_BYTE_T* binary_value, BI_UINT32_T size);
        void pushNewNode(const BI_CHAR_T* name, BiNode* new_node);

        void destroy(const BI_CHAR_T* name);

        virtual BiNode& operator[] (const BI_CHAR_T* name);
        virtual BI_UINT32_T getSize();
        const BI_CHAR_T* getName(BI_UINT32_T index);
};

#endif

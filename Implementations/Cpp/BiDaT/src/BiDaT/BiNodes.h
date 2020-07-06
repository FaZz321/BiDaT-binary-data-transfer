#ifndef __BINODES_H__
#define __BINODES_H__

// dependencies:
#include <cstring>
#include <vector>
#include <cmath>

////////////
// CONSTS //
////////////

enum BI_TYPE {
    BI_INTEGER,
    BI_REAL,
    BI_STRING,
    BI_BOOL,
    BI_LIST,
    BI_NAMED_LIST,
    BI_BINARY
};

class BiNode {
    private:
        BI_TYPE type;

    protected:
        virtual void _setReference(void* value) = 0;

    public:
        BiNode(BI_TYPE type);
        virtual ~BiNode();

        BI_TYPE getType();
        virtual BiNode& operator [] (const int);
        virtual BiNode& operator [] (const char*);
};

//////////////////
// Scalar types //
//////////////////

class BiScalar: public BiNode {
    private:
        bool _allocated;

    protected:
        BiScalar(BI_TYPE type);
        virtual ~BiScalar();

        void _setAsAllocated();
        bool _isAllocated();
};

class BiInteger: public BiScalar {
    private:
        int* value;

    protected:
        virtual void _setReference(void* value);

    public:
        BiInteger();
        BiInteger(int initValue);
        ~BiInteger();

        int getValue();
        void setValue(int newValue);
};

class BiReal: public BiScalar {
    private:
        double* value;

    protected:
        virtual void _setReference(void* value);

    public:
        BiReal();
        BiReal(double initValue);
        ~BiReal();

        double getValue();
        void setValue(double newValue);
};

class BiBool: public BiScalar {
    private:
        bool* value;

    protected:
        virtual void _setReference(void* value);

    public:
        BiBool();
        BiBool(bool initValue);
        ~BiBool();

        bool getValue();
        void setValue(bool newValue);
};

///////////////
// BiComplex //
///////////////

class BiComplex: public BiScalar {
    public:
        BiComplex(BI_TYPE type);
        virtual ~BiComplex();

        virtual unsigned int getSize() = 0;
};

class BiString: public BiComplex {
    private:
        char* value;

    protected:
        virtual void _setReference(void* value);

    public:
        BiString();
        BiString(const char* initValue);
        ~BiString();

        virtual unsigned int getSize();
        const char* getValue();
        void setValue(const char* newValue);
};

class BiBinary: public BiComplex {
    private:
        char* value;
        unsigned int size;

    protected:
        virtual void _setReference(void* value);

    public:
        BiBinary();
        BiBinary(const char* initValue, unsigned int size);
        virtual ~BiBinary();

        unsigned int getSize();
        void setValue(const char* newValue, unsigned int size);
        const char* getValue();
};

/////////////
// BiLists //
/////////////

class BiAbstrList: public BiNode {
    protected:
        virtual void _setReference(void* value);

    public:
        BiAbstrList(BI_TYPE type);
        virtual ~BiAbstrList();

        virtual unsigned int getSize() = 0;
};

class BiList: public BiAbstrList {
    protected:
        std::vector<BiNode*> values;

    public:
        BiList();
        virtual ~BiList();

        void pushBack(int integerValue);
        void pushBack(double realValue);
        void pushBack(bool boolValue);
        void pushBack(const char* stringValue);
        void pushBack(const char* binaryValue, unsigned int size);
        void pushBackNewNode(BiNode* node);

        void pushFront(int integerValue);
        void pushFront(double realValue);
        void pushFront(bool boolValue);
        void pushFront(const char* stringValue);
        void pushFront(const char* binaryValue, unsigned int size);
        void pushFrontNewNode(BiNode* node);

        void insert(int integerValue, unsigned int index);
        void insert(double integerValue, unsigned int index);
        void insert(bool integerValue, unsigned int index);
        void insert(const char* integerValue, unsigned int index);
        void insert(const char* integerValue, unsigned int size, unsigned int index);
        void insert(BiAbstrList* new_list, unsigned int index);
        void insertNewNode(BiNode* node, unsigned int index);

        void destroy(unsigned int index);

        virtual BiNode& operator[] (const int index);
        virtual unsigned int getSize();
};

struct BiNameNodeTuple {
    private:
        const char* name;
        BiNode* node;

    public:
        BiNameNodeTuple(const char* name, BiNode* node);
        ~BiNameNodeTuple();

        const char* getName();
        BiNode* getNode();
};

class BiHashList {
    private:
        std::vector<BiNameNodeTuple*> items;

    public:
        ~BiHashList();

        BiNameNodeTuple* get(unsigned int index);
        BiNameNodeTuple* find(const char* name);
        void push(BiNameNodeTuple* newValue);
        void destroy(const char* name);

        unsigned int getSize();
};

class BiHashTable {
    private:
        const static unsigned int table_size = 7;
        BiHashList table[BiHashTable::table_size];

    protected:
        unsigned int _calcNameHash(const char* name);

    public:
        BiNode* find(const char* name);
        void push(const char* name, BiNode* new_node);
        void destroy(const char* name);

        unsigned int getSize();
        const char* getName(unsigned int index);
};

class BiNamedList: public BiAbstrList {
    protected:
        BiHashTable hash_table;

    public:
        BiNamedList();
        virtual ~BiNamedList();

        void push(const char* name, int integer_value);
        void push(const char* name, double real_value);
        void push(const char* name, bool bool_value);
        void push(const char* name, const char* string_value);
        void push(const char* name, const char* binary_value, unsigned int size);
        void pushNewNode(const char* name, BiNode* new_node);

        void destroy(const char* name);

        virtual BiNode& operator[] (const char* name);
        virtual unsigned int getSize();
        const char* getName(unsigned int index);
};

#endif

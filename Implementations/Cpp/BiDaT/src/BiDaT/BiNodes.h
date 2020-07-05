#ifndef __BINODES_H__
#define __BINODES_H__

// dependencies:
#include <cstring>
#include <vector>

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
        virtual BiNode* operator [] (const int);
        virtual BiNode* operator [] (const char*);
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


// TOFIX: implement further:

class BiList: public BiAbstrList {
    protected:
        std::vector<BiNode*> values;

    public:
        BiList();
        virtual ~BiList();

        void pushBack(BiNode* new_node);
        void pushFront(BiNode* new_node);
        void insert(BiNode* new_node, unsigned int index);
        void destroy(unsigned int index);

        virtual BiNode* operator[] (const int index);
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
        BiNameNodeTuple* get(unsigned int index);
        BiNameNodeTuple* find(const char* name);
        void push(BiNameNodeTuple* newValue);
        void destroy(const char* name);

        unsigned int getSize();
};

class BiHashTable {
    private:
        const static unsigned int table_size = 10;
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

        void push(const char* name, BiNode* new_node);
        void destroy(const char* name);

        virtual BiNode* operator[] (const char* name);
        virtual unsigned int getSize();
        const char* getName(unsigned int index);
};

#endif

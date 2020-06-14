#ifndef __BILISTS_H__
#define __BILISTS_H__

// dependencies:
#include <cstring>

#include "BiNodes.h"

/////////////////
// BiAbstrList //
/////////////////

class BiAbstrList: public BiNode {
    protected:
        unsigned int size;
        BiNode** values;

    public:
        BiAbstrList(BI_TYPE type, unsigned int size = 0);
        ~BiAbstrList();
};

////////////
// BiList //
////////////

class BiList: public BiAbstrList {
    public:
        BiList();

        void insertNode(unsigned int index, BiNode* new_node);
        void pushBack(BiNode* new_node);
        void pushFront(BiNode* new_node);

        virtual BiNode* byIndex(unsigned int index);
};

/////////////////
// BiNamedList //
/////////////////

// TOFIX: implement

#endif

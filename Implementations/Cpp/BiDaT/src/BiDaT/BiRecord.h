#ifndef __BIRECORDS_H__
#define __BIRECORDS_H__

// dependencies:
#include "BiNode.h"

struct BiRecordChunk {
    unsigned int size;
    char* data;
};

class BiRecord {
    private:
        BiNode* root;
        std::vector<BiRecordChunk> chunks;

    public:
        BiRecord();
        ~BiRecord();

        void setRoot(BiNode* node);
        BiNode* getRoot();
        char* encodeMessage();
};

#endif

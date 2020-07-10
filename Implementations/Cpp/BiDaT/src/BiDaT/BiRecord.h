#ifndef __BIRECORDS_H__
#define __BIRECORDS_H__

// dependencies:
#include "BiNode.h"
#include <cstring>

struct BiRecordChunk {
    public:
        BiRecordChunk(unsigned int size);
        ~BiRecordChunk();

        unsigned int size;
        unsigned char* data;
};

class BiRecord {
    private:
        BiNode* root;
        std::vector<BiRecordChunk*> chunks;

        // last encoded message:
        unsigned char* message;
        unsigned int message_size;

        // methods:
        void _encodeNode(BiNode* node);
        void _encodeInt(BiInteger* node);
        void _encodeReal(BiReal* node);
        void _encodeBool(BiBool* node);
        void _encodeStr(BiString* node);
        void _encodeList(BiList* node);
        void _encodeNamedList(BiNamedList* node);
        void _encodeBinary(BiBinary* node);

    public:
        BiRecord();
        BiRecord(BiNode* root);
        ~BiRecord();

        void setRoot(BiNode* node);
        BiNode* getRoot();
        const unsigned char* getMessage();
        unsigned int getMessageSize();

        void encodeMessage();
};

#endif

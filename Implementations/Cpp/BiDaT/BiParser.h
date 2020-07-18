#ifndef __BIPARSER_H__
#define __BIPARSER_H__

#include "BiNode.h"
#include "BiRecord.h"

class BiParser {
    private:
        // data:
        unsigned int _cursor;
        BI_BYTE_T* _binary_data;

        // methods:
        BiNode* _parseValue();
        BiNode* _parseInt();
        BiNode* _parseReal();
        BiNode* _parseBool();
        BiNode* _parseStr();
        BiNode* _parseList(bool large);
        BiNode* _parseNamedList(bool large);
        BiNode* _parseBinary(bool large);

    public:
        BiParser();
        BiParser(BI_BYTE_T* binary_data);

        void parse(BiRecord &dest);

        void setBinaryData(BI_BYTE_T* binary_data);
        const BI_BYTE_T* getBinaryData();
};

#endif

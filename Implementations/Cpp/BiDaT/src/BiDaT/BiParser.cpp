#include "BiParser.h"

//////////////
// BiParser //
//////////////

BiParser::BiParser(BI_BYTE_T* binary_data) {
    this->_cursor = 0;
    this->_binary_data = binary_data;
}

void BiParser::parseMessage(BiRecord &dest) {
    if (this->_binary_data[this->_cursor] != 0x00)
        throw BI_ERROR_CANT_PARSE;  // Not a BiDaT message
    this->_cursor++;

    BiNode* result = this->_parseValue();

    if (this->_binary_data[this->_cursor] != 0xFF) {
        delete result;
        throw BI_ERROR_CANT_PARSE;  // Not a BiDaT message
    }

    dest.setRoot(result);
}

BiNode* BiParser::_parseValue() {
    char type = this->_binary_data[this->_cursor];
    this->_cursor++;

    switch (type) {
        case 0x01:
            return this->_parseInt();
        case 0x02:
            return this->_parseReal();
        case 0x03:
            return this->_parseBool();
        case 0x04:
            return this->_parseStr();
        case 0x05:
            return this->_parseList(false);
        case 0x15:
            return this->_parseList(true);
        case 0x06:
            return this->_parseNamedList(false);
        case 0x16:
            return this->_parseNamedList(true);
        case 0x07:
            return this->_parseBinary(false);
        case 0x17:
            return this->_parseBinary(true);
        default:
            throw BI_ERROR_CANT_PARSE;  // Wrong value type
    }
}

BiNode* BiParser::_parseInt() {
    // 4 bytes integer
    BI_INT32_T* value = (BI_INT32_T*) (this->_binary_data + this->_cursor);
    BiInteger* result = new BiInteger();
    result->_setReference(value);
    this->_cursor += 4;
    return result;
}

BiNode* BiParser::_parseReal() {
    // 8 bytes real
    BI_FLOAT64_T* value = (BI_FLOAT64_T*) (this->_binary_data + this->_cursor);
    BiReal* result = new BiReal();
    result->_setReference(value);
    this->_cursor += 8;
    return result;
}

BiNode* BiParser::_parseBool() {
    // 1 byte bool
    BI_BOOL_T* value = (BI_BOOL_T*) (this->_binary_data + this->_cursor);
    if (!(*value == 0x00 || *value == 0x01))
        throw BI_ERROR_CANT_PARSE;  // Abnormal boolean value
    BiBool* result = new BiBool();
    result->_setReference(value);
    this->_cursor += 1;
    return result;
}

BiNode* BiParser::_parseStr() {
    // null terminated string
    BI_CHAR_T* value = (BI_CHAR_T*) (this->_binary_data + this->_cursor);
    size_t size = strlen(value)+1;
    BiString* result = new BiString();
    result->_setReference(value);
    this->_cursor += size;
    return result;
}

BiNode* BiParser::_parseList(bool large) {
    size_t size;
    if (large) {
        size = *((BI_UINT32_T*) (this->_binary_data + this->_cursor));
        this->_cursor += 4;
    } else {
        size = *((BI_BYTE_T*) (this->_binary_data + this->_cursor));
        this->_cursor++;
    }

    BiList* result = new BiList();
    for (unsigned int i = 0; i < size; i++) {
        result->pushBackNewNode(this->_parseValue());
    }

    return result;
}

BiNode* BiParser::_parseNamedList(bool large) {
    size_t size;
    if (large) {
        size = *((BI_UINT32_T*) (this->_binary_data + this->_cursor));
        this->_cursor += 4;
    } else {
        size = *((BI_BYTE_T*) (this->_binary_data + this->_cursor));
        this->_cursor++;
    }

    BiNamedList* result = new BiNamedList();
    size_t name_size;
    for (unsigned int i = 0; i < size; i++) {
        BI_CHAR_T* name = (BI_CHAR_T*) (this->_binary_data + this->_cursor);
        name_size = strlen(name);
        this->_cursor += name_size;

        result->pushNewNode(name, this->_parseValue());
    }

    return result;
}

BiNode* BiParser::_parseBinary(bool large) {
    size_t size;
    if (large) {
        size = *((BI_UINT32_T*) (this->_binary_data + this->_cursor));
        this->_cursor += 4;
    } else {
        size = *((BI_BYTE_T*) (this->_binary_data + this->_cursor));
        this->_cursor++;
    }

    BiBinary* result = new BiBinary();
    result->_setReference(this->_binary_data + this->_cursor);
    result->_setSize(size);
    this->_cursor += size;

    return result;
}

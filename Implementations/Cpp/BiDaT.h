#ifndef __BIDAT_H__
#define __BIDAT_H__

// BASIC CONCEPTS:
// 1. BiDaT message used in "read only" way, so there will be no changes in original message. Readed BiDaT message will not be deleted.
// 2. If some value in the message is changed, memory will be reallocated only for the changed value. Other values will still be readed from binary data of an original message.
// 3. When work is done BiDaT message should be allocated in a new memory location.

// COMPILATION PARAMETERS:
//   BI_HASH_TABLE_SIZE — decreases searching time in named lists, but increases memory usage and a adds a bit of time for named list creation

// RAW DATA TYPES:
//   BI_BYTE_T — unsigned byte;
//   BI_CHAR_T — signed byte
//   BI_INT32_T — signed 4 byte integer
//   BI_UINT8_T — unsigned 1 byte integer
//   BI_UINT32_T — unsigned 4 byte integer
//   BI_FLOAT64_T — float64
//   BI_BOOL_T — 0x00 of 'false', 0x01 if 'true'

// OTHER:
// enum for reverse casting:  BI_NODE_TYPE
// exceptions enum:  BI_ERROR

// USAGE:
// encoding:
// 1. Use BiNode child classes to create a data structure.
//   Note 1: Use constructors with initial values to avoid unnecessary errors.
//   Note 2: Complex scalar types (such as BiBinary and BiString) are copying initial values. Be sure to empty used memory if needed.
//   Note 3: list classes provide a lot of overloads, so it's not necessary to create new BiNode child for every value by hand.
//   Note 4: adding nodes using BiList::pushBackNewNode(BiNode* node), BiNamedList::pushNewNode(const BI_CHAR_T* name, BiNode* new_node) etc.
//   requires to create a BiNode using "new". It'll be deleted automatically inside destructor of parent node. Use this to create nested lists.
// 2. Create an instance of BiRecord using created root node as an argument.
// 3. Encode a new BiDaT message using BiRecord::encode();
//    Note 1: Use BiRecord::getMessage() and BiRecord::getMessageSize() to get access to binary data of BiDaT message.
//    Note 2: New message is stored in BiRecord instance. When BiRecord is deleted or root value is changed, stored message will be also deleted.
// decoding:
// 1. Create BiParser instance using binary data of BiDaT message as an argument.
// 2. Create BiRecord instance using default constructor.
// 3. Use BiParser::parse(BiRecord &dest) to parse data into created record.
//    Note 1: if BiDaT message is corrupted BI_ERROR_CANT_PARSE exception will be raised
//    Note 2: Use BiRecord::getRoot() to get root node of parsed data.
//    Note 3: You can use sequences like record[1]["name1"][2]["name2"] to navigate inside the data. If index or name of a value is not found BI_ERROR_OUT_OF_RANGE exception will be raised.
//    Note 4: use types defined in BI_NODE_TYPE to restore node types
//      Example:
//        BiNode* root = record.getRoot();
//        if (node->getType() == BI_NODE_TYPE_INT) {
//            BiInteger* int_node = (BiInteger*) node;
//            // do stuff...
//        }

// See DemoFunctions folder for more examples

#include "BiDaT/BiParser.h"

#endif

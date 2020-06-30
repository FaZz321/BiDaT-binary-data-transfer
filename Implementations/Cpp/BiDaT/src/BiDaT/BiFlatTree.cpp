#include "BiFlatTree.h"

////////////////////
// BiFlatTreeList //
////////////////////

BiFlatTreeList::BiFlatTreeList(
    unsigned char* binary_position,
    unsigned int size,
    unsigned int item_count,
    unsigned int list_count,
    unsigned int parent_index
) {
    this->binary_position = binary_position;
    this->size = size;
    this->item_count = item_count;
    this->list_count = list_count;
    this->parent_index = parent_index;
}

////////////////////
// BiFlatTreeItem //
////////////////////

BiFlatTreeItem::BiFlatTreeItem(
    unsigned char* binary_position,
    unsigned int size
) {
    this->binary_position = binary_position;
    this->size = size;
}

////////////////
// BiFlatTree //
////////////////

BiFlatTree::BiFlatTree(unsigned char* binary_data) {
    this->binary_data = binary_data;
    this->_buildTree();
}

void BiFlatTree::_buildTree() {
    unsigned int cursor = 0;

    // first bracket
    if (this->binary_data[cursor] != 0x00)
        throw 1;  // ERROR: not a BiDaT message
    cursor += 1;

    this->_buildNode(cursor);

    // last bracket:
    if (this->binary_data[cursor] != 0xff)
        throw 1;  // ERROR: not a BiDaT message
    cursor += 1;

    this->message_size = cursor;
}

void BiFlatTree::_buildNode(unsigned int& cursor, unsigned int parent_index) {
    char type = this->binary_data[cursor];
    unsigned int additional_size;

    switch(type) {
        case 0x01:  // int
            additional_size = 5;  // type + 4 byte int
            this->items.push_back(
                new BiFlatTreeItem(this->binary_data + cursor, additional_size)
            );
            break;
        case 0x02:  // real
            additional_size = 9;  // type + 8 byte float (double)
            this->items.push_back(
                new BiFlatTreeItem(this->binary_data + cursor, additional_size)
            );
            break;
        case 0x03:
            additional_size = 2;  // type + 1 byte bool
            this->items.push_back(
                new BiFlatTreeItem(this->binary_data + cursor, additional_size)
            );
            break;
        case 0x04:  // string
            {
                additional_size = strlen((char*) this->binary_data + cursor) + 1;  // type + strlen()
                this->items.push_back(
                    new BiFlatTreeItem(this->binary_data + cursor, additional_size)
                );
                break;
            }
        case 0x05:
        case 0x15:
            {
                unsigned char* binary_position = this->binary_data + cursor;

                cursor += 1;
                unsigned int nItems;  // number of (items+lists) in list
                if (type == 0x05)
                    nItems = this->binary_data[cursor];  // 1 byte unsigned integer
                if (type == 0x15)
                    nItems = *((unsigned int*) this->binary_data + cursor);  // 4 bytes unsigned integer

                unsigned char subtype;
                unsigned int item_count = 0;
                unsigned int list_count = 0;
                for(unsigned int i = 0; i < nItems; i++) {
                    subtype = this->binary_data[cursor+1];

                    if (subtype == 0x05 || subtype == 0x15 || subtype == 0x06 || subtype == 0x16)  // if type is list-like
                        list_count += 1;
                    else
                        item_count += 1;

                    this->_buildNode(cursor, i);
                }

                this->structure.push_back(
                    new BiFlatTreeList(
                        binary_position,
                        cursor - (unsigned int) (binary_position - this->binary_data),
                        item_count,
                        list_count,
                        parent_index
                    )
                );

                break;
            }
        case 0x06:
        case 0x16:
            {
                // TOFIX: Как учитывать именованные списки?
                // Например, отдельная таблица с общей таблицей ключей из всех списков
                break;
            }
        case 0x07:
        case 0x17:
            {
                if (type == 0x07)
                    additional_size = this->binary_data[cursor + 1] + 1;  // type + value of 1 byte size
                if (type == 0x17)
                    additional_size = *((unsigned int*) this->binary_data[cursor + 1]) + 1;  // type + value of 4 byte size

                this->items.push_back(
                    new BiFlatTreeItem(this->binary_data + cursor, additional_size)
                );

                cursor += additional_size;
                break;
            }
        default:
            throw 1;  // ERROR: not a BiDaT message
    }

    cursor += additional_size;
}

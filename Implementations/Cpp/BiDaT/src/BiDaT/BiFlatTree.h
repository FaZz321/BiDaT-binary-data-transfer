#ifndef __BIFLATTREE_H__
#define __BIFLATTREE_H__

// dependencies:
#include <vector>
#include <cstring>

struct BiFlatTreeList {
    unsigned char* binary_position;  // binary position of a list
    unsigned int size;  // size of list in bytes
    unsigned int item_count;  // number of items in a list
    unsigned int list_count;  // number of inner lists in a list
    unsigned int parent_index;  // index inside a parent list

    BiFlatTreeList(
        unsigned char* binary_position,
        unsigned int size,
        unsigned int item_count,
        unsigned int list_count,
        unsigned int parent_index
    );
};

struct BiFlatTreeItem {
    unsigned char* binary_position;  // position in raw data
    unsigned int size;  // size (in bytes) of an item

    BiFlatTreeItem(
        unsigned char* binary_position,
        unsigned int size
    );
};

class BiFlatTree {
    private:
        // fields:
        unsigned char* binary_data;
        unsigned int message_size;  // binary_data size in bytes

        std::vector<BiFlatTreeList*> structure;
        std::vector<BiFlatTreeItem*> items;

        // methods:
        void _buildTree();  // fills flat tree from binary_data
        void _buildNode(unsigned int& cursor, unsigned int parent_index = 0);

    public:
        // init:
        BiFlatTree(unsigned char* binary_data);
        ~BiFlatTree();

        // read:
        // write:
        // modify:
};


#endif

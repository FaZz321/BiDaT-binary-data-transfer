#ifndef __BIRECORD_H__
#define __BIRECORD_H__

// dependencies:
#include "BiUtils.h"
#include "BiFlatTree.h"

////////////
// Common //
////////////

class BiRecord {
    private:
        unsigned char* binary_data;  // pointer to a binary data of a message

        BI_ACCESS_MODE access_mode;

        BiFlatTree* flatTree;  // moderate mode only
        // BiNodeTree* nodeTree;  // busy mode only

        // changing modes:
        void initToLazy();
        void fromLazyToModerate();
        void fromModerateToBusy();
        void fromLazyToBusy();

    public:
        BiRecord(unsigned char* binary_data);
        ~BiRecord();

        void moderateMode();
        void busyMode();
};



#endif

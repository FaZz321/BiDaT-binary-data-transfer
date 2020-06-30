#include "BiRecord.h"

//////////////
// BiRecord //
//////////////

BiRecord::BiRecord(unsigned char* binary_data) {
    if (!binary_data)
        throw 1;  // ERROR: message is empty

    this->binary_data = binary_data;
    this->flatTree = 0;

    this->initToLazy();
}

BiRecord::~BiRecord() {
    // DO NOT TRY TO DELETE "this->binary_data", this class is basically a wrapper around it
}

void BiRecord::initToLazy() {
    this->access_mode = BI_LAZY;
}

void BiRecord::fromLazyToModerate() {
    this->access_mode = BI_MODERATE;

    if (this->flatTree)
        throw 1;  // ERROR: flat tree already created

    this->flatTree = new BiFlatTree(this->binary_data);
}

void BiRecord::fromModerateToBusy() {
    this->access_mode = BI_BUSY;

    // use flat tree to create node tree
}

void BiRecord::fromLazyToBusy() {
    // TOFIX: Временное решение. Менять режим сразу, то, скорее всего, получится быстрее.

    this->fromLazyToModerate();
    this->fromModerateToBusy();
}

void BiRecord::moderateMode() {
    if (this->access_mode == BI_LAZY)
        this->fromLazyToModerate();
}

void BiRecord::busyMode() {
    if (this->access_mode == BI_LAZY)
        this->fromLazyToBusy();
    if (this->access_mode == BI_MODERATE)
        this->fromModerateToBusy();
}

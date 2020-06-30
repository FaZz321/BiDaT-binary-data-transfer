#include "BiNodes.h"

////////////
// BiNode //
////////////

BiNode::BiNode(BI_TYPE type) {
	this->type = type;
}

BiNode::~BiNode() {}

BiNode* BiNode::byIndex(unsigned int index) {
	// indexing action is not defined
	throw 1;  // TOFIX: make BI_ERR_* enum
}

BiNode* BiNode::byName(const char* name) {
	// name indexing action is not defined
	throw 1;  // TOFIX: make BI_ERR_* enum
}

BI_TYPE BiNode::getType() {
    return this->type;
}


///////////////
// BiInteger //
///////////////

BiInteger::BiInteger(int value):BiNode(BI_INTEGER) {
    this->value = value;
}

BiInteger::~BiInteger() {}

void BiInteger::setValue(int new_value) {
    this->value = new_value;
}

int BiInteger::getValue() {
    return this->value;
}


////////////
// BiReal //
////////////

BiReal::BiReal(double value):BiNode(BI_REAL) {
    this->value = value;
}

BiReal::~BiReal() {}

void BiReal::setValue(double new_value) {
    this->value = new_value;
}

double BiReal::getValue() {
    return this->value;
}


////////////
// BiBool //
////////////

BiBool::BiBool(bool value):BiNode(BI_BOOL) {
    this->value = value;
}

BiBool::~BiBool() {}

void BiBool::setValue(bool new_value) {
    this->value = new_value;
}

bool BiBool::getValue() {
    return this->value;
}


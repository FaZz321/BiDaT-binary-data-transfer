#ifndef __BIUTILS_H__
#define __BIUTILS_H__

//////////////////////
// CONSTS AND ENUMS //
//////////////////////

enum BI_TYPE {
    BI_EMPTY,  // TOFIX: do I need that?
	BI_INTEGER,
	BI_REAL,
	BI_STRING,
	BI_BOOL,
	BI_LIST,
	BI_NAMED_LIST,
	BI_BINARY
};

enum BI_KEY_TYPE {
	BI_INDEX,
	BI_NAME
};

enum BI_ACCESS_MODE {
    BI_LAZY,
    BI_MODERATE,
    BI_BUSY
};

///////////////
// FUNCTIONS //
///////////////

#endif

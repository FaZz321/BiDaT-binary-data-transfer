#ifndef __BINODES_H__
#define __BINODES_H__

// dependencies:
#include "BiUtils.h"

class BiNode {
	private:
		unsigned int binary_index;  // position of node in raw binary data
		BI_TYPE type;  // child type BiNode

	protected:
		void setType(BI_TYPE newType);

	public:
		BiNode(BI_TYPE type, unsigned int binary_index = 0);
		virtual ~BiNode();

		virtual BiNode* byIndex(unsigned int index);
		virtual BiNode* byName(const char* name);

		BI_TYPE getType();
};

class BiInteger: public BiNode {
    private:
        int value;

    public:
        BiInteger(int value);
        ~BiInteger();

        void setValue(int new_value);
        int getValue();
};

class BiReal: public BiNode {
    private:
        double value;

    public:
        BiReal(double value);
        ~BiReal();

        void setValue(double new_value);
        double getValue();
};

class BiBool: public BiNode {
    private:
        bool value;

    public:
        BiBool(bool value);
        ~BiBool();

        void setValue(bool new_value);
        bool getValue();
};

#endif

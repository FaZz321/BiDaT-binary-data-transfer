// BiDaT includes:
#include "BiDaT/BiLists.h"

////////////////
// DEBUG ONLY //
////////////////

#include <iostream>
using namespace std;

int main() {
    BiReal* n1 = new BiReal(1.2);
    BiReal* n2 = new BiReal(2.3);
    BiInteger* n3 = new BiInteger(5);

    BiList na;

    na.pushFront(n1);
    na.pushFront(n2);
    na.pushFront(n3);

    BiReal* n11 = (BiReal*) na.byIndex(0);
    BiReal* n12 = (BiReal*) na.byIndex(1);
    BiInteger* n13 = (BiInteger*) na.byIndex(2);

    double v1 = n11->getValue();
    double v2 = n12->getValue();
    int v3 = n13->getValue();

	std::cout << v1 << std::endl;
	std::cout << v2 << std::endl;
	std::cout << v3 << std::endl;

	system("pause");
	return 0;
}

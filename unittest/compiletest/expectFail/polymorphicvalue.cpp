#include "nfse.h"

using namespace NFSE;

class NoDefaultCTor {
    NoDefaultCTor(int) {}
};

int main() {
    PolymorphicValue<NoDefaultCTor> pmv;
}

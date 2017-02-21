#include <iostream>

#include "InfixTransform.h"

using namespace std;

int main() {
    std::string s("0(0|1)*1+");
    auto _s = InfixTransform::toPostfix(s);

    cout << *_s << endl;
    return 0;
}
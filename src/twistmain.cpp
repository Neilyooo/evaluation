#include "twistmain.h"
#include <iostream>

TwistMain::TwistMain(int param1, int param2) : param1(param1), param2(param2) {}

void TwistMain::twistmain_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

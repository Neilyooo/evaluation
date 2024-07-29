#include "posemain.h"
#include <iostream>

PoseMain::PoseMain(int param1, int param2) : param1(param1), param2(param2) {}

void PoseMain::posemain_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

#include "adjust.h"
#include <iostream>

Adjust::Adjust(int param1, int param2) : param1(param1), param2(param2) {}

void Adjust::adjust_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

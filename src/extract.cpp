#include "extract.h"
#include <iostream>

Extract::Extract(int param1, int param2) : param1(param1), param2(param2) {}

void Extract::extract_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

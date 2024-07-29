#include "readros2bag.h"
#include <iostream>

ReadRos2bag::ReadRos2bag(int param1, int param2) : param1(param1), param2(param2) {}

void ReadRos2bag::readros2bag_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

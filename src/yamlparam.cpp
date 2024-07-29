#include "yamlparam.h"
#include <iostream>

YamlParam::YamlParam(int param1, int param2) : param1(param1), param2(param2) {}

void YamlParam::yamlparam_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

#include "subndtevaluation.h"
#include <iostream>

SubNdtEvaluation::SubNdtEvaluation(int param1, int param2) : param1(param1), param2(param2) {}

void SubNdtEvaluation::subndtevaluation_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

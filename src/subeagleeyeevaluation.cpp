#include "subeagleeyeevaluation.h"
#include <iostream>

SubEagleeyeEvaluation::SubEagleeyeEvaluation(int param1, int param2) : param1(param1), param2(param2) {}

void SubEagleeyeEvaluation::subeagleeyeevaluation_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

#include "utileagleeye.h"
#include <iostream>

UtilEagleeye::UtilEagleeye(int param1, int param2) : param1(param1), param2(param2) {}

void UtilEagleeye::utileagleeye_method() {
    std::cout << "Adjusting with parameters: " << param1 << ", " << param2 << std::endl;
}

#include <cassert>
#include "color.h"
#include <sstream>
#include <iostream>

void test_scale_color() {
    std::ostringstream oss;
    Color color(0.5, 0.75, 1.0);
    writeColor(oss, color);
    std::string expectedOutput = "127 191 255\n";
    assert(expectedOutput == oss.str());
}

int main() {
    std::cout << "Running tests...\n";
    test_scale_color();
    std::cout << "Color tests passed!\n";
    return 0;
}
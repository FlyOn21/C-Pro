//
// Created by flyon21 on 26.10.24.
//

#include <iostream>
#include "Rectangle.h"

int main() {
    constexpr Rectangle rect_const{10.25, 5.25};
    std::cout << "Compile-time area: " << rect_const.area() << std::endl;

    double width, height;
    std::cout << "Enter width: ";
    std::cin >> width;
    std::cout << "Enter height: ";
    std::cin >> height;

    Rectangle rect_dynamic{width, height};
    std::cout << "Run-time area: " << rect_dynamic.area() << std::endl;

    return 0;
}
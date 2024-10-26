//
// Created by flyon21 on 26.10.24.
//

#ifndef C_PRO_RECTANGLE_H
#define C_PRO_RECTANGLE_H

struct Rectangle {
    double width;
    double height;

    constexpr double area() const {
        return width * height;
    }
};

#endif //C_PRO_RECTANGLE_H

//
// Created by flyon21 on 07.12.24.
//

#ifndef C_PRO_CONVERTER_H_converter
#define C_PRO_CONVERTER_H_converter

#include <iostream>
#include <string>
#include <stdexcept>

enum class ConversionType { Length, Mass, Temperature };

template <ConversionType Type>
double convert(double value, const std::string& from, const std::string& to) {
    if constexpr (Type == ConversionType::Length) {
        if (from == "m" && to == "km") return value / 1000.0;
        else if (from == "km" && to == "m") return value * 1000.0;
        else if (from == "cm" && to == "m") return value / 100.0;
        else if (from == "m" && to == "cm") return value * 100.0;
        else throw std::invalid_argument("Unsupported length conversion");
    } else if constexpr (Type == ConversionType::Mass) {
        if (from == "kg" && to == "g") return value * 1000.0;
        else if (from == "g" && to == "kg") return value / 1000.0;
        else if (from == "lb" && to == "kg") return value * 0.453592;
        else if (from == "kg" && to == "lb") return value / 0.453592;
        else throw std::invalid_argument("Unsupported mass conversion");
    } else if constexpr (Type == ConversionType::Temperature) {
        if (from == "C" && to == "F") return (value * 9.0 / 5.0) + 32.0;
        else if (from == "F" && to == "C") return (value - 32.0) * 5.0 / 9.0;
        else if (from == "C" && to == "K") return value + 273.15;
        else if (from == "K" && to == "C") return value - 273.15;
        else throw std::invalid_argument("Unsupported temperature conversion");
    } else {
        throw std::logic_error("Invalid conversion type");
    }
}

#endif //C_PRO_CONVERTER_H_converter

//
// Created by flyon21 on 07.12.24.
//

#ifndef C_PRO_CONVERTER_H_converter
#define C_PRO_CONVERTER_H_converter

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

enum class ConversionType { Length, Mass, Temperature };

template <ConversionType Type>
struct ConversionFactor {
    const char* from = nullptr;
    const char* to = nullptr;
    double factor;
    double (*convert)(double) = nullptr;
};

std::vector<std::string> getAbleUnits(const ConversionType type) {
    std::vector<std::string> units;
    if (type == ConversionType::Length) {
        units = {"m", "km", "cm"};
    } else if (type == ConversionType::Mass) {
        units = {"kg", "g", "lb"};
    } else if (type == ConversionType::Temperature) {
        units = {"C", "F", "K"};
    }
    return units;
}


template <ConversionType Type>
const std::vector<ConversionFactor<Type>>& getConversionTable() {
    if constexpr (Type == ConversionType::Length) {
        static const std::vector<ConversionFactor<Type>> table = {
                {"m", "km", 1.0 / 1000.0},
                {"km", "m", 1000.0},
                {"cm", "m", 1.0 / 100.0},
                {"m", "cm", 100.0}
        };
        return table;
    } else if constexpr (Type == ConversionType::Mass) {
        static const std::vector<ConversionFactor<Type>> table = {
                {"kg", "g", 1000.0},
                {"g", "kg", 1.0 / 1000.0},
                {"lb", "kg", 0.453592},
                {"kg", "lb", 1.0 / 0.453592}
        };
        return table;
    } else if constexpr (Type == ConversionType::Temperature) {
        static const std::vector<ConversionFactor<Type>> table = {
                {"C", "F", 1.0, [](double value) { return (value * 9.0 / 5.0) + 32.0; }},
                {"F", "C", 1.0, [](double value) { return (value - 32.0) * 5.0 / 9.0; }},
                {"C", "K", 1.0, [](double value) { return value + 273.15; }},
                {"K", "C", 1.0, [](double value) { return value - 273.15; }}
        };
        return table;
    } else {
        throw std::logic_error("Unsupported conversion type");
    }
}

template <ConversionType Type>
double convert(double value, const std::string& from, const std::string& to) {
    const auto& table = getConversionTable<Type>();
    for (const auto& conv : table) {
        if (from == conv.from && to == conv.to) {
            return conv.convert ? conv.convert(value) : value * conv.factor;
        }
    }
    throw std::invalid_argument("Unsupported conversion, from: " + from + " to: " + to + ". May be next app version will support it.");
}



#endif //C_PRO_CONVERTER_H_converter

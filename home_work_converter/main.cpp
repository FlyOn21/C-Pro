//
// Created by flyon21 on 07.12.24.
//
#include "Converter.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>


void displayMenu() {
    std::cout << "Unit Conversion Application\n";
    std::cout << "1. Length (m, km, cm)\n";
    std::cout << "2. Mass (kg, g, lb)\n";
    std::cout << "3. Temperature (C, F, K)\n";
    std::cout << "0. Exit\n";
    std::cout << "Select conversion type: ";
}

int getMenuChoice() {
    int choice;
    while (true) {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail() || choice < 0 || choice > 3) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again.\n\n";
        } else {
            break;
        }
    }
    return choice;
}

double getConversionValue() {
    double value;
    while (true) {
        std::cout << "Enter value to convert: ";
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again.\n\n";
        } else {
            break;
        }
    }
    return value;
}

std::string getValidUnit(const std::vector<std::string>& availableValues, const std::string& prompt) {
    std::string unit;
    while (true) {
        std::cout << prompt << " (" << [&availableValues]() {
            std::string units;
            for (const auto& u : availableValues) units += u + ", ";
            return units.substr(0, units.size() - 2);
        }() << "): ";
        std::cin >> unit;

        if (std::find(availableValues.begin(), availableValues.end(), unit) != availableValues.end()) {
            break;
        }
        std::cout << "Invalid unit. Please try again.\n";
    }
    return unit;
}

void performConversion(int choice) {
    double value = getConversionValue();

    std::vector<std::string> availableValues;

    try {
        switch (choice) {
            case 1: {
                availableValues = getAbleUnits(ConversionType::Length);
                std::string from = getValidUnit(availableValues, "Enter from unit");
                std::string to = getValidUnit(availableValues, "Enter to unit");
                double result = convert<ConversionType::Length>(value, from, to);
                std::cout << "Converted value: " << result << " " << to << "\n\n";
                break;
            }
            case 2: {
                availableValues = getAbleUnits(ConversionType::Mass);
                std::string from = getValidUnit(availableValues, "Enter from unit");
                std::string to = getValidUnit(availableValues, "Enter to unit");
                double result = convert<ConversionType::Mass>(value, from, to);
                std::cout << "Converted value: " << result << " " << to << "\n\n";
                break;
            }
            case 3: {
                availableValues = getAbleUnits(ConversionType::Temperature);
                std::string from = getValidUnit(availableValues, "Enter from unit");
                std::string to = getValidUnit(availableValues, "Enter to unit");
                double result = convert<ConversionType::Temperature>(value, from, to);
                std::cout << "Converted value: " << result << " " << to << "\n\n";
                break;
            }
            default:
                std::cout << "Invalid choice. Returning to main menu.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n\n";
    }
}

int main() {
    while (true) {
        int choice = getMenuChoice();

        if (choice == 0) {
            std::cout << "Exiting the application. Goodbye!\n";
            break;
        }

        performConversion(choice);
    }

    return 0;
}
//
// Created by flyon21 on 07.12.24.
//
#include "Converter.h"

void displayMenu() {
    std::cout << "Unit Conversion Application\n";
    std::cout << "1. Length (m, km, cm)\n";
    std::cout << "2. Mass (kg, g, lb)\n";
    std::cout << "3. Temperature (C, F, K)\n";
    std::cout << "0. Exit\n";
    std::cout << "Select conversion type: ";
}

int main() {
    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Exiting the application. Goodbye!\n";
            break;
        }

        double value;
        std::string from, to;

        std::cout << "Enter value to convert: ";
        std::cin >> value;
        std::cout << "Enter from unit: ";
        std::cin >> from;
        std::cout << "Enter to unit: ";
        std::cin >> to;

        try {
            double result = 0.0;

            switch (choice) {
                case 1:
                    result = convert<ConversionType::Length>(value, from, to);
                    break;
                case 2:
                    result = convert<ConversionType::Mass>(value, from, to);
                    break;
                case 3:
                    result = convert<ConversionType::Temperature>(value, from, to);
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    continue;
            }

            std::cout << "Converted value: " << result << " " << to << "\n\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n\n";
        }
    }

    return 0;
}

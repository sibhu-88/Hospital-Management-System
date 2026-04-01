#include "../include/Utils.h"
#include <iostream>
#include <string>
#include <limits>

#ifdef _WIN32
#include <windows.h>  // for Windows clear screen fallback
#endif

namespace Utils {

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void pause(const std::string& message) {
        std::cout << "\n" << message;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    void printHeader(const std::string& title) {
        clearScreen();
        std::cout << CYAN << BOLD
                  << "============================================================\n"
                  << "          " << title << "\n"
                  << "============================================================\n"
                  << RESET << "\n";
    }

    int getValidInt(const std::string& prompt, int min, int max) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                if (value >= min && value <= max) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return value;
                } else {
                    printWarning("Value must be between " + std::to_string(min) + " and " + std::to_string(max) + ".");
                }
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                printError("Invalid input. Please enter a number.");
            }
        }
    }

    std::string getValidString(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        // Basic trim (remove leading/trailing spaces)
        size_t start = input.find_first_not_of(" \t");
        if (start == std::string::npos) return "";
        size_t end = input.find_last_not_of(" \t");
        return input.substr(start, end - start + 1);
    }

    void printSuccess(const std::string& msg) {
        std::cout << GREEN << "✓ " << msg << RESET << "\n";
    }

    void printError(const std::string& msg) {
        std::cout << RED << "✗ " << msg << RESET << "\n";
    }

    void printWarning(const std::string& msg) {
        std::cout << YELLOW << "! " << msg << RESET << "\n";
    }

}
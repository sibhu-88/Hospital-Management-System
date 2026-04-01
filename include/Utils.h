#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <limits>

// ANSI color codes (works on Linux/Mac + most Windows terminals with enabled ANSI)
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

namespace Utils {

    void clearScreen();
    void pause(const std::string& message = "Press Enter to continue...");
    void printHeader(const std::string& title);
    int getValidInt(const std::string& prompt, int min = 0, int max = 999999);
    std::string getValidString(const std::string& prompt);
    void printSuccess(const std::string& msg);
    void printError(const std::string& msg);
    void printWarning(const std::string& msg);

}

#endif
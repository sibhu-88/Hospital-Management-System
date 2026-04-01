#include "../include/Inventory.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>

std::vector<Medicine> Medicine::allMedicines;
int Medicine::nextId = 10001;

Medicine::Medicine() : id(0), pricePerUnit(0.0), quantityInStock(0), lowStockThreshold(10) {}

Medicine::Medicine(int i, std::string n, std::string cat, double p, int q, int thresh)
    : id(i), name(std::move(n)), category(std::move(cat)),
      pricePerUnit(p), quantityInStock(q), lowStockThreshold(thresh) {}

int Medicine::getId() const { return id; }
std::string Medicine::getName() const { return name; }
int Medicine::getQuantity() const { return quantityInStock; }
double Medicine::getPrice() const { return pricePerUnit; }

bool Medicine::isLowStock() const {
    return quantityInStock <= lowStockThreshold;
}

void Medicine::display() const {
    std::cout << std::left
              << std::setw(8)  << id
              << std::setw(25) << name
              << std::setw(15) << category
              << std::setw(12) << pricePerUnit
              << std::setw(12) << quantityInStock
              << (isLowStock() ? "LOW STOCK!" : "") << "\n";
}

void Medicine::addStock(int amount) {
    if (amount > 0) {
        quantityInStock += amount;
        std::cout << "Added " << amount << " units. New stock: " << quantityInStock << "\n";
    }
}

bool Medicine::reduceStock(int amount) {
    if (amount <= 0 || amount > quantityInStock) {
        std::cout << "Invalid amount or insufficient stock.\n";
        return false;
    }
    quantityInStock -= amount;
    std::cout << "Reduced " << amount << " units. Remaining: " << quantityInStock << "\n";
    return true;
}

void Medicine::addNew() {
    int id = nextId++;
    std::string name, category;
    double price;
    int qty, threshold;

    std::cout << "\n=== Add New Medicine/Item ===\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Name                  : "; std::getline(std::cin, name);
    std::cout << "Category              : "; std::getline(std::cin, category);
    std::cout << "Price per unit (₹)    : "; std::cin >> price;
    std::cout << "Initial Quantity      : "; std::cin >> qty;
    std::cout << "Low stock threshold   : "; std::cin >> threshold;

    allMedicines.emplace_back(id, name, category, price, qty, threshold);
    std::cout << "✅ Item added (ID: " << id << ")\n";
    saveToFile();
}

void Medicine::viewAll(bool showOnlyLow) {
    if (allMedicines.empty()) {
        std::cout << "No items in inventory.\n";
        return;
    }

    std::cout << "\n=== Inventory " << (showOnlyLow ? "(Low Stock Only)" : "") << " ===\n";
    std::cout << std::left
              << std::setw(8)  << "ID"
              << std::setw(25) << "Name"
              << std::setw(15) << "Category"
              << std::setw(12) << "Price ₹"
              << std::setw(12) << "Stock"
              << "Status\n";
    std::cout << std::string(80, '-') << "\n";

    bool anyShown = false;
    for (const auto& m : allMedicines) {
        if (!showOnlyLow || m.isLowStock()) {
            m.display();
            anyShown = true;
        }
    }
    if (!anyShown && showOnlyLow) {
        std::cout << "No low-stock items currently.\n";
    }
}

void Medicine::searchByName() {
    std::string search;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter medicine name (partial OK): ";
    std::getline(std::cin, search);

    bool found = false;
    std::cout << "\nMatching items:\n";
    for (const auto& m : allMedicines) {
        if (m.getName().find(search) != std::string::npos) {
            m.display();
            found = true;
        }
    }
    if (!found) std::cout << "No matches found.\n";
}

void Medicine::updateStock() {
    int id, change;
    char action;

    std::cout << "Enter Medicine ID: ";
    std::cin >> id;
    std::cout << "Add (+) or Reduce (-) stock? (a/r): ";
    std::cin >> action;
    std::cout << "Amount: ";
    std::cin >> change;

    for (auto& m : allMedicines) {
        if (m.getId() == id) {
            if (action == 'a' || action == 'A') {
                m.addStock(change);
            } else if (action == 'r' || action == 'R') {
                m.reduceStock(change);
            } else {
                std::cout << "Invalid action.\n";
                return;
            }
            saveToFile();
            return;
        }
    }
    std::cout << "Medicine ID not found.\n";
}

void Medicine::saveToFile() {
    std::ofstream file("../data/inventory.txt");
    if (!file.is_open()) {
        std::cout << "Warning: Could not save inventory.\n";
        return;
    }
    for (const auto& m : allMedicines) {
        file << m.id << "|" << m.name << "|" << m.category << "|"
             << m.pricePerUnit << "|" << m.quantityInStock << "|"
             << m.lowStockThreshold << "\n";
    }
}

void Medicine::loadFromFile() {
    allMedicines.clear();
    std::ifstream file("../data/inventory.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string token;
        int id, qty, thresh;
        double price;
        std::string name, cat;

        std::getline(iss, token, '|'); id = std::stoi(token);
        std::getline(iss, name, '|');
        std::getline(iss, cat, '|');
        std::getline(iss, token, '|'); price = std::stod(token);
        std::getline(iss, token, '|'); qty = std::stoi(token);
        std::getline(iss, token); thresh = std::stoi(token);

        allMedicines.emplace_back(id, name, cat, price, qty, thresh);
        nextId = std::max(nextId, id + 1);
    }
}
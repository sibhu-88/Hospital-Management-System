#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>

class Medicine {
private:
    int id;
    std::string name;
    std::string category;       // e.g., "Tablet", "Syrup", "Injection", "General"
    double pricePerUnit;
    int quantityInStock;
    int lowStockThreshold;      // alert when below this level

public:
    Medicine();
    Medicine(int id, std::string name, std::string category, 
             double price, int qty, int threshold = 10);

    void display() const;
    int getId() const;
    std::string getName() const;
    int getQuantity() const;
    double getPrice() const;
    bool isLowStock() const;
    void addStock(int amount);
    bool reduceStock(int amount);   // returns true if reduction was successful

    static void addNew();
    static void viewAll(bool showOnlyLow = false);
    static void searchByName();
    static void updateStock();
    static void saveToFile();
    static void loadFromFile();

    static std::vector<Medicine> allMedicines;
    static int nextId;          // auto-increment starting from 10001
};

#endif
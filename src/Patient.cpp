#include "../include/Patient.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>

std::vector<Patient> Patient::allPatients;

Patient::Patient() : id(0), age(0) {}

std::string Patient::getName() const
{
    return name;
}

Patient::Patient(int i, std::string n, int a, std::string g, std::string ad, std::string p)
    : id(i), name(std::move(n)), age(a), gender(std::move(g)),
      address(std::move(ad)), phone(std::move(p)) {}

int Patient::getId() const { return id; }

void Patient::display() const
{
    std::cout << std::left << std::setw(6) << id
              << std::setw(22) << name
              << std::setw(6) << age
              << std::setw(12) << gender
              << std::setw(16) << phone
              << address << "\n";
}

void Patient::addNew()
{
    int id, age;
    std::string name, gender, address, phone;

    std::cout << "\n=== Add New Patient ===\n";
    std::cout << "Enter ID          : ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Name        : ";
    std::getline(std::cin, name);
    std::cout << "Enter Age         : ";
    std::cin >> age;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Gender (M/F): ";
    std::getline(std::cin, gender);
    std::cout << "Enter Address     : ";
    std::getline(std::cin, address);
    std::cout << "Enter Phone       : ";
    std::getline(std::cin, phone);

    allPatients.emplace_back(id, name, age, gender, address, phone);
    std::cout << "✅ Patient added successfully!\n";
    saveToFile();
}

void Patient::viewAll()
{
    if (allPatients.empty())
    {
        std::cout << "No patients found.\n";
        return;
    }
    std::cout << "\n=== All Patients ===\n";
    std::cout << std::left << std::setw(6) << "ID"
              << std::setw(22) << "Name"
              << std::setw(6) << "Age"
              << std::setw(12) << "Gender"
              << std::setw(16) << "Phone"
              << "Address\n";
    std::cout << std::string(90, '-') << "\n";

    for (const auto &p : allPatients)
    {
        p.display();
    }
}

void Patient::searchById()
{
    int sid;
    std::cout << "Enter Patient ID to search: ";
    std::cin >> sid;

    bool found = false;
    for (const auto &p : allPatients)
    {
        if (p.getId() == sid)
        {
            std::cout << "\n✅ Patient Found:\n";
            p.display();
            found = true;
            break;
        }
    }
    if (!found)
    {
        std::cout << "❌ Patient with ID " << sid << " not found.\n";
    }
}

void Patient::saveToFile()
{
    std::ofstream file("../data/patients.txt");
    if (!file.is_open())
    {
        std::cout << "Warning: Could not save to file.\n";
        return;
    }
    for (const auto &p : allPatients)
    {
        file << p.id << "|" << p.name << "|" << p.age << "|"
             << p.gender << "|" << p.address << "|" << p.phone << "\n";
    }
}

void Patient::loadFromFile()
{
    allPatients.clear();
    std::ifstream file("../data/patients.txt");
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        std::string token;
        int id, age;
        std::string name, gender, address, phone;

        std::getline(iss, token, '|');
        id = std::stoi(token);
        std::getline(iss, name, '|');
        std::getline(iss, token, '|');
        age = std::stoi(token);
        std::getline(iss, gender, '|');
        std::getline(iss, address, '|');
        std::getline(iss, phone);

        allPatients.emplace_back(id, name, age, gender, address, phone);
    }
}
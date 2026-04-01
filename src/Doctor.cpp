#include "../include/Doctor.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>

std::vector<Doctor> Doctor::allDoctors;

Doctor::Doctor() : id(0), available(true) {}
Doctor::Doctor(int i, std::string n, std::string spec, std::string qual,
               std::string p, std::string e, bool avail)
    : id(i), name(std::move(n)), specialization(std::move(spec)),
      qualification(std::move(qual)), phone(std::move(p)),
      email(std::move(e)), available(avail) {}

int Doctor::getId() const { return id; }
std::string Doctor::getName() const { return name; }
std::string Doctor::getSpecialization() const { return specialization; }
bool Doctor::isAvailable() const { return available; }
void Doctor::setAvailability(bool status) { available = status; }

void Doctor::display() const
{
    std::cout << std::left << std::setw(6) << id
              << std::setw(22) << name
              << std::setw(20) << specialization
              << std::setw(15) << qualification
              << std::setw(16) << phone
              << std::setw(25) << email
              << (available ? "Yes" : "No") << "\n";
}

void Doctor::addNew()
{
    int id;
    std::string name, spec, qual, phone, email;
    char availChar;

    std::cout << "\n=== Add New Doctor ===\n";
    std::cout << "Enter ID               : ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Name             : ";
    std::getline(std::cin, name);
    std::cout << "Enter Specialization   : ";
    std::getline(std::cin, spec);
    std::cout << "Enter Qualification    : ";
    std::getline(std::cin, qual);
    std::cout << "Enter Phone            : ";
    std::getline(std::cin, phone);
    std::cout << "Enter Email            : ";
    std::getline(std::cin, email);

    std::cout << "Available for appointments? (y/n): ";
    std::cin >> availChar;
    bool available = (availChar == 'y' || availChar == 'Y');

    allDoctors.emplace_back(id, name, spec, qual, phone, email, available);
    std::cout << "✅ Doctor added successfully!\n";
    saveToFile();
}

void Doctor::viewAll()
{
    if (allDoctors.empty())
    {
        std::cout << "No doctors found.\n";
        return;
    }

    std::cout << "\n=== All Doctors ===\n";
    std::cout << std::left
              << std::setw(6) << "ID"
              << std::setw(22) << "Name"
              << std::setw(20) << "Specialization"
              << std::setw(15) << "Qualification"
              << std::setw(16) << "Phone"
              << std::setw(25) << "Email"
              << "Available\n";
    std::cout << std::string(110, '-') << "\n";

    for (const auto &d : allDoctors)
    {
        d.display();
    }
}

void Doctor::searchById()
{
    int sid;
    std::cout << "Enter Doctor ID to search: ";
    std::cin >> sid;

    bool found = false;
    for (const auto &d : allDoctors)
    {
        if (d.getId() == sid)
        {
            std::cout << "\n✅ Doctor Found:\n";
            d.display();
            found = true;
            break;
        }
    }
    if (!found)
    {
        std::cout << "❌ Doctor with ID " << sid << " not found.\n";
    }
}

void Doctor::searchBySpecialization()
{
    std::string spec;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter Specialization to search: ";
    std::getline(std::cin, spec);

    bool found = false;
    std::cout << "\nDoctors matching '" << spec << "':\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto &d : allDoctors)
    {
        if (d.getSpecialization().find(spec) != std::string::npos)
        {
            d.display();
            found = true;
        }
    }
    if (!found)
    {
        std::cout << "No doctors found with specialization containing '" << spec << "'.\n";
    }
}

void Doctor::toggleAvailability()
{
    int id;
    std::cout << "Enter Doctor ID to change availability: ";
    std::cin >> id;

    for (auto &d : allDoctors)
    {
        if (d.getId() == id)
        {
            d.setAvailability(!d.isAvailable());
            std::cout << "Availability updated → "
                      << (d.isAvailable() ? "Available" : "Not Available") << "\n";
            saveToFile();
            return;
        }
    }
    std::cout << "Doctor with ID " << id << " not found.\n";
}

void Doctor::saveToFile()
{
    std::ofstream file("../data/doctors.txt");
    if (!file.is_open())
    {
        std::cout << "Warning: Could not save doctors to file.\n";
        return;
    }

    for (const auto &d : allDoctors)
    {
        file << d.id << "|"
             << d.name << "|"
             << d.specialization << "|"
             << d.qualification << "|"
             << d.phone << "|"
             << d.email << "|"
             << (d.available ? "1" : "0") << "\n";
    }
}

void Doctor::loadFromFile()
{
    allDoctors.clear();
    std::ifstream file("../data/doctors.txt");
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        std::string token;
        int id;
        std::string name, spec, qual, phone, email;
        bool available;

        std::getline(iss, token, '|');
        id = std::stoi(token);
        std::getline(iss, name, '|');
        std::getline(iss, spec, '|');
        std::getline(iss, qual, '|');
        std::getline(iss, phone, '|');
        std::getline(iss, email, '|');
        std::getline(iss, token);
        available = (token == "1");

        allDoctors.emplace_back(id, name, spec, qual, phone, email, available);
    }
}
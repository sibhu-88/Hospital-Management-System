#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <vector>

class Doctor
{
private:
    int id;
    std::string name;
    std::string specialization;
    std::string qualification;
    std::string phone;
    std::string email;
    bool available; // true = available for appointments

public:
    Doctor();
    Doctor(int id, std::string name, std::string specialization,
           std::string qualification, std::string phone, std::string email, bool available = true);

    void display() const;
    int getId() const;
    std::string getName() const;
    std::string getSpecialization() const;
    bool isAvailable() const;
    void setAvailability(bool status);

    // Management functions
    static void addNew();
    static void viewAll();
    static void searchById();
    static void searchBySpecialization();
    static void toggleAvailability();
    static void saveToFile();
    static void loadFromFile();

    static std::vector<Doctor> allDoctors;
};

#endif
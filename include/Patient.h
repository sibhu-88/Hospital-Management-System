#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>

class Patient {
private:
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string address;
    std::string phone;

public:
    Patient();
    Patient(int id, std::string name, int age, std::string gender, 
            std::string address, std::string phone);

    void display() const;
    int getId() const;
    std::string getName() const;

    // Management functions
    static void addNew();
    static void viewAll();
    static void searchById();
    static void saveToFile();
    static void loadFromFile();

    static std::vector<Patient> allPatients;
};

#endif
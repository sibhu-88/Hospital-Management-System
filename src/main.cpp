#include <iostream>
#include "../include/Utils.h"
#include "../include/Patient.h"
#include "../include/Doctor.h"
#include "../include/Appointment.h"
#include "../include/Billing.h"
#include "../include/Inventory.h"

using namespace Utils;

int main() {
    Patient::loadFromFile();
    Doctor::loadFromFile();
    Appointment::loadFromFile();
    Bill::loadFromFile();
    Medicine::loadFromFile();

    while (true) {
        printHeader("HOSPITAL / CLINIC MANAGEMENT SYSTEM");

        std::cout << "  1. Patient Management\n";
        std::cout << "  2. Doctor Management\n";
        std::cout << "  3. Appointment Management\n";
        std::cout << "  4. Billing & Payments\n";
        std::cout << "  5. Inventory / Pharmacy\n";
        std::cout << "  0. Exit\n\n";

        int choice = getValidInt("Enter your choice: ", 0, 5);

        if (choice == 0) {
            clearScreen();
            printSuccess("Thank you for using the system. Goodbye!");
            pause("");
            break;
        }

        switch (choice) {
            case 1: {
                while (true) {
                    printHeader("PATIENT MANAGEMENT");
                    std::cout << "  1. Add New Patient\n";
                    std::cout << "  2. View All Patients\n";
                    std::cout << "  3. Search Patient by ID\n";
                    std::cout << "  0. Back\n\n";
                    int sub = getValidInt("Enter sub-choice: ", 0, 3);
                    if (sub == 0) break;
                    if (sub == 1) Patient::addNew();
                    else if (sub == 2) Patient::viewAll();
                    else if (sub == 3) Patient::searchById();
                    pause();
                }
                break;
            }

            case 2: {
                while (true) {
                    printHeader("DOCTOR MANAGEMENT");
                    std::cout << "  1. Add New Doctor\n";
                    std::cout << "  2. View All Doctors\n";
                    std::cout << "  3. Search by ID\n";
                    std::cout << "  4. Search by Specialization\n";
                    std::cout << "  5. Toggle Availability\n";
                    std::cout << "  0. Back\n\n";
                    int sub = getValidInt("Enter sub-choice: ", 0, 5);
                    if (sub == 0) break;
                    if (sub == 1) Doctor::addNew();
                    else if (sub == 2) Doctor::viewAll();
                    else if (sub == 3) Doctor::searchById();
                    else if (sub == 4) Doctor::searchBySpecialization();
                    else if (sub == 5) Doctor::toggleAvailability();
                    pause();
                }
                break;
            }

            case 3: {
                while (true) {
                    printHeader("APPOINTMENT MANAGEMENT");
                    std::cout << "  1. Book New Appointment\n";
                    std::cout << "  2. View All Appointments\n";
                    std::cout << "  3. View by Patient\n";
                    std::cout << "  4. View by Doctor\n";
                    std::cout << "  5. Cancel Appointment\n";
                    std::cout << "  6. Mark as Completed\n";
                    std::cout << "  0. Back\n\n";
                    int sub = getValidInt("Enter sub-choice: ", 0, 6);
                    if (sub == 0) break;
                    if (sub == 1) Appointment::bookNew();
                    else if (sub == 2) Appointment::viewAll();
                    else if (sub == 3) Appointment::viewByPatient();
                    else if (sub == 4) Appointment::viewByDoctor();
                    else if (sub == 5) Appointment::cancelAppointment();
                    else if (sub == 6) Appointment::markCompleted();
                    pause();
                }
                break;
            }

            case 4: {
                while (true) {
                    printHeader("BILLING & PAYMENTS");
                    std::cout << "  1. Generate Bill for Completed Appointment\n";
                    std::cout << "  2. View All Bills\n";
                    std::cout << "  3. View Bills by Patient\n";
                    std::cout << "  4. View Unpaid Bills\n";
                    std::cout << "  5. Mark Bill as Paid\n";
                    std::cout << "  0. Back\n\n";
                    int sub = getValidInt("Enter sub-choice: ", 0, 5);
                    if (sub == 0) break;
                    if (sub == 1) Bill::generateForAppointment();
                    else if (sub == 2) Bill::viewAll();
                    else if (sub == 3) Bill::viewByPatient();
                    else if (sub == 4) Bill::viewUnpaid();
                    else if (sub == 5) Bill::payBill();
                    pause();
                }
                break;
            }

            case 5: {
                while (true) {
                    printHeader("INVENTORY / PHARMACY");
                    std::cout << "  1. Add New Medicine/Item\n";
                    std::cout << "  2. View All Inventory\n";
                    std::cout << "  3. View Low Stock Items Only\n";
                    std::cout << "  4. Search by Name\n";
                    std::cout << "  5. Update Stock (Add/Reduce)\n";
                    std::cout << "  0. Back\n\n";
                    int sub = getValidInt("Enter sub-choice: ", 0, 5);
                    if (sub == 0) break;
                    if (sub == 1) Medicine::addNew();
                    else if (sub == 2) Medicine::viewAll(false);
                    else if (sub == 3) Medicine::viewAll(true);
                    else if (sub == 4) Medicine::searchByName();
                    else if (sub == 5) Medicine::updateStock();
                    pause();
                }
                break;
            }

            default:
                printError("Invalid main menu choice!");
                pause();
        }
    }

    return 0;
}
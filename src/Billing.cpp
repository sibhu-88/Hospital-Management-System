#include "../include/Billing.h"
#include "../include/Appointment.h"
#include "../include/Patient.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>

std::vector<Bill> Bill::allBills;
int Bill::nextBillId = 5001;

Bill::Bill() : billId(0), appointmentId(0), patientId(0),
               consultationFee(0.0), medicineCost(0.0), testCost(0.0),
               totalAmount(0.0) {}

Bill::Bill(int bId, int aId, int pId, double cFee, double mCost, double tCost,
           std::string date, std::string stat)
    : billId(bId), appointmentId(aId), patientId(pId),
      consultationFee(cFee), medicineCost(mCost), testCost(tCost),
      totalAmount(cFee + mCost + tCost),
      dateIssued(std::move(date)), status(std::move(stat)) {}

int Bill::getBillId() const { return billId; }
int Bill::getAppointmentId() const { return appointmentId; }
double Bill::getTotal() const { return totalAmount; }
std::string Bill::getStatus() const { return status; }

void Bill::display() const {
    std::string patName = "Unknown";
    for (const auto& p : Patient::allPatients) {
        if (p.getId() == patientId) {
            patName = p.getName();
            break;
        }
    }

    std::cout << std::left
              << std::setw(10) << billId
              << std::setw(14) << appointmentId
              << std::setw(10) << patientId << std::setw(20) << patName
              << std::setw(12) << consultationFee
              << std::setw(12) << medicineCost
              << std::setw(12) << testCost
              << std::setw(12) << totalAmount
              << dateIssued << "  " << status << "\n";
}

bool Bill::appointmentHasBill(int appId) {
    return std::any_of(allBills.begin(), allBills.end(),
                       [appId](const Bill& b){ return b.appointmentId == appId; });
}

void Bill::generateForAppointment() {
    if (Appointment::allAppointments.empty()) {
        std::cout << "No appointments available.\n";
        return;
    }

    std::cout << "\nCompleted Appointments without bills:\n";
    bool hasAny = false;
    for (const auto& app : Appointment::allAppointments) {
        if (app.getStatus() == "Completed" && !appointmentHasBill(app.getId())) {
            std::cout << "App ID: " << app.getId()
                      << " | Patient: " << app.getPatientId()
                      << " | Date: " << app.getDate() << "\n";
            hasAny = true;
        }
    }

    if (!hasAny) {
        std::cout << "No completed appointments without bills.\n";
        return;
    }

    int appId;
    std::cout << "\nEnter Appointment ID to generate bill for: ";
    std::cin >> appId;

    const Appointment* selectedApp = nullptr;
    for (const auto& app : Appointment::allAppointments) {
        if (app.getId() == appId && app.getStatus() == "Completed") {
            selectedApp = &app;
            break;
        }
    }

    if (!selectedApp || appointmentHasBill(appId)) {
        std::cout << "Invalid or already billed / not completed appointment.\n";
        return;
    }

    double consFee, medCost = 0.0, testCost = 0.0;
    std::cout << "Enter Consultation Fee: "; std::cin >> consFee;
    std::cout << "Enter Medicine Cost (0 if none): "; std::cin >> medCost;
    std::cout << "Enter Test/Lab Cost (0 if none): "; std::cin >> testCost;

    std::string today = "2025-02-25"; // ← in real app: get current date

    allBills.emplace_back(nextBillId++, appId, selectedApp->getPatientId(),
                          consFee, medCost, testCost, today, "Pending");
    std::cout << "✅ Bill generated successfully! Bill ID = " << (nextBillId-1) << "\n";
    saveToFile();
}

void Bill::viewAll() {
    if (allBills.empty()) {
        std::cout << "No bills found.\n";
        return;
    }
    std::cout << "\n=== All Bills ===\n";
    std::cout << std::left
              << std::setw(10) << "Bill ID"
              << std::setw(14) << "App ID"
              << std::setw(10) << "Pat ID" << std::setw(20) << "Patient"
              << std::setw(12) << "Consult"
              << std::setw(12) << "Medicine"
              << std::setw(12) << "Tests"
              << std::setw(12) << "Total"
              << "Date       Status\n";
    std::cout << std::string(120, '-') << "\n";

    for (const auto& b : allBills) {
        b.display();
    }
}

void Bill::viewByPatient() {
    int pid;
    std::cout << "Enter Patient ID: ";
    std::cin >> pid;

    bool found = false;
    std::cout << "\nBills for Patient ID " << pid << ":\n";
    for (const auto& b : allBills) {
        if (b.patientId == pid) {
            b.display();
            found = true;
        }
    }
    if (!found) std::cout << "No bills found for this patient.\n";
}

void Bill::viewUnpaid() {
    bool found = false;
    std::cout << "\n=== Unpaid Bills ===\n";
    for (const auto& b : allBills) {
        if (b.status == "Pending") {
            b.display();
            found = true;
        }
    }
    if (!found) std::cout << "No unpaid bills.\n";
}

void Bill::payBill() {
    int bid;
    std::cout << "Enter Bill ID to mark as Paid: ";
    std::cin >> bid;

    for (auto& b : allBills) {
        if (b.billId == bid && b.status == "Pending") {
            b.status = "Paid";
            std::cout << "Bill " << bid << " marked as Paid.\n";
            saveToFile();
            return;
        }
    }
    std::cout << "Bill not found or already paid.\n";
}

void Bill::saveToFile() {
    std::ofstream file("../data/bills.txt");
    if (!file.is_open()) {
        std::cout << "Warning: Could not save bills.\n";
        return;
    }
    for (const auto& b : allBills) {
        file << b.billId << "|" << b.appointmentId << "|" << b.patientId << "|"
             << b.consultationFee << "|" << b.medicineCost << "|" << b.testCost << "|"
             << b.dateIssued << "|" << b.status << "\n";
    }
}

void Bill::loadFromFile() {
    allBills.clear();
    std::ifstream file("../data/bills.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string token;
        int bid, aid, pid;
        double cf, mc, tc;
        std::string date, stat;

        std::getline(iss, token, '|'); bid = std::stoi(token);
        std::getline(iss, token, '|'); aid = std::stoi(token);
        std::getline(iss, token, '|'); pid = std::stoi(token);
        std::getline(iss, token, '|'); cf = std::stod(token);
        std::getline(iss, token, '|'); mc = std::stod(token);
        std::getline(iss, token, '|'); tc = std::stod(token);
        std::getline(iss, date, '|');
        std::getline(iss, stat);

        allBills.emplace_back(bid, aid, pid, cf, mc, tc, date, stat);
        nextBillId = std::max(nextBillId, bid + 1);
    }
}
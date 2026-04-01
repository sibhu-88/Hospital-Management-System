#ifndef BILLING_H
#define BILLING_H

#include <string>
#include <vector>
#include "Appointment.h"

class Bill {
private:
    int billId;                 // unique bill ID
    int appointmentId;          // links to an Appointment
    int patientId;
    double consultationFee;
    double medicineCost;
    double testCost;
    double totalAmount;
    std::string dateIssued;     // YYYY-MM-DD
    std::string status;         // "Pending", "Paid"

public:
    Bill();
    Bill(int billId, int appId, int patId, double consFee, double medCost, double testCost,
         std::string date, std::string status = "Pending");

    void display() const;
    int getBillId() const;
    int getAppointmentId() const;
    double getTotal() const;
    std::string getStatus() const;
    void markAsPaid();

    static void generateForAppointment();
    static void viewAll();
    static void viewByPatient();
    static void viewUnpaid();
    static void payBill();
    static void saveToFile();
    static void loadFromFile();

    static std::vector<Bill> allBills;
    static int nextBillId;      // auto-increment

private:
    static bool appointmentHasBill(int appId);
};

#endif
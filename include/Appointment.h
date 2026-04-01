#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>
#include <vector>
#include "Patient.h"
#include "Doctor.h"

class Appointment {
private:
    int id;                     // unique appointment ID
    int patientId;
    int doctorId;
    std::string date;           // format: YYYY-MM-DD
    std::string timeSlot;       // format: HH:MM (e.g. 09:30, 14:00)
    std::string status;         // "Scheduled", "Completed", "Cancelled"

public:
    Appointment();
    Appointment(int id, int patientId, int doctorId, 
                std::string date, std::string timeSlot, std::string status = "Scheduled");

    void display() const;
    int getId() const;
    int getPatientId() const;
    int getDoctorId() const;
    std::string getDate() const;
    std::string getTimeSlot() const;
    std::string getStatus() const;

    static void bookNew();
    static void viewAll();
    static void viewByPatient();
    static void viewByDoctor();
    static void cancelAppointment();
    static void markCompleted();
    static void saveToFile();
    static void loadFromFile();

    static std::vector<Appointment> allAppointments;
    static int nextId;  // auto-increment ID

private:
    static bool isDoctorAvailableOnSlot(int doctorId, const std::string& date, const std::string& timeSlot);
};

#endif
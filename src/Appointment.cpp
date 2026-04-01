#include "../include/Appointment.h"
#include "../include/Patient.h"
#include "../include/Doctor.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>

std::vector<Appointment> Appointment::allAppointments;
int Appointment::nextId = 1001; // starting ID

Appointment::Appointment() : id(0), patientId(0), doctorId(0) {}

Appointment::Appointment(int i, int pid, int did, std::string d, std::string t, std::string s)
    : id(i), patientId(pid), doctorId(did), date(std::move(d)), timeSlot(std::move(t)), status(std::move(s)) {}

int Appointment::getId() const { return id; }
int Appointment::getPatientId() const { return patientId; }
int Appointment::getDoctorId() const { return doctorId; }
std::string Appointment::getDate() const { return date; }
std::string Appointment::getTimeSlot() const { return timeSlot; }
std::string Appointment::getStatus() const { return status; }

void Appointment::display() const
{
    std::string patName = "Unknown", docName = "Unknown", docSpec = "";

    for (const auto &p : Patient::allPatients)
    {
        if (p.getId() == patientId)
        {
            patName = p.getName();
            break;
        }
    }
    for (const auto &d : Doctor::allDoctors)
    {
        if (d.getId() == doctorId)
        {
            docName = d.getName();
            docSpec = d.getSpecialization();
            break;
        }
    }

    std::cout << std::left << std::setw(8) << id
              << std::setw(10) << patientId << std::setw(20) << patName
              << std::setw(10) << doctorId << std::setw(20) << docName
              << std::setw(12) << date
              << std::setw(10) << timeSlot
              << status << "\n";
}

bool Appointment::isDoctorAvailableOnSlot(int docId, const std::string &d, const std::string &t)
{
    for (const auto &app : allAppointments)
    {
        if (app.doctorId == docId && app.date == d && app.timeSlot == t &&
            app.status == "Scheduled")
        {
            return false; // already booked
        }
    }
    return true;
}

void Appointment::bookNew()
{
    if (Patient::allPatients.empty() || Doctor::allDoctors.empty())
    {
        std::cout << "Cannot book: No patients or doctors registered yet.\n";
        return;
    }

    int appId = nextId++;
    int patId, docId;
    std::string date, time;

    std::cout << "\n=== Book New Appointment ===\n";

    Patient::viewAll();
    std::cout << "\nEnter Patient ID: ";
    std::cin >> patId;
    bool patExists = std::any_of(Patient::allPatients.begin(), Patient::allPatients.end(),
                                 [patId](const Patient &p)
                                 { return p.getId() == patId; });
    if (!patExists)
    {
        std::cout << "Invalid Patient ID!\n";
        nextId--;
        return;
    }

    std::cout << "\nAvailable Doctors:\n";
    for (const auto &d : Doctor::allDoctors)
    {
        if (d.isAvailable())
            std::cout << d.getId() << " - " << d.getName() << " (" << d.getSpecialization() << ")\n";
    }
    std::cout << "Enter Doctor ID: ";
    std::cin >> docId;

    bool docExists = false;
    for (const auto &d : Doctor::allDoctors)
    {
        if (d.getId() == docId && d.isAvailable())
        {
            docExists = true;
            break;
        }
    }
    if (!docExists)
    {
        std::cout << "Invalid or unavailable Doctor!\n";
        nextId--;
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter Date (YYYY-MM-DD): ";
    std::getline(std::cin, date);
    std::cout << "Enter Time (HH:MM)     : ";
    std::getline(std::cin, time);

    if (!isDoctorAvailableOnSlot(docId, date, time))
    {
        std::cout << "❌ This slot is already booked for the selected doctor.\n";
        nextId--;
        return;
    }

    allAppointments.emplace_back(appId, patId, docId, date, time, "Scheduled");
    std::cout << "✅ Appointment booked successfully! ID = " << appId << "\n";
    saveToFile();
}

void Appointment::viewAll()
{
    if (allAppointments.empty())
    {
        std::cout << "No appointments found.\n";
        return;
    }
    std::cout << "\n=== All Appointments ===\n";
    std::cout << std::left
              << std::setw(8) << "App ID"
              << std::setw(10) << "Pat ID" << std::setw(20) << "Patient"
              << std::setw(10) << "Doc ID" << std::setw(20) << "Doctor"
              << std::setw(12) << "Date"
              << std::setw(10) << "Time"
              << "Status\n";
    std::cout << std::string(100, '-') << "\n";

    for (const auto &a : allAppointments)
    {
        a.display();
    }
}

void Appointment::viewByPatient()
{
    int pid;
    std::cout << "Enter Patient ID: ";
    std::cin >> pid;

    bool found = false;
    std::cout << "\nAppointments for Patient ID " << pid << ":\n";
    for (const auto &a : allAppointments)
    {
        if (a.patientId == pid)
        {
            a.display();
            found = true;
        }
    }
    if (!found)
        std::cout << "No appointments found.\n";
}

void Appointment::viewByDoctor()
{
    int did;
    std::cout << "Enter Doctor ID: ";
    std::cin >> did;

    bool found = false;
    std::cout << "\nAppointments for Doctor ID " << did << ":\n";
    for (const auto &a : allAppointments)
    {
        if (a.doctorId == did)
        {
            a.display();
            found = true;
        }
    }
    if (!found)
        std::cout << "No appointments found.\n";
}

void Appointment::cancelAppointment()
{
    int aid;
    std::cout << "Enter Appointment ID to cancel: ";
    std::cin >> aid;

    for (auto &a : allAppointments)
    {
        if (a.id == aid && a.status == "Scheduled")
        {
            a.status = "Cancelled";
            std::cout << "Appointment " << aid << " cancelled.\n";
            saveToFile();
            return;
        }
    }
    std::cout << "Appointment not found or already cancelled/completed.\n";
}

void Appointment::markCompleted()
{
    int aid;
    std::cout << "Enter Appointment ID to mark as completed: ";
    std::cin >> aid;

    for (auto &a : allAppointments)
    {
        if (a.id == aid && a.status == "Scheduled")
        {
            a.status = "Completed";
            std::cout << "Appointment " << aid << " marked as completed.\n";
            saveToFile();
            return;
        }
    }
    std::cout << "Appointment not found or not in Scheduled state.\n";
}

void Appointment::saveToFile()
{
    std::ofstream file("../data/appointments.txt");
    if (!file.is_open())
    {
        std::cout << "Warning: Could not save appointments.\n";
        return;
    }
    for (const auto &a : allAppointments)
    {
        file << a.id << "|" << a.patientId << "|" << a.doctorId << "|"
             << a.date << "|" << a.timeSlot << "|" << a.status << "\n";
    }
}

void Appointment::loadFromFile()
{
    allAppointments.clear();
    std::ifstream file("../data/appointments.txt");
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        std::string token;
        int id, pid, did;
        std::string date, time, status;

        std::getline(iss, token, '|');
        id = std::stoi(token);
        std::getline(iss, token, '|');
        pid = std::stoi(token);
        std::getline(iss, token, '|');
        did = std::stoi(token);
        std::getline(iss, date, '|');
        std::getline(iss, time, '|');
        std::getline(iss, status);

        allAppointments.emplace_back(id, pid, did, date, time, status);
        nextId = std::max(nextId, id + 1);
    }
}
#!/bin/bash
# Root files
touch CMakeLists.txt README.md .gitignore

# Folders
mkdir src include data docs tests build

# Source files
touch src/main.cpp
touch src/Patient.cpp src/Doctor.cpp src/Appointment.cpp
touch src/Billing.cpp src/Inventory.cpp src/Utils.cpp

# Headers
touch include/Patient.h include/Doctor.h include/Appointment.h
touch include/Billing.h include/Inventory.h include/Utils.h

# Data files
touch data/patients.txt data/doctors.txt data/appointments.txt
touch data/bills.txt data/inventory.txt

# Documentation
touch docs/project_requirements.md docs/design_document.md

# Basic test file (optional)
touch tests/test_patient.cpp
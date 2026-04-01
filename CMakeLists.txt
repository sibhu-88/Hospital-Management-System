cmake_minimum_required(VERSION 3.15)
project(HospitalManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

include_directories(include)

add_executable(hospital
    src/main.cpp
    src/Patient.cpp
    src/Doctor.cpp
    src/Appointment.cpp
    src/Billing.cpp
    src/Inventory.cpp
    src/Utils.cpp
)

# Optional: enable warnings
if (MSVC)
    target_compile_options(hospital PRIVATE /W4)
else()
    target_compile_options(hospital PRIVATE -Wall -Wextra -Wpedantic)
endif()
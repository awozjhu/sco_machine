/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    Application Class: Orchestrates setup and main command loop for the SCO.
*/

#pragma once        //   #ifndef/#define
#include "sco.h"

// Application Class: initializes components and manages the main loop
class Application {
// member variables  
    ProductDatabase db;  // stores all known products
    UserDisplay display; // manages terminal output
    SCO machine;         // processes input commands and sco functions
public:
// constructor
    Application(); 
// declare public methods
    void run();          // starts program main loop
};

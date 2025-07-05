/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of Application class
*/

#include "sco_application.h"
#include <iostream>

// constructor definition 
Application::Application()
    : db(), display(), machine(db, display) {} // initialization list (constructs members)

// run method definition 
void Application::run() {
    display.showWelcome();                // display welcome message
    std::string line;                     // string for user input
    while (true) {                        // main program loop
        display.prompt();                 // display prompt character
        std::getline(std::cin, line);     // get user input and store in string
        if (!line.empty()) {              // only process command if string is not empty
            machine.processCommand(line);
        }
    }
}

/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of Payment class
*/

#include <cmath>
#include "payment.h"

bool Payment::processCash(double amountDue, double cashGiven, double& change) {
    if (cashGiven < amountDue) return false;
    // Round down to nearest cent
    change = std::floor((cashGiven - amountDue) * 100.0) / 100.0; // ensures only 2 decimal places
    return true;
}

std::string Payment::processCard(double amountDue) {
    std::ostringstream oss;                  // create output string stream named oss
    oss << "APPROVED-" << rand() % 1000000;  // generate a random approval code between (0-999,999)
    return oss.str();
}


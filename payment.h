/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    Payment: Provides static methods for processing payments.
*/

#pragma once
#include <string>
#include <sstream>

class Payment {
public:
// NOTE: make payment memeber functions static bc they don't depend on any instance specific data
// i.e. they belong only to the Payment class
    static bool processCash(double amountDue, double cashGiven, double& change);
    static std::string processCard(double amountDue);   
};



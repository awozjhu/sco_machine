/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    SCO Class: Manages the logic of scanning, removign, and paying for items.
*/

#pragma once
#include <map>

class ChangeRepository {
// member variables 
    // double balance;
    std::map<int, int> denominations; // denomination in cents -> quantity
    const int lowThreshold = 5000;    // $50 in cents, lower cash threshold can't be changed
public:
// constructor 
    ChangeRepository(double initial = 200.0);
// member functions
    bool canDispense(double amount) const; 
    bool dispense(double amount);
    void replenish();
    // bool isLow() const;
};

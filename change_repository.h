/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    SCO Class: Manages the logic of scanning, removign, and paying for items.
*/

#pragma once
#include "user_display.h"
#include <chrono>
#include <map>

class ChangeRepository {
// member variables 
    std::map<int, int> denominations;        // current available denominations (in cents)
    std::map<int, int> originalQuantities;   // initial/target values for each denomination
    const int lowThreshold = 5000;           // $50 in cents, lower cash threshold can't be changed
    std::chrono::system_clock::time_point lastReplenishTime; // to track if the repo has been replenished more than once in 24 hours
public:
// constructor 
    // ChangeRepository(double initial = 200.0);
    ChangeRepository();
// member functions
    void acceptInsertedCash(const std::map<int, int>& inserted, int amountToKeepCents);
    bool canDispense(int amountCents) const;
    std::map<int, int> dispenseChange(int amountCents);
    void addCash(int denomination, int count);
    int  getTotalBalance() const;
    bool isLow() const; // void isLow();
    bool canReplenish() const; // const ensures this function doesn't modify the object   
    bool replenish(UserDisplay& display); 
     
};

/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of ChangeRepository class
*/

#include "change_repository.h"
#include <algorithm>
#include <cmath>


// constructor 
// TODO for initial test
ChangeRepository::ChangeRepository(double initial)
    : balance(std::round(initial * 100.0) / 100.0) {}

// start out with $200
// ChangeRepository() {
//     denominations = { 
//         {10000, 0},  // $100 × 0
//         {5000,  1},  // $50
//         {2000,  2},  // $40
//         {1000,  1},  // $10
//         {500,   2},  // $10
//         {100,   20}, // $20
//         {25,    40}, // $10
//         {10,    50}, // $5
//         {5,     50}, // $2.50
//         {1,     100} // $1
//     };
// }


bool ChangeRepository::canDispense(double amount) const {
    return balance >= amount;    
}

bool ChangeRepository::dispense(double amount) {
    if (amount > balance) return false;
    balance -= amount;
    balance = std::round(balance * 100.0) / 100.0; // enforce 2-decimal precision
    return balance >= lowThreshold;
}

void ChangeRepository::replenish() {
    balance = 200.0;                               // replenish cash reserves 
}

// bool ChangeRepository::isLow() const {
//     return balance < lowThreshold;
// }

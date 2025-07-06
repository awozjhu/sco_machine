/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of ChangeRepository class
*/

#include "change_repository.h"
#include <algorithm>
// #include <cmath>
#include <chrono>

// constructor 
// TODO for initial test
// ChangeRepository::ChangeRepository(double initial)
//     : balance(std::round(initial * 100.0) / 100.0) {}

// start out with $200
ChangeRepository::ChangeRepository() {
    originalQuantities = {
        {10000, 0},   // $100
        {5000,  2},   // $50
        {2000,  2},   // $20
        {1000,  4},   // $10
        {500,   2},   // $5
        {100,   6},   // $1
        {25,    6},   // 25 cent
        {10,   10},   // 10 cent
        {5,    10},   // 5 cent
        {1,   100}    // 1 cent
    };
    denominations = originalQuantities; // start with full stock
    lastReplenishTime = std::chrono::system_clock::now() - std::chrono::hours(24); // get startup timestamp
}

// TODO
// records only the part of the inserted cash that was needed to pay the bill
void ChangeRepository::acceptInsertedCash(const std::map<int, int>& inserted, int amountDueCents) {
    int remaining = amountDueCents;                // start remaining with the balance

    for (auto it = inserted.rbegin(); it != inserted.rend(); ++it) { // iterate from highest denom to lowest
        int denom = it->first;                     // get denomination value in cents
        int qty = it->second;                      // get quantity of the denomination
        int used = 0;                              // stores how many of current demonination to keep

        // use as many of active denomination as needed to cover remaining balance
        while (qty > 0 && remaining >= denom) {
            remaining -= denom;                    // subtract denom value from remaining balance 
            qty--;                                 // decrement denom available quantity
            used++;                                // increment denom quantity used
        }

        denominations[denom] += used;              // add the accepted quantity to change repo 
    }

    // if remaining > 0, store underpayment silently
}

bool ChangeRepository::canDispense(int amountCents) const {
    int remaining = amountCents;                  // start with change amount needed
    auto copy = denominations;                    // make copy so not to moidfy real denom stock

    // for this check dispense from largest to smallest denom
    for (auto it = copy.rbegin(); it != copy.rend(); ++it) {
        while (remaining >= it->first && it->second > 0) {
            remaining -= it->first;               // use active denom to cover part of remaining change balance
            it->second--;                         // decrement denom available quantity 
        }
    }
    return remaining == 0;                        // return true if exact change can be made
}

// Update signature to return map of dispensed denominations
std::map<int, int> ChangeRepository::dispenseChange(int amountCents) {
    int remaining = amountCents;  
    std::map<int, int> dispensed;                 // track which denom/qtys we actually dispensed

    // iterate over denominations from largest to smallest  
    for (auto i = denominations.rbegin(); i != denominations.rend(); ++i) {
        int denom = i->first;                     // get denomination value in cents
        int& qty = i->second;                     // get quantity of the denomination
                 
        // Use as many of active denomination as possible without over-dispensing
        while (remaining >= denom && qty > 0) {
            remaining -= denom;                   // subtract denom value from remaining balance                  
            qty--;                                // decrement denom available quantity
            dispensed[denom]++;                   // increment 1 unit of active denom dispensed 
        }
    }

    if (remaining == 0) {
        return dispensed;                         // success, all required change was dispensed 
    } else {
        // failure, rollback (restore denominations)
        for (const auto& [d, count] : dispensed) {
            denominations[d] += count; 
        }
        return {};                                // return empty map to show failure
    }
}

void ChangeRepository::addCash(int denomination, int count) {
    denominations[denomination] += count;
}

int ChangeRepository::getTotalBalance() const {
    int total = 0;
    for (const auto& [denom, qty] : denominations) {
        total += denom * qty;
   }
    return total;
}

bool ChangeRepository::isLow() const {
    return getTotalBalance() < lowThreshold;
}

// TODO update 
bool ChangeRepository::canReplenish() const {
    auto now = std::chrono::system_clock::now();
    auto durationSinceLast = std::chrono::duration_cast<std::chrono::hours>(now - lastReplenishTime);
    return durationSinceLast.count() >= 24;
}

bool ChangeRepository::replenish(UserDisplay& display) {
    if (!canReplenish()) return false;

    std::map<int, int> replenished;
           // auto$ reference allows to modify denominations map directly 
    for (auto& [denom, currentQty] : denominations) { // iterate over current change repo map
        int targetQty = originalQuantities[denom];    // default denom quantities 
        if (currentQty < targetQty) {                 // check if current quantity is below baseline quantity 
            int added = targetQty - currentQty;       // determine how many units needed to restock 
            currentQty = targetQty;                   // sets stock back to default value 
            replenished[denom] = added;               // save number of units added to restock
        }
    }

    if (replenished.empty()) {
        display.showMessage("All denominations are already at full levels. Nothing to replenish.");
        return false; // Don't update lastReplenishTime
    }

    display.showReplenishmentReport(replenished);
    lastReplenishTime = std::chrono::system_clock::now(); // save when stock was replenished 
    return true;
}


// bool ChangeRepository::isLow() const {
//     return balance < lowThreshold;
// }

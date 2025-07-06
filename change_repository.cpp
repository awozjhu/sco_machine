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
        {25,    6},   // 25¢
        {10,   10},   // 10¢
        {5,    10},   // 5¢
        {1,   100}    // 1¢
    };
    denominations = originalQuantities; // start with full stock
    lastReplenishTime = std::chrono::system_clock::now() - std::chrono::hours(24); // get startup timestamp
}

// TODO
void ChangeRepository::acceptInsertedCash(const std::map<int, int>& inserted, int amountDueCents) {
    int remaining = amountDueCents;

    for (auto it = inserted.rbegin(); it != inserted.rend(); ++it) { // start from highest denom
        int denom = it->first;
        int qty = it->second;
        int used = 0;

        while (qty > 0 && remaining >= denom) {
            remaining -= denom;
            qty--;
            used++;
        }

        denominations[denom] += used;
    }

    // if remaining > 0, store underpayment silently (optional warning)
}


// TODO
bool ChangeRepository::canDispense(int amountCents) const {
    int remaining = amountCents;
    auto copy = denominations;

    for (auto it = copy.rbegin(); it != copy.rend(); ++it) {
        while (remaining >= it->first && it->second > 0) {
            remaining -= it->first;
            it->second--;
        }
    }
    return remaining == 0;
}

// bool ChangeRepository::dispenseChange(int amountCents) { // determines if change amount can be dispensed and if so dispenses change
//     int remaining = amountCents;
//     for (auto i = denominations.rbegin(); i != denominations.rend(); ++i) { // iterate over $ denominations in reverse order (largest bills first)
//         while (remaining >= i->first && i->second > 0) {                    // check if current denomination is >= to remaining change balance
//             remaining -= i->first;                                          // subtract value of denomination from remaining change balance
//             i->second--;                                                    // remove quanity 1 of the current denomintion
//         }
//     }
//     return remaining == 0;
// }

// TODO
// bool ChangeRepository::dispenseChange(int amountCents) {
//     int remaining = amountCents;
//     auto temp = denominations; // simulate first

//     for (auto i = temp.rbegin(); i != temp.rend(); ++i) {
//         while (remaining >= i->first && i->second > 0) {
//             remaining -= i->first;
//             i->second--;
//         }
//     }

//     if (remaining == 0) {
//         denominations = temp; // commit only if successful
//         return true;
//     } else {
//         return false;
//     }
// }

// Update signature to return map of dispensed denominations
std::map<int, int> ChangeRepository::dispenseChange(int amountCents) {
    int remaining = amountCents;
    std::map<int, int> dispensed;

    for (auto i = denominations.rbegin(); i != denominations.rend(); ++i) {
        int denom = i->first;
        int& qty = i->second;

        while (remaining >= denom && qty > 0) {
            remaining -= denom;
            qty--;
            dispensed[denom]++;
        }
    }

    if (remaining == 0) {
        return dispensed; // success
    } else {
        // rollback (restore denominations)
        for (const auto& [d, count] : dispensed) {
            denominations[d] += count;
        }
        return {}; // failure: empty map
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

    for (auto& [denom, currentQty] : denominations) {
        int targetQty = originalQuantities[denom];
        if (currentQty < targetQty) {
            int added = targetQty - currentQty;
            currentQty = targetQty;
            replenished[denom] = added;
        }
    }

    if (replenished.empty()) {
        display.showMessage("All denominations are already at full levels. Nothing to replenish.");
        return false; // Don't update lastReplenishTime
    }

    display.showReplenishmentReport(replenished);
    lastReplenishTime = std::chrono::system_clock::now();
    return true;
}


// bool ChangeRepository::isLow() const {
//     return balance < lowThreshold;
// }

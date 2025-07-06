/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of UserDisplay class
*/

#include "user_display.h"
#include <iostream>
#include <iomanip> 
#include <chrono>
#include <thread>

// constructor 
UserDisplay::UserDisplay() {
    std::cout << std::fixed << std::setprecision(2); // configure UserDisplay to 2 decimal places fixed point (bc money)
}

void UserDisplay::prompt() {
    std::cout << "> ";
}

void UserDisplay::cashprompt() {
    std::cout << ">>enter cash>> ";
}

void UserDisplay::showScanned(const Product& p, int qty) {  // TODO
    std::cout << "Scanned: " << p.getDescription()
              << " x" << qty << " @ $" << p.getPrice() << "\n";
}

void UserDisplay::showSubtotal(double subtotal) {
    std::cout << "Subtotal: $" << subtotal << "\n";
}

void UserDisplay::showTax(double tax) {
    std::cout << "Tax: $" << tax << "\n";
}

void UserDisplay::showTotal(double total) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total: $" << total << "\n";
}

void UserDisplay::showChange(double change) {
    std::cout << "Change returned: $" << change << "\n";
}

void UserDisplay::showApprovalCode(const std::string& code) {
    std::cout << "Card approved with code: " << code << "\n";
}

void UserDisplay::showMessage(const std::string& msg) {
    std::cout << msg << "\n";
}

void UserDisplay::showWelcome() {
    std::cout << "*************************************\n";
    std::cout << "Welcome to the Self-Checkout Machine!\n";
    std::cout << "*************************************\n";
    showHelp();
}

void UserDisplay::showHelp() {
    std::cout << "\nCommands:\n"
              << "  scan <productID> <quantity>\n"
              << "  remove <itemNumber>\n"
              << "  pay cash\n"
              << "  pay card\n"
              << "  list\n"
              << "  cancel\n"
              << "  help\n"
              << "  exit\n\n"
              << "  *** ADMIN ONLY ***\n"
              << "  show change\n"
              << "  replenish\n\n";
}

void UserDisplay::showReplenishmentReport(const std::map<int, int>& replenished) {
    std::cout << "[Replenishment Report]\n";
    for (const auto& [denom, added] : replenished) {
        if (added > 0) {
            std::cout << "  $" << denom / 100.0 << " -> added " << added << " units\n";
        }
    }
}

void UserDisplay::showCardProcessingAnimation() {
    std::cout << "Processing";
    for (int i = 0; i < 5; ++i) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << std::endl;
}

void UserDisplay::showChangeBreakdown(const std::map<int, int>& dispensed) {
    std::ostringstream oss;
    oss << "Change breakdown:\n";
    for (auto it = dispensed.rbegin(); it != dispensed.rend(); ++it) {
        double dollars = it->first / 100.0;
        oss << "  $" << std::fixed << std::setprecision(2) << dollars
            << " x " << it->second << "\n";
    }
    showMessage(oss.str());
}

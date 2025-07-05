/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of UserDisplay class
*/

#include "user_display.h"
#include <iostream>
#include <iomanip> 

// constructor 
UserDisplay::UserDisplay() {
    std::cout << std::fixed << std::setprecision(2); // configure UserDisplay to 2 decimal places fixed point (bc money)
}

void UserDisplay::prompt() {
    std::cout << "> ";
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
    std::cout << "Welcome to the Self-Checkout Machine!\n";
    showHelp();
}

void UserDisplay::showHelp() {
    std::cout << "\nCommands:\n"
              << "  scan <productID> <quantity>\n"
              << "  remove <itemNumber>\n"
              << "  pay cash <amount>\n"
              << "  pay card\n"
              << "  list\n"
              << "  help\n"
              << "  exit\n\n"
              << "  ADMIN ONLY:\n"
              << "  replenish\n\n";
}

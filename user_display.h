/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    UserDisplay: Handles display output to the terminal.
*/

#pragma once
#include "product.h"
#include <string>
#include <map>

class UserDisplay {
public:
    // default constructor (only needed to to set precision cleanly)
    UserDisplay();
    // member functions
    void showWelcome();
    void showHelp();
    void prompt();
    void cashprompt();
    void showScanned(const Product& p, int qty);
    void showSubtotal(double subtotal);
    void showTax(double tax);
    void showTotal(double total);   
    void showChange(double change);
    void showApprovalCode(const std::string& code);
    void showMessage(const std::string& msg);
    void showReplenishmentReport(const std::map<int, int>& replenished);
    void showCardProcessingAnimation();
    void showChangeBreakdown(const std::map<int, int>& dispensed);
};

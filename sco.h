/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    SCO Class: Manages the logic of scanning, removing, and paying for items.
*/

#pragma once
#include "product_database.h"
#include "cart.h"
#include "change_repository.h"
#include "payment.h"
#include "user_display.h"
#include <string>

// Self-Checkout (SCO) Class: processes sco user inputs (scanning, paying, removing, etc.)
class SCO {
// member variables
    ProductDatabase& db;         // reference to ProductDatabase objecdt (avoids unnessesary deep copy)
    UserDisplay& display;        // declares reference to a UserDisplay object (alias to existing object)
    Cart cart;                   // instantiate (create) an empty cart (i.e. call Cart constructor)
    ChangeRepository changeRepo; // instantiate a change repository
    const double taxRate = 0.05; // const bc tax rate can't change during runtime
    bool inService = true;       // indicates if the SCO is operational
// private methods (helper functions)
    void generateReceipt(double subtotal, double tax, double total, double paid, double change,
                        const std::string& method = "", const std::string& approvalCode = "");
 // generate receipt and write to file
    void goodbyeReset();
    void checkoutMessages(double subtotal, double tax, double total);
    void notifyControlCenter();
public:
// constructor 
    SCO(ProductDatabase& pdb, UserDisplay& disp);
// declare public methods
    void processCommand(const std::string& input);
};

/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of CartItem and Cart classes
*/

#include "cart.h"
#include <iomanip>   // for output formatting
#include <algorithm> // to use std::remove_if

////////// CartItem Class 
// constructor 
CartItem::CartItem(const Product& p, int q, int num)
    : product(p), quantity(q), itemNumber(num) {} // initializer list ex. copies passed in Product object p into product member var

// member functions
void CartItem::print() const {
    std::cout << std::setw(2) << itemNumber << ". "
              << product.getID() << " | "
              << product.getDescription() << " x" << quantity << " @ $"
              << product.getPrice() << " = $"
              << product.getPrice() * quantity << "\n";
}

// getter methods
double CartItem::getTotalPrice() const {
    return product.getPrice() * quantity;
}

int CartItem::getItemNumber() const {
    return itemNumber;
}

std::string CartItem::getDescription() const {
    return product.getDescription(); 
}

////////// Cart Class 
// constructor 
Cart::Cart() : counter(1) {}

// member functions
void Cart::addItem(const Product& p, int quantity) {
    items.emplace_back(p, quantity, counter++); // constructs a new CartItem object and places at the end of the vector
}

void Cart::removeItem(int itemNumber) {
    // items.erase(std::remove_if(items.begin(), items.end(),[itemNumber]
    // remove_if : reorders the vector so that all elements not matching the condition are moved to the front, all that do match go to the end
    auto updtVec = std::remove_if(items.begin(), items.end(), // store iterator that points to unwanted "end" portion of vector 
        [itemNumber](const CartItem& item) {
            return item.getItemNumber() == itemNumber;
        });

    items.erase(updtVec, items.end()); // remove all vector elements after updtVec
}

void Cart::printItems() const {    // const bc method does not modify Cart class
        // auto --> compiler deduce type CartItem, & bc accessing each CartItem by reference to avoid copying 
    for (const auto& item : items) // loop over each item inside items vector 
        item.print();
}

// generates contents written to receipt, note: pass an output stream as a parameter, "const" can't modify Cart object 
void Cart::printReceipt(std::ostream& os, double total, double paid, double change) const {   
    os << "\n--- RECEIPT ---\n";
    // iterate over each CartItem in member variable (vector) items
    for (const auto& item : items)                // doesn't modify each item, reference to avoid copying 
        os << item.getDescription() << " - $" << item.getTotalPrice() << "\n";
    if (total > 0.0) {
        os << "-------------------------\n";
        os << "Total: $" << std::fixed << std::setprecision(2) << total << "\n";   
        os << "Paid:  $" << std::fixed << std::setprecision(2) << paid << "\n";
        os << "Change:$" << std::fixed << std::setprecision(2) << change << "\n";
        os << "-------------------------\n";
        os << "Thank you for shopping!\n";
    }
}

double Cart::getSubtotal() const {
    double total = 0.0;    
    for (const auto& item : items)
        total += item.getTotalPrice();
    return total;
}

bool Cart::isEmpty() const {
    return items.empty();    
}


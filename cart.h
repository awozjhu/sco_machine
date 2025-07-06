/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    CartItem Class: Represents an item in the cart including quantity and item number.
    Cart     Class: Manages a collection of items scanned into the shopping cart.
*/

#pragma once
#include "product.h"
#include <vector>
#include <iostream>

class CartItem {
    // member variables 
    Product product;
    int quantity;
    int itemNumber;
public:
    // constructor 
    CartItem(const Product& p, int q, int num);

    // member functions
    void print() const;
    double getTotalPrice() const;
    int getItemNumber() const;
    std::string getDescription() const;
    void setItemNumber(int num);
};

class Cart {
    // member variables 
    std::vector<CartItem> items;
    int counter;
public:
    // constructor 
    Cart();

    // member functions
    void renumberItems(); // Reassigns item numbers to all CartItems
    void addItem(const Product& p, int quantity);
    void removeItem(int itemNumber);
    void printItems() const;
    void printReceipt(std::ostream& os, double total = 0.0, double paid = 0.0, double change = 0.0) const;
    double getSubtotal() const;
    bool isEmpty() const;
};


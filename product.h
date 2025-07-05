/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    Payment: Provides static methods for processing payments.
*/

#pragma once
#include <string>

class Product {
// member variables
    std::string id;
    std::string description;
    double price;
public:
// default constructor 
    Product();
// parameterized constructor
    Product(std::string pid, std::string desc, double p);
// member functions (getters)
    std::string getID() const;
    std::string getDescription() const;
    double getPrice() const;
};

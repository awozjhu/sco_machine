/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation of Product class
*/

#include "product.h"

Product::Product() : price(0.0) {}

Product::Product(std::string pid, std::string desc, double p)
    : id(pid), description(desc), price(p) {}

std::string Product::getID() const { return id; }
std::string Product::getDescription() const { return description; }
double Product::getPrice() const { return price; }

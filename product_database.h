/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Class Descriptions:
    ProductDatabase: Simulates a database for looking up products by ID.
*/

#pragma once
#include "product.h"
#include <map> // to use std::map
#include <string>

class ProductDatabase {
// member variables 
    std::map<std::string, Product> db;  // Use a map (associative container, like a python dict) to create database
public:
// default constructor 
    ProductDatabase();
// member functions 
    bool contains(const std::string& id) const;  // check if database contains a product 
    Product lookup(const std::string& id) const; // returns Product object if found
};

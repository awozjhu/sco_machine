/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: implementation ProductDatabase class
*/

#include "product_database.h"

ProductDatabase::ProductDatabase() {
    db["Meat01"]        = Product("Meat01", "T-Bone Steak", 7.99);
    db["Meat02"]        = Product("Meat02", "Tyson Fresh Chicken Wings", 10.00);
    db["Icecream01"]    = Product("Icecream01", "Chocolate Ice Cream", 2.50);
    db["Icecream02"]    = Product("Icecream02", "Vanilla Ice Cream", 2.50);
    db["Corn01"]        = Product("Corn01", "Fresh Sweet Corn", 2.00);
    db["Casewater01"]   = Product("Casewater01", "24 Bottles Deer Park Water", 4.99);
    db["Potatochips01"] = Product("Potatochips01", "Plain Potato Chips", 2.00);
    db["Potatochips02"] = Product("Potatochips02", "Green Onion Potato Chips", 2.00);
    db["Donuts01"]      = Product("Donuts01", "Glazed Donuts Dozen", 4.99);
    db["Sausage01"]     = Product("Sausage01", "8-Sausage Pack", 4.99);
    db["Eggs01"]        = Product("Eggs01", "Dozen Eggs", 3.00);
    db["Milk01"]        = Product("Milk01", "Gallon Milk", 4.00);
    db["Apple01"]       = Product("Apple01", "One nice crispy Gala apple", 1.00); 
}

bool ProductDatabase::contains(const std::string& id) const {
    return db.find(id) != db.end();
}

Product ProductDatabase::lookup(const std::string& id) const {
    auto it = db.find(id);
     return (it != db.end()) ? it->second : Product();
}

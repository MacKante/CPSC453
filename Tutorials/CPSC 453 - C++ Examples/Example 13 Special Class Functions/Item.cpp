#include "Item.h"
#include <iostream>

// Default constructor
Item::Item() : name("Unknown"), quantity(0) {}

// Parameterized constructor
Item::Item(const std::string& name_, int quantity_) 
    : name(name_), quantity(quantity_) {}

// Copy constructor
Item::Item(const Item& other) 
    : name(other.name), quantity(other.quantity) {}

// Move constructor
Item::Item(Item&& other) noexcept 
    : name(std::move(other.name)), quantity(other.quantity) {
    other.quantity = 0; // Reset the moved-from object
}

// Copy assignment operator
Item& Item::operator=(const Item& other) {
    if (this == &other) return *this; // Handle self-assignment
    name = other.name;
    quantity = other.quantity;
    return *this;
}

// Move assignment operator
Item& Item::operator=(Item&& other) noexcept {
    if (this == &other) return *this; // Handle self-assignment
    name = std::move(other.name);
    quantity = other.quantity;
    other.quantity = 0; // Reset the moved-from object
    return *this;
}

// Destructor
Item::~Item() {
    // Cleanup if necessary (often to call delete on raw heap allocation)
}

// Member functions
void Item::display() const {
    std::cout << "Item: " << name << ", Quantity: " << quantity << std::endl;
}

const std::string& Item::getName() const {
    return name;
}

int Item::getQuantity() const {
    return quantity;
}

void Item::addQuantity(int amount) {
    quantity += amount;
}

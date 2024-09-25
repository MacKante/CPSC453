#pragma once

#include <string>

class Item {
public:
    // Default constructor
    Item();

    // Parameterized constructor
    Item(const std::string& name, int quantity);

    // Copy constructor
    Item(const Item& other);

    // Move constructor
    Item(Item&& other) noexcept;

    // Copy assignment operator
    Item& operator=(const Item& other);

    // Move assignment operator
    Item& operator=(Item&& other) noexcept;

    // Destructor
    ~Item();

    // Member functions
    void display() const;
    const std::string& getName() const;
    int getQuantity() const;
    void addQuantity(int amount);

private:
    std::string name;
    int quantity;
};

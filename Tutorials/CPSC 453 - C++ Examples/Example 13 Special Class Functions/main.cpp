#include "Item.h"
#include <iostream>

int main() {
    // Default constructor
    Item item1;
    item1.display(); // Outputs: Item: Unknown, Quantity: 0

    // Parameterized constructor
    Item item2("Widget", 10);
    item2.display();

    // Copy constructor
    Item item3(item2);
    item3.display();

    // Move constructor
    Item item4(std::move(item2));
    item4.display();
    item2.display(); // item2's data is now in a moved-from state

    // Copy assignment operator
    Item item5("Gadget", 20);
    item5 = item3;
    item5.display();

    // Move assignment operator
    Item item6("Tool", 30);
    item6 = std::move(item4);
    item6.display();
    item4.display(); // item4's data is now in a moved-from state

    // Modify quantity
    item6.addQuantity(10);
    item6.display(); // Updated quantity

    return 0;
}

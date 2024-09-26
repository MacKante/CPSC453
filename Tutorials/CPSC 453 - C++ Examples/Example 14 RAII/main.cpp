#include "ResourceManager.h"
#include <memory> // For std::unique_ptr
#include <iostream>

int main() {
    ResourceManager rm1; // Uses default constructor
    rm1.setValue(10);
    std::cout << "Value in rm1: " << rm1.getValue() << "\n";

    ResourceManager rm2(20); // Uses parameterized constructor
    std::cout << "Value in rm2: " << rm2.getValue() << "\n";

    ResourceManager rm3 = rm2; // Uses copy constructor
    std::cout << "Value in rm3 (copied from rm2): " << rm3.getValue() << "\n";

    // Uses copy assignment operator
    rm3 = rm1;
    std::cout << "Value in rm3: " << rm3.getValue() << "\n";

    // Uses Move Constructor
    ResourceManager rm4(std::move(rm3));
    std::cout << "Value in rm3 after move: " << rm3.getValue() << "\n";
    std::cout << "Value in rm4: " << rm4.getValue() << "\n";

    //Uses Move assignemnt operator
    rm4 = std::move(rm2);
    std::cout << "Value in rm2 after move: " << rm2.getValue() << "\n";
    std::cout << "Value in rm4: " << rm4.getValue() << "\n";

    return 0;
}

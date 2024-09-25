#include <iostream>

// Function prototypes
void passByValue(int value);
void passByReference(int& ref);
void passByConstReference(const int& cref);

int main() {
    int num = 10;

    std::cout << "Original num: " << num << std::endl;

    // Passing by value
    passByValue(num); // Pass num by value (a copy is made)
    std::cout << "num after passByValue: " << num << std::endl; 
    // num remains unchanged

    // Passing by reference
    passByReference(num); // Pass num by reference (original is modified)
    std::cout << "num after passByReference: " << num << std::endl; 
    // num is modified

    // Passing by const reference
    passByConstReference(num); // Pass num by const reference (read-only access)
    std::cout << "num after passByConstReference: " << num << std::endl; 
    // num remains unchanged

    return 0;
}

// Function definitions

// Pass by value: function receives a copy of the argument
void passByValue(int value) {
    value += 5; // Modifies the local copy
    std::cout << "Inside passByValue: " << value << std::endl; 
    // New value (local copy)
}

// Pass by reference: function receives a reference to the original argument
void passByReference(int& ref) {
    ref += 5; // Modifies the original value
    std::cout << "Inside passByReference: " << ref << std::endl; 
    // Modified value (original)
}

// Pass by const reference: function receives a const reference (read-only access)
void passByConstReference(const int& cref) {
    // cref += 5; // Error: cannot modify a const reference
    std::cout << "Inside passByConstReference: " << cref << std::endl; 
    // Original value (read-only)
}

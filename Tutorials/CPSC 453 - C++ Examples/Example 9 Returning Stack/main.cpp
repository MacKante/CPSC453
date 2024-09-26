#include <iostream>

// Function prototypes
int returnByValue();
int& returnDanglingReference();
int* returnDanglingPointer();

int main() {
    // Demonstrate return by value
    int value = returnByValue(); // This is safe, value is copied
    std::cout << "Returned value: " << value << std::endl;

    // Demonstrate problem with returning a dangling reference
    int& ref = returnDanglingReference(); // This will lead to a dangling reference
    std::cout << "Value through dangling reference: " << ref << std::endl; // Undefined behavior

    // Demonstrate problem with returning a dangling pointer
    int* ptr = returnDanglingPointer(); // This will lead to a dangling pointer
    std::cout << "Value through dangling pointer: " << *ptr << std::endl; // Undefined behavior

    return 0;
}

// Function definitions

// Function that returns by value
// Note: Return Value Optimization (RVO) will help optimize 
// this by avoiding unnecessary copies (very helpful on LARGE data)
int returnByValue() {
    int localValue = 42; // Local variable
    return localValue; // Returns a copy of the local variable (safe)
}

// Function that returns a reference to a local variable 
// (dangling reference after scope)
int& returnDanglingReference() {
    int localValue = 42; // Local variable
    return localValue; 
    // Returns a reference to the local variable (dangling reference)
}

// Function that returns a pointer to a local variable 
// (dangling pointer after scope)
int* returnDanglingPointer() {
    int localValue = 100; // Local variable
    int* localPtr = &localValue; 
    return localPtr; 
    // Returns pointer to the local variable (dangling pointer)
}

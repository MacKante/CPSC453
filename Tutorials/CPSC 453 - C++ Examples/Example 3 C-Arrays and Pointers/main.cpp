#include <iostream>

int main() {
    // C-style array example
    int myArray[5] = {1, 2, 3, 4, 5};

    std::cout << "=== C-style Array Example ===" << std::endl;

    // Accessing array elements using array indexing
    std::cout << "Array elements:" << std::endl;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << "myArray[" << i << "] = " << myArray[i] << std::endl;
    }

    // Modifying array elements
    myArray[2] = 10;  // Modify the third element
    std::cout << "After modification, array elements:" << std::endl;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << "myArray[" << i << "] = " << myArray[i] << std::endl;
    }

    std::cout << std::endl;

    // Pointer example
    int* myPointer = myArray; // Pointer to the first element of the array
    // int* myPointer = &(myArray[0]) //Also works (& grabs the address)

    std::cout << "=== Pointer Example ===" << std::endl;

    // Accessing array elements using the pointer
    std::cout << "Accessing elements via pointer:" << std::endl;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << "*(myPointer + " << i << ") = " << *(myPointer + i) << std::endl;
    }

    // Modifying array elements using the pointer
    *(myPointer + 2) = 10;  // Modify the third element
    std::cout << "After modification, array elements accessed via pointer:" << std::endl;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << "*(myPointer + " << i << ") = " << *(myPointer + i) << std::endl;
    }

    // Pointer arithmetic: incrementing the pointer
    int* incrementedPointer = myPointer + 1;
    std::cout << "Value at incremented pointer position: " << *incrementedPointer << std::endl;

    // Demonstrating nullptr
    int* nullPointer = nullptr; // Initializing a pointer to nullptr
    std::cout << "nullPointer is " << (nullPointer == nullptr ? "null" : "not null") << std::endl;

    // Using nullptr in pointer comparisons
    if (myPointer != nullptr) {
        std::cout << "myPointer is not null" << std::endl;
    } else {
        std::cout << "myPointer is null" << std::endl;
    }

    return 0;
}

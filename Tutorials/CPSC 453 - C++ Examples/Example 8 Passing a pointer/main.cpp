#include <iostream>

// Function prototypes
void passByPointer(int* ptr);
void passByConstPointer(const int* cptr);
void passByConstPointerToConst(const int* const cptr);

// Helper function to print the content pointed to by a pointer
void printPointerValue(const int* ptr) {
    if (ptr) { // Check if the pointer is not null
        std::cout << *ptr << std::endl;
    }
}

int main() {
    int num = 10;
    const int constNum = 20;
    int* numPtr = &num; // Pointer to num

    std::cout << "Original num: " << num << std::endl;

    // Passing by pointer (non-const pointer)
    passByPointer(numPtr); 
    // Pass address of num (original can be modified)
    std::cout << "num after passByPointer: " << num << std::endl; 
    // num is modified

    // Passing by const pointer (read-only access to pointed value)
    passByConstPointer(&num); 
    // Pass address of num (cannot modify num through pointer)
    std::cout << "num after passByConstPointer: " << num << std::endl; 
    // num remains unchanged

    // Passing by const pointer to const 
    // (read-only access to both pointer and pointed value)
    passByConstPointerToConst(&constNum); 
    // Pass address of constNum (cannot modify num or the pointer)
    std::cout << "constNum: " << constNum << std::endl; 
    // constNum remains unchanged

    return 0;
}

// Function definitions

// Pass by non-const pointer: function can modify the original value and the pointer address
void passByPointer(int* ptr) {
    if (ptr) { // Check if the pointer is not null
        *ptr += 5; // Modify the value pointed to by the pointer
        std::cout << "Inside passByPointer: ";
        printPointerValue(ptr); // Print modified value

        // Modify the pointer itself (if you want to point to another address)
        int newVal = 100;
        ptr = &newVal; 
        // Change the pointer to a different address (newVal is local to this function)
        std::cout << "Pointer address changed inside passByPointer. New value at new address: ";
        printPointerValue(ptr); // Print new value (local to the function)
    }
}

// Pass by const pointer: function cannot modify the original value 
// but can modify the pointer itself (in practice, you should not modify const pointers)
void passByConstPointer(const int* cptr) {
    std::cout << "Inside passByConstPointer: ";
    // *cptr += 5; // Error: cannot modify the value pointed to by a const pointer
    printPointerValue(cptr); // Print value (cannot modify)
    
    // Changing the pointer itself (in practice, you should not modify const pointers)
    // const int* newPtr = nullptr; // You can assign a new address to a local pointer
    // std::cout << "Pointer address changed inside passByConstPointer: " << newPtr << std::endl; 
    // newPtr is local and changes nothing outside
}

// Pass by const pointer to const: function cannot modify the pointer or the value
void passByConstPointerToConst(const int* const cptr) {
    std::cout << "Inside passByConstPointerToConst: ";
    // *cptr += 5; // Error: cannot modify the value pointed to by a const pointer
    printPointerValue(cptr); // Print value (cannot modify)

    // cptr itself is also const, so you cannot change the pointer address
    // cptr = nullptr; // Error: cannot modify the const pointer
}

#include <iostream>

// Function prototypes
void demonstrateMemoryLeak();
int* returnPointerWithoutDeallocation();

int main() {
    // === Demonstrate Dynamic Allocation ===
    int* dynamicArray = new int[5]; 
    // Allocate memory for an array of 5 integers

    // Initialize and use the array
    for (size_t i = 0; i < 5; ++i) {
        dynamicArray[i] = static_cast<int>(i * 10);
    }

    std::cout << "Dynamic Array Contents:" << std::endl;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << dynamicArray[i] << " ";
    }
    std::cout << std::endl;

    // Deallocate memory
    delete[] dynamicArray; // Correctly deallocate the memory

    // === Demonstrate Memory Leak ===
    demonstrateMemoryLeak(); // Call function that demonstrates memory leak

    // === Demonstrate Dangling Pointer ===
    int* danglingPtr = new int(100); // Allocate memory and initialize
    delete danglingPtr; // Deallocate memory
    std::cout << "Dangling pointer demonstration: ";
    // Uncommenting the following line will cause undefined behavior
    // std::cout << *danglingPtr << std::endl;

    // === Demonstrate Returning Pointer Without Deallocation ===
    int* returnedPtr = returnPointerWithoutDeallocation(); // Get pointer from function
    // Unlike stack, the heap will persist till we delete it
    std::cout << "Returned pointer value: " << *returnedPtr << std::endl;
    // Note: its hard to know if we are responsable for deallocation,
    // maybe side effects somewhere else is responsible? Who knows!
    // Without following line, there will be a memory leak
    // delete returnedPtr; // Uncommenting this line would fix the issue

    return 0;
}

// Function that demonstrates a memory leak
void demonstrateMemoryLeak() {
    int* memoryLeakPtr = new int(42); // Allocate memory and initialize

    // Intentionally forgetting to deallocate memory
    // delete memoryLeakPtr; // Uncommenting this line would fix the issue

    // This will cause a memory leak as the allocated memory is never freed
    std::cout << "Memory leak demonstration: allocated memory is not deallocated." << std::endl;
}

// Function that returns a pointer but the caller does not deallocate it
int* returnPointerWithoutDeallocation() {
    int* ptr = new int(50); // Allocate memory and initialize
    return ptr; // Return pointer to allocated memory
}

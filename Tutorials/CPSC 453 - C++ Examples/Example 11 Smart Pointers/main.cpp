#include <iostream>
#include <memory> //Defines smart pointers

// Demonstrates the use of std::unique_ptr
void demonstrateUniquePtr() {
    // Create a unique_ptr to manage an integer. It owns the integer and is responsible for its deletion.
    std::unique_ptr<int> uniquePtr = std::make_unique<int>(10);
    std::cout << "uniquePtr value: " << *uniquePtr << std::endl;

    // Transfer ownership from uniquePtr to anotherUniquePtr. 
    // After the transfer, uniquePtr is null and cannot be used.
    std::unique_ptr<int> anotherUniquePtr = std::move(uniquePtr);
    if (!uniquePtr) {
        std::cout << "uniquePtr is now null\n"; // uniquePtr no longer owns the integer
    }
    std::cout << "anotherUniquePtr value: " << *anotherUniquePtr << std::endl;
    // The integer will be automatically deallocated when anotherUniquePtr goes out of scope.
}

// Demonstrates the use of std::shared_ptr
void demonstrateSharedPtr() {
    // Create a shared_ptr to manage an integer. It starts with a reference count of 1.
    std::shared_ptr<int> sharedPtr1 = std::make_shared<int>(20);
    std::cout << "sharedPtr1 value: " << *sharedPtr1 << std::endl;

    // Create another shared_ptr (sharedPtr2) pointing to the same integer.
    // The reference count is incremented to 2.
    std::shared_ptr<int> sharedPtr2 = sharedPtr1;
    std::cout << "sharedPtr2 value: " << *sharedPtr2 << std::endl;
    
    // Print the reference count, which should be 2 as both sharedPtr1 and sharedPtr2 own the integer.
    std::cout << "Use count: " << sharedPtr1.use_count() << std::endl; // Should be 2

    // The integer will be automatically deallocated when the last shared_ptr 
    //(sharedPtr1 or sharedPtr2) goes out of scope.
}

// Demonstrates the use of std::weak_ptr
void demonstrateWeakPtr() {
    // Create a shared_ptr to manage an integer.
    std::shared_ptr<int> sharedPtr = std::make_shared<int>(30);
    std::cout << "sharedPtr value: " << *sharedPtr << std::endl;

    // Create a weak_ptr from the shared_ptr. A weak_ptr does not affect the reference count.
    std::weak_ptr<int> weakPtr = sharedPtr;

    // Convert weak_ptr to shared_ptr to access the object.
    // This step checks if the object is still available (i.e., if the reference count is > 0).
    if (std::shared_ptr<int> lockedPtr = weakPtr.lock()) {
        std::cout << "lockedPtr value: " << *lockedPtr << std::endl;
    } else {
        std::cout << "The object is no longer available\n"; // The object is destroyed if the reference count drops to 0
    }
    // The integer will be automatically deallocated when the last shared_ptr (sharedPtr) goes out of scope.
}

int main() {
    std::cout << "Demonstrating unique_ptr:\n";
    demonstrateUniquePtr();

    std::cout << "\nDemonstrating shared_ptr:\n";
    demonstrateSharedPtr();

    std::cout << "\nDemonstrating weak_ptr:\n";
    demonstrateWeakPtr();

    return 0;
}

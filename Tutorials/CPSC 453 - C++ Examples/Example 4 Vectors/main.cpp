#include <iostream>
#include <vector>

int main() {
    // Initialization of std::vector

    // 1. Default initialization (empty vector)
    std::vector<int> vec1; // An empty vector of integers
    std::cout << "vec1.size() = " << vec1.size() << std::endl; 
    // Size is 0

    // 2. Initialization with a specific size and default values
    std::vector<int> vec2(5); // Vector with 5 elements, 
    // all initialized to 0
    std::cout << "vec2 elements:" << std::endl;
    for (size_t i = 0; i < vec2.size(); ++i) {
        std::cout << "vec2[" << i << "] = " << vec2[i] << std::endl; 
        // All values are 0
    }

    // 3. Initialization with size and a specific value
    std::vector<int> vec3(5, 10); // Vector with 5 elements, 
    // all initialized to 10
    std::cout << "vec3 elements:" << std::endl;
    for (size_t i = 0; i < vec3.size(); ++i) {
        std::cout << "vec3[" << i << "] = " << vec3[i] << std::endl; 
        // All values are 10
    }

    // 4. Initialization with a list of values
    std::vector<int> vec4 = {1, 2, 3, 4, 5}; 
    // Vector with elements initialized from a list
    std::cout << "vec4 elements:" << std::endl;
    for (size_t i = 0; i < vec4.size(); ++i) {
        std::cout << "vec4[" << i << "] = " << vec4[i] << std::endl; 
        // Values are 1, 2, 3, 4, 5
    }

    // Accessing and modifying elements
    vec4[2] = 10; // Modify the third element to 10
    std::cout << "vec4 elements after modification:" << std::endl;
    for (size_t i = 0; i < vec4.size(); ++i) {
        std::cout << "vec4[" << i << "] = " << vec4[i] << std::endl; 
        // Third value is now 10
    }

    // Use iterators to access and print vector elements
    std::cout << "Vector elements using iterators:" << std::endl;
    for (std::vector<int>::iterator it = vec4.begin(); it != vec4.end(); ++it) {
        std::cout << *it << " "; // Dereference iterator to get element value
    }
    std::cout << std::endl;

    // Add new elements to the vector
    vec4.push_back(6); // Add element 6 to the end of the vector
    vec4.push_back(7); // Add element 7 to the end of the vector

    for (size_t i = 0; i < vec4.size(); ++i) {
        std::cout << "vec4[" << i << "] = " << vec4[i] << std::endl; 
        // Print elements after adding new values
    }

    // Remove elements from the end of the vector
    vec4.pop_back(); // Remove the last element (7)
    vec4.pop_back(); // Remove the last element (6)

    for (size_t i = 0; i < vec4.size(); ++i) {
        std::cout << "vec4[" << i << "] = " << vec4[i] << std::endl; 
        // Print elements after removing values
    }

    return 0;
}

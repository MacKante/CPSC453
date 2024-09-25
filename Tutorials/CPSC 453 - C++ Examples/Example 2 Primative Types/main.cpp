#include <iostream>

int main() {
    // Integer types
    int myInt = 42;
    // Signed integer: min = -2^31, max = 2^31 - 1

    unsigned int myUnsignedInt = 100u;
    // Unsigned integer: min = 0, max = 2^32 - 1

    short myShort = 32767;
    // Short integer: min = -32,768, max = 32,767

    long myLong = 1234567890L;
    // Long integer: min = -2^31, max = 2^31 - 1 (or higher, depending on system)

    size_t mySizeT = 500u;
    // Unsigned type for sizes: min = 0, max = platform-dependent
    // Typically 2^32 - 1 on 32-bit, 2^64 - 1 on 64-bit

    // Floating-point types
    float myFloat = 3.14f;
    // Single-precision floating-point: approx min = 1.2E-38, max = 3.4E+38

    double myDouble = 3.1415926535;
    // Double-precision floating-point: approx min = 2.3E-308, max = 1.7E+308

    // Character and boolean types
    char myChar = 'A';
    // Single character: min = -128, max = 127 (ASCII values)

    bool myBool = true;
    // Boolean: min = false (0), max = true (1)

    // Output each value
    std::cout << "Integer: " << myInt << std::endl;
    std::cout << "Unsigned Integer: " << myUnsignedInt << std::endl;
    std::cout << "Short: " << myShort << std::endl;
    std::cout << "Long: " << myLong << std::endl;
    std::cout << "size_t: " << mySizeT << std::endl;
    std::cout << "Float: " << myFloat << std::endl;
    std::cout << "Double: " << myDouble << std::endl;
    std::cout << "Character: " << myChar << std::endl;
    std::cout << "Boolean: " << myBool << std::endl;

    return 0;
}


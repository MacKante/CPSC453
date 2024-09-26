#include <iostream>
#include <cstring>  // For C-string functions like strlen, strcpy, etc.
#include <string>   // For std::string

int main() {
    // C-string example
    std::cout << "=== C-string Example ===" << std::endl;

    char myCString[20] = "Hello";
    // C-strings are arrays of characters, null-terminated ('\0') at the end
    std::cout << "C-string: " << myCString << std::endl;

    // C-string length is measured manually using functions like strlen
    std::cout << "Length of C-string: " << strlen(myCString) << std::endl;

    // Modifying C-strings requires care, using functions like strcpy or strcat
    strcat(myCString, ", world!");  // Concatenate two C-strings
    std::cout << "Modified C-string: " << myCString << std::endl;

    // Compare C-strings (using strcmp)
    if (strcmp(myCString, "Hello, world!") == 0) {
        std::cout << "C-string matches 'Hello, world!' using strcmp()" << std::endl;
    } else {
        std::cout << "C-string does not match 'Hello, world!' using strcmp()" << std::endl;
    }

    std::cout << std::endl;

    // std::string example
    std::cout << "=== std::string Example ===" << std::endl;

    std::string myCppString = "Hello";
    // std::string is a class that handles strings automatically
    std::cout << "std::string: " << myCppString << std::endl;

    // Length is obtained using the length() or size() method
    std::cout << "Length of std::string: " << myCppString.length() << std::endl;

    // Modifying std::string is simple and safer
    myCppString += ", world!";
    std::cout << "Modified std::string: " << myCppString << std::endl;

    // Compare using == operator for std::string (simple equality check)
    if (myCppString == "Hello, world!") {
        std::cout << "std::string matches 'Hello, world!' using ==" << std::endl;
    }

    // More complex .compare() operation (lexicographical comparison)
    std::string anotherString = "Hello, universe!";

    int comparisonResult = myCppString.compare(anotherString);

    if (comparisonResult == 0) {
        std::cout << "myCppString and anotherString are lexicographically equal" << std::endl;
    } else if (comparisonResult < 0) {
        std::cout << "myCppString is lexicographically less than anotherString" << std::endl;
    } else {
        std::cout << "myCppString is lexicographically greater than anotherString" << std::endl;
    }

    // Partial comparison (first 5 characters of both std::string and anotherString)
    if (myCppString.compare(0, 5, anotherString, 0, 5) == 0) {
        std::cout << "First 5 characters of myCppString and anotherString are equal" << std::endl;
    }

    return 0;
}


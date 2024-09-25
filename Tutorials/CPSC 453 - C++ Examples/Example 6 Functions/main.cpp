#include <iostream>

// Function declaration (prototype)
int square(int number);

int main() {
    int num = 5;

    // Function call
    int result = square(num);

    std::cout << "The square of " << num << " is " << result << std::endl;

    return 0;
}

// Function definition
int square(int number) {
    return number * number; // Calculate and return the square of the number
}

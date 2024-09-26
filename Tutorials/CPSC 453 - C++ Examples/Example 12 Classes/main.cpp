#include "Person.h"
#include <memory> // For std::unique_ptr
#include <iostream>

int main() {
    // Stack allocation
    Person person("Alice", 30);
    person.introduce();
    std::cout << person.getName() << " is " << person.getAge()
              << " years old.\n";
    std::cout << person.getName() << " is in their "
              << person.getDecadesOld() << " decades.\n";

    // Age the person by one year
    person.ageOneYear();
    std::cout << "After one year, " << person.getName() << " is "
              << person.getAge() << " years old.\n";
    std::cout << person.getName() << " is now in their "
              << person.getDecadesOld() << " decades.\n";

    // Heap (dynamic) allocation
    std::unique_ptr<Person> person2 = std::make_unique<Person>("Bob", 25);
    person2->introduce();
    std::cout << person2->getName() << " is " << person2->getAge()
              << " years old.\n";
    std::cout << person2->getName() << " is in their "
              << person2->getDecadesOld() << " decades.\n";

    // Age the dynamically allocated person by one year
    person2->ageOneYear();
    std::cout << "After one year, " << person2->getName() << " is "
              << person2->getAge() << " years old.\n";
    std::cout << person2->getName() << " is now in their "
              << person2->getDecadesOld() << " decades.\n";

    return 0;
}

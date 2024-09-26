#pragma once //Guard

#include <string>

// Class declaration
class Person {
public:
    // Constructor
    Person(const std::string& name, int age);

    // Member function
    void introduce() const; //const means promises not to change data
    const std::string& getName() const;
    int getAge() const;
    int getDecadesOld() const;

    void ageOneYear(); //No const because it will change data 
    //(try adding  const and see what happens)
    

private: //If you use inheritence, they wont be seen (change to protected to do so)

    void updateDecadesOld();

    const std::string name; // Member variables are private by default in C++
    int age;
    int decades_old;
};


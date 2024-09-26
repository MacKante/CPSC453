#include "Person.h"
#include <iostream>

// Constructor definition
Person::Person(const std::string& name_, int age_) 
// Initialization list is used here (construct directly right away)
    : name(name_) 
    , age(age_) 
    //, decades_old(age / 10) // Will work 
    //, decades_old(age_ / 10) //(on input or previous initization)
{
    //For objects with default behaviour/constructors, it can be done here
    decades_old = age / 10; // floor of division
    // updateDecadesOld(); we have this defined so we could also do this

    // name = name_
    // const variable will not allow this since it already
    // called the default constructor of std::string
}

// Member function definition
void Person::introduce() const {
    std::cout << "Hi, I'm " << name << " and I'm " << age << " years old.\n";
}

const std::string& Person::getName() const{
    return name;
}
int Person::getAge() const{
    return age;
}
int Person::getDecadesOld() const{
    return decades_old;
}

void Person::ageOneYear(){
    age = age + 1;
    updateDecadesOld();
}

void Person::updateDecadesOld(){
    decades_old = age / 10;
}

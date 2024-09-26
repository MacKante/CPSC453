#include "ResourceManager.h"
#include <iostream>
#include <stdexcept>

ResourceManager::ResourceManager() 
    : resource(new int(0)) { // Resource Acquisition (1): Acquires resource by allocating memory
    std::cout << "Resource acquired with default value 0.\n";
}

ResourceManager::ResourceManager(int value) 
    : resource(new int(value)) { // Resource Acquisition (1): Allocates resource with specified value
    std::cout << "Resource acquired with value " << value << ".\n";
}

ResourceManager::ResourceManager(const ResourceManager& other) 
    : resource(new int(*other.resource)) { // Resource Management (3): Copies resource to handle deep copy
    std::cout << "Resource copied with value " << *resource << ".\n";
}

ResourceManager::ResourceManager(ResourceManager&& other) noexcept 
    : resource(other.resource) { // Resource Acquisition (1) and Ownership (5): Transfers ownership
    other.resource = nullptr; // Prevents double deletion by setting the old resource to null
    std::cout << "Resource moved.\n";
}

ResourceManager& ResourceManager::operator=(const ResourceManager& other) {
    if (this == &other) return *this; // Handle self-assignment: Prevents unnecessary operations

    // Resource Management (3): Releases current resource before copying new resource
    delete resource;

    resource = new int(*other.resource); // Resource Acquisition (1): Allocates and copies new resource
    std::cout << "Resource assigned with value " << *resource << ".\n";

    return *this;
}

ResourceManager& ResourceManager::operator=(ResourceManager&& other) noexcept {
    if (this == &other) return *this; // Handle self-assignment: Avoids redundant operations

    // Resource Management (3) and Ownership (5): Releases current resource and transfers ownership
    delete resource;

    resource = other.resource; // Takes over the resource
    other.resource = nullptr; // Prevents double deletion
    std::cout << "Resource move assigned.\n";

    return *this;
}

ResourceManager::~ResourceManager() {
    // Automatic Cleanup (2): Releases the resource when the object is destroyed
    if (resource) {
        std::cout << "Resource released.\n";
        delete resource;
    }
    // Deterministic Destruction (6): Ensures the resource is cleaned up when the object goes out of scope
}

void ResourceManager::setValue(int value) {
    if (resource) {
        *resource = value;
    }
}

int ResourceManager::getValue() const {
    return resource ? *resource : 0;
}

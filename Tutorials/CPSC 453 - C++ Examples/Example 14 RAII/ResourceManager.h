#pragma once

class ResourceManager {
public:
    // Default constructor: Initializes resource
    ResourceManager();

    // Parameterized constructor: Acquires the resource (sets the value)
    ResourceManager(int value);

    // Copy constructor: Handles copying of resources
    ResourceManager(const ResourceManager& other);

    // Move constructor: Transfers ownership of the resource
    ResourceManager(ResourceManager&& other) noexcept;

    // Copy assignment operator: Handles assignment of resources
    ResourceManager& operator=(const ResourceManager& other);

    // Move assignment operator: Transfers ownership in assignment
    ResourceManager& operator=(ResourceManager&& other) noexcept;

    // Destructor: Releases the resource
    ~ResourceManager();

    // Member functions
    void setValue(int value);
    int getValue() const;

private:
    int* resource; // Pointer to the dynamically allocated resource
};

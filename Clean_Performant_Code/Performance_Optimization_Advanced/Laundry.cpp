// ===========================================================================
// Laundry.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../Person/Person.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <print>
#include <string>

namespace MemoryLaundry {

    // =======================================================================
    // Memory Laundry: std::launder
    static void test_std_launder()
    {
        constexpr int CountPersons{ 5 };

        alignas(class Person) unsigned char buffer[sizeof(class Person) * CountPersons];

        for (size_t i{}; i != CountPersons; ++i) {

            std::string first{ "first_name_" };
            std::string last{ "last_name_" };

            // manually construct objects using placement new
            new(buffer + sizeof(Person) * i) Person{ first + std::to_string(i), last + std::to_string(i), i };
        }

        auto ptr{ std::launder(reinterpret_cast<Person*>(buffer)) };

        for (size_t i{}; i != CountPersons; ++i) {

            std::destroy_at(&ptr[i]);
        }
    }
}

// ===========================================================================
// End-of-File
// ===========================================================================

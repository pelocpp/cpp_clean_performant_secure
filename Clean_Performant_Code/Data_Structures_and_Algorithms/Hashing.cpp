// ===========================================================================
// Hashing.cpp
// ===========================================================================

#include "../Person/Person.h"

#include <print>
#include <string>
#include <unordered_map>

namespace std
{
    // =======================================================================
    // std::hash

    template <>
    struct hash<Person>
    {
        size_t operator()(const Person& p) const {

            auto hash1{ std::hash<std::string>() (p.getFirstname()) };
            auto hash2{ std::hash<size_t>() (p.getAge()) };
            auto hash3{ std::hash<std::string>() (p.getLastname()) };

            size_t hash{ hash1 ^ (hash2 << 1) ^ (hash3 << 2) };  // combine these hash values
            return hash;
        }
    };
}

namespace DataStructuresAndAlgorithms {

    namespace Hashing {

        // =======================================================================
        // Use Cases

        static void test_hashing_01()
        {
            std::unordered_map<Person, size_t> phoneBook;

            Person sepp = { "Sepp", "Meier", 30 };
            phoneBook[sepp] = 123456;

            // retrieving phone number of sepp
            auto phone{ phoneBook[sepp] };
            std::println("{} - Phone Number: {}", sepp, phone);
        }

        static void test_hashing_02()
        {
            std::unordered_map<Person, std::string> personJobs;

            Person sepp = { "Sepp", "Meier", 30 };
            personJobs[sepp] = "Entwickler";

            // retrieving job description of sepp
            auto job{ personJobs[sepp] };
            std::println("{} - Job: {}", sepp, job);
        }
    }
}

// =================================================================

void test_hashing()
{
    using namespace DataStructuresAndAlgorithms::Hashing;
    test_hashing_01();
    test_hashing_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================

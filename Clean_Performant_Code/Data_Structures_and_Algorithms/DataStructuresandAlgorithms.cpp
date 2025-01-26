// ===========================================================================
// DataStructuresandAlgorithms.cpp
// ===========================================================================

#include <iostream>
#include <print>
#include <string>
#include <unordered_map>

namespace DataStructuresandAlgorithms_Hashing {

    // =======================================================================
    // Hashing

    struct Person
    {
        std::string m_firstName;
        std::string m_lastName;
        size_t      m_age;

        bool operator== (const Person& other) const {

            return 
                m_firstName == other.m_firstName && 
                m_lastName == other.m_lastName && 
                m_age == other.m_age;
        }
    };
}

namespace std
{
    // =======================================================================
    // Supporting std::hash

    using namespace DataStructuresandAlgorithms_Hashing;

    template <>
    struct hash<Person>
    {
        size_t operator()(const Person& p) const {

            auto h1 { std::hash<std::string>()(p.m_firstName) };
            auto h2 { std::hash<size_t>()(p.m_age) };
            auto h3 { std::hash<std::string>()(p.m_lastName) };

            size_t hash{ h1 ^ (h2 << 1) ^ (h3 << 2) };  // combine these hash values

            return hash;
        }
    };
}

namespace std
{
    // =======================================================================
    // Supporting std::println

    using namespace DataStructuresandAlgorithms_Hashing;

    template <>
    struct formatter<Person>
    {
        constexpr auto parse(std::format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const Person& person, std::format_context& ctx) const {

            auto firstName{ person.m_firstName };
            auto lastName{ person.m_lastName };
            auto age{ person.m_age };

            return std::format_to(
                ctx.out(),
                "Person {} {} [Age: {}]",
                firstName, lastName, age
            );
        }
    };
}

namespace DataStructuresandAlgorithms_Hashing {

    // =======================================================================
    // Hashing

    static void hashing_01()
    {
        std::unordered_map<Person, size_t> phoneBook;

        Person sepp = { "Sepp", "Meier", 30 };
        phoneBook[sepp] = 123456;

        // retrieving phone number of sepp
        auto phone{ phoneBook[sepp] };
        std::println("{} - Phone Number: {}", sepp, phone);
    }

    static void hashing_02()
    {
        std::unordered_map<Person, std::string> personJobs;

        Person sepp = { "Sepp", "Meier", 30};
        personJobs[sepp] = "Entwickler";

        // retrieving job description of sepp
        auto job{ personJobs[sepp] };
        std::println("{} - Job: {}", sepp, job);
    }
}

// =================================================================

void data_atructures_and_algorithms()
{
    using namespace DataStructuresandAlgorithms_Hashing;

    hashing_01();
    hashing_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================

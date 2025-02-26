// ===========================================================================
// DataStructuresAndAlgorithms.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <iostream>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

namespace DataStructuresAndAlgorithms {

    namespace Hashing {

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
}

namespace std
{
    // =======================================================================
    // std::hash

    using namespace DataStructuresAndAlgorithms::Hashing;

    template <>
    struct hash<Person>
    {
        size_t operator()(const Person& p) const {

            auto hash1 { std::hash<std::string>() (p.m_firstName) };
            auto hash2 { std::hash<size_t>() (p.m_age) };
            auto hash3 { std::hash<std::string>() (p.m_lastName) };

            size_t hash{ hash1 ^ (hash2 << 1) ^ (hash3 << 2) };  // combine these hash values
            return hash;
        }
    };
}

namespace std
{
    // =======================================================================
    // std::println

    using namespace DataStructuresAndAlgorithms::Hashing;

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

namespace DataStructuresAndAlgorithms {

    namespace UsingParallelArrays {

        //class SmallObject {
        //private:
        //    std::array<char, 4> m_data;
        //    int m_score;
        //public:
        //    SmallObject() : m_data{}, m_score{ std::rand() } {}
        //};
        //
        //class BigObject {
        //private:
        //    std::array<char, 256> m_data;
        //    int m_score;
        //public:
        //    BigObject() : m_data{}, m_score{ std::rand() } {}
        //};


        // =======================================================
        // calculate sizes of SmallObject and BigObject objects

        template <size_t Size>
        class Object {
        private:
            std::array<char, Size> m_data;
            size_t                 m_score;
        public:
            Object() : m_data{}, m_score{ static_cast<size_t>(std::rand()) } {}

            auto getScore() const { return m_score; }
        };

        using SmallObject = Object<4>;
        using BigObject = Object<128>;

        static void test_parallel_arrays_01() {

            std::println("{}", sizeof(SmallObject));
            std::println("{}", sizeof(BigObject));
        }

        // =======================================================
        // evaluating performance - need a lot of objects

        constexpr auto Size = 1'000'000;

        auto smallObjects = std::vector<SmallObject>(Size);
        auto bigObjects = std::vector<BigObject>(Size);

        template <class T>
        auto sumScores(const std::vector<T>& objects) {

            ScopedTimer watch{};

            size_t sum{ 0 };

            for (const auto& obj : objects) {
                sum += obj.getScore();
            }

            return sum;
        }

        static void test_parallel_arrays_02() {

            std::srand(static_cast<unsigned int>(std::time({})));

            size_t sum{ 0 };

            sum += sumScores(smallObjects);
            sum += sumScores(bigObjects);
        }
    }
}

// =================================================================

void data_structures_and_algorithms()
{
    //using namespace DataStructuresAndAlgorithms::Hashing;
    //test_hashing_01();
    //test_hashing_02();

    using namespace DataStructuresAndAlgorithms::UsingParallelArrays;
 //   test_parallel_arrays_01();
    test_parallel_arrays_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================

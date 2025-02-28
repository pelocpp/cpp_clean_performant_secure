// ===========================================================================
// DataStructuresAndAlgorithms.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

// https://github.com/PacktPublishing/Cpp-High-Performance-Second-Edition

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

        // =======================================================
        // calculate sizes of SmallObject and BigObject objects

        template <size_t Size>
        class Object {
        private:
            std::array<char, Size> m_data{};
            int m_score{ std::rand() };
        public:
            auto getScore() const { return m_score; }
        };

        using SmallObject = Object<4>;

        using BigObject = Object<256>;

        static void test_parallel_arrays_01() {

            std::println("{}", sizeof(SmallObject));
            std::println("{}", sizeof(BigObject));
        }

        // =======================================================
        // evaluating performance - using a lot of objects

        constexpr auto Size = 1;  // set to 1'000'000

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

    // =======================================================
    // Examine small and big objects 

    // debug mode:   use 5'000'000
    // release mode: use 30'000'000
    constexpr size_t NumObjects{ 10 };

    namespace UsingParallelArrays_OriginalUser {
    
        struct User
        {
            std::string m_name;
            std::string m_username;
            std::string m_password;
            std::string m_securityQuestion;
            std::string m_securityAnswer;
            short       m_level{};
            bool        m_isPlaying{};
        };

        // =============================================
        // Some utility functions for creating test data

        static auto numUsersAtLevel(short level, const std::vector<User>& users) {

            ScopedTimer watch{};

            auto num_users{ 0 };
            for (const auto& user : users) {
                if (user.m_level == level) {
                    ++num_users;
                }
            }
            return num_users;
        }

        static auto numPlayingUsers(const std::vector<User>& users) {

            ScopedTimer watch{};

            auto result{ 
                std::count_if(
                    users.begin(),
                    users.end(),
                    [](const auto& user) { return user.m_isPlaying; }
                )
            };

            return result;
        }

        static auto genIsPlaying() {
            return static_cast<bool>(std::rand() % 2);
        }

        static auto genLevel() {
            return static_cast<short>(std::rand() % 100);
        }

        static auto createUsers(size_t count) {

            auto users{ std::vector<User>(count) };

            std::generate(
                users.begin(),
                users.end(),
                [] () { 
                    return User{ "AnyName", "", "", "", "", genLevel() , genIsPlaying() }; 
                }
            );

            return users;
        }

        static void test_parallel_arrays_with_original_users() {

            std::srand(123);  // want same output of all test functions

            std::println("Creating regular User Objects ...");
            std::println("sizeof(User): {}", sizeof(User));

            auto users{ createUsers(NumObjects) };
            std::println("Done.");

            auto n{ size_t{ 0 } };
            auto level{ short{ 5 } };

            std::println("+++ Count stats using original User class +++");

            n += numUsersAtLevel(level, users);
            n += numUsersAtLevel(level, users);
            n += numUsersAtLevel(level, users);

            n += numPlayingUsers(users);
            n += numPlayingUsers(users);
            n += numPlayingUsers(users);

            // prevent n from being optmized away ...
            std::println("n: {}\n", n);
        }
    }

    namespace UsingParallelArrays_ImprovedUser {

        struct AuthInfo {
            std::string m_username;
            std::string m_password;
            std::string m_securityQuestion;
            std::string m_securityAnswer;
        };

        struct User {
            std::string m_name;
            std::unique_ptr<AuthInfo> auth_info_;
            short m_level{};
            bool m_isPlaying{};
        };

        // =============================================
        // Some utility functions for creating test data

        static auto numUsersAtLevel(short level, const std::vector<User>& users) {

            ScopedTimer watch{};

            auto num_users{ 0 };
            for (const auto& user : users) {
                if (user.m_level == level) {
                    ++num_users;
                }
            }
            return num_users;
        }

        static auto numPlayingUsers(const std::vector<User>& users) {

            ScopedTimer watch{};

            auto result{
                std::count_if(
                    users.begin(),
                    users.end(),
                    [](const auto& user) { return user.m_isPlaying; }
                )
            };
            return result;
        }

        static auto genIsPlaying() {
            return static_cast<bool>(std::rand() % 2);
        }

        static auto genLevel() {
            return static_cast<short>(std::rand() % 100);
        }

        static auto createUsers(size_t count) {

            auto users{ std::vector<User>(count) };

            std::generate(
                users.begin(),
                users.end(),
                [] () { 
                    return User{ "AnyName",  std::make_unique<AuthInfo>(), genLevel() , genIsPlaying() };
                }
            );

            return users;
        }

        static void test_parallel_arrays_with_improved_users() {

            std::srand(123);  // want same output of all test functions

            std::println("Creating smaller User Objects ...");
            std::println("sizeof(User): {}", sizeof(User));

            auto users{ createUsers(NumObjects) };
            std::println("Done.");

            auto n{ size_t{ 0 } };
            auto level{ short{ 5 } };

            std::println("+++ Count stats using improved User class +++");

            n += numUsersAtLevel(level, users);
            n += numUsersAtLevel(level, users);
            n += numUsersAtLevel(level, users);

            n += numPlayingUsers(users);
            n += numPlayingUsers(users);
            n += numPlayingUsers(users);

            // prevent n from being optmized away ...
            std::println("n: {}\n", n);
        }
    }

    namespace UsingParallelArrays_ParallelUserData {

        // =============================================
        // Some utility functions for creating test data

        static auto createLevels(size_t count) {
            auto vec = std::vector<short>(count);
            for (auto& level : vec) {
                level = std::rand() % 100;
            }
            return vec;
        };

        static auto createPlayingUsers(size_t count) {

            auto vec = std::vector<bool>(count); // Note: std::vector<bool> is special
            for (auto&& is_playing : vec) {
                is_playing = static_cast<bool>(std::rand() % 2);
            }
            return vec;
        };

        static auto numUsersAtLevel(short level, const std::vector<short>& users) {
            ScopedTimer watch{};
            return std::count(users.begin(), users.end(), level);
        }

        static auto numPlayingUsers(const std::vector<bool>& users) {
            ScopedTimer watch{};
            return std::count(users.begin(), users.end(), true);
        }

        static void test_parallel_arrays_with_parallel_user_data()
        {
            std::srand(123);  // want same output of all test functions

            std::println("Creating Parallel Arrays ...");

            auto userLevels{ createLevels(NumObjects) };
            auto playingUsers{ createPlayingUsers(NumObjects) };

            std::println("Done.");

            auto n{ size_t{ 0 } };
            auto level{ short{ 5 } };

            std::println("+++ Count stats using vector<short> and vector<bool> +++");

            n += numUsersAtLevel(level, userLevels);
            n += numUsersAtLevel(level, userLevels);
            n += numUsersAtLevel(level, userLevels);

            n += numPlayingUsers(playingUsers);
            n += numPlayingUsers(playingUsers);
            n += numPlayingUsers(playingUsers);

            // prevent n from being optmized away ...
            std::println("n: {}\n", n);
        }
    }

    namespace Vectors {

        static void test_vector()
        {
            {
                auto vec = std::vector{ 1, 2, 3 };

                vec.push_back(4);
                vec.push_back(5);

                std::println("Size: {} - Capacity: {}", vec.size(), vec.capacity());
            }

            {
                auto vec = std::vector{ 1, 2, 3, 4, 5 };
                for (auto elem : vec) {
                    std::print("{} ", elem);
                }
                std::println();
            }

            {
                auto vec = std::vector{ -1, 5, 2, -3, 4, -5, 5 };

                std::erase(vec, 5);                                 // v: [-1, 2, -3, 4, -5]
                for (auto elem : vec) {
                    std::print("{} ", elem);
                }
                std::println();

                std::erase_if(vec, [] (auto x) { return x < 0; });  // v: [2, 4]
                for (auto elem : vec) {
                    std::print("{} ", elem);
                }
                std::println();
            }
        }

        static auto print(const std::array<int, 10>& a) {

            for (auto elem : a) {
                std::print("{} ", elem);
            }
        }

        static void test_array()
        {
            {
                auto a = std::array<int, 5>{ 1, 2, 3, 4, 5 };

                auto b = std::array{ 1, 2, 3, 4, 5 };
            }

            {
                auto a = std::array<int, 5>{ 1, 2, 3, 4, 5 };

                for (auto elem : a) {
                    std::print("{} ", elem);
                }
            }

            {
                auto a = std::array<int, 10>{};
                auto b = std::array<int, 20>{};

                print(a);
                // print(b); // does not compile, print requires an int array of size 10
            }
        }
    }

    namespace Strings {

        static void test_strings() {

            auto in = std::ifstream{ "../Data_Structures_and_Algorithms/Program.cpp", std::ios::binary | std::ios::ate };

            if (in.is_open()) {
                auto size = in.tellg();
                auto content = std::string(size, '\0');
                in.seekg(0);
                in.read(&content[0], size);   // "content" now contains the entire file
            }
        }
    }
}

// =================================================================

void data_structures_and_algorithms()
{
    //using namespace DataStructuresAndAlgorithms::Hashing;
    //test_hashing_01();
    //test_hashing_02();

    //using namespace DataStructuresAndAlgorithms::UsingParallelArrays;
    //test_parallel_arrays_01();
    //test_parallel_arrays_02();

    //using namespace DataStructuresAndAlgorithms::UsingParallelArrays_OriginalUser;
    //test_parallel_arrays_with_original_users();

    //using namespace DataStructuresAndAlgorithms::UsingParallelArrays_ImprovedUser;
    //test_parallel_arrays_with_improved_users();

    //using namespace DataStructuresAndAlgorithms::UsingParallelArrays_ParallelUserData;
    //test_parallel_arrays_with_parallel_user_data();

     using namespace DataStructuresAndAlgorithms::Vectors;
     test_vector();
     //test_array();

    //using namespace DataStructuresAndAlgorithms::Strings;
    //test_strings();

}

// ===========================================================================
// End-of-File
// ===========================================================================

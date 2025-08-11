// ===========================================================================
// ParallelArrays.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <algorithm>
#include <array>
#include <memory>
#include <print>
#include <string>
#include <vector>

namespace DataStructuresAndAlgorithms {

    namespace SmallVsBigObjects {

        // =======================================================
        // calculate sizes of SmallObject and BigObject objects

        template <size_t Length>
        class Object {
        private:
            char m_data[Length]{};
            int m_score{ std::rand() };
        public:
            Object() = default;
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

#ifdef _DEBUG
        // Note: Extremely long compilation times: Values greater than 2'000'000 won't work
        // For 2'000'000 the results in Release Mode are:
        // Elapsed time : 2 milliseconds.
        // Elapsed time : 12 milliseconds.

        // constexpr auto Size = 1;  // just to prevent huge compilation times!
        constexpr auto Size = 1'000'000;
#else
        constexpr auto Size = 1;  // just to prevent huge compilation times!
        // constexpr auto Size = 2'000'000; 
#endif

        auto smallObjects = std::vector<SmallObject>(Size);
        auto bigObjects = std::vector<BigObject>(Size);

        template <class T>
        auto sumScores(const std::vector<T>& objects) {

            ScopedTimer watch{};

            volatile size_t sum{ 0 };

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

#ifdef _DEBUG
    constexpr size_t NumObjects{ 5'000'000 };    // debug mode
#else
    constexpr size_t NumObjects{ 30'000'000 };   // release mode
#endif

    namespace ParallelArrays_OriginalUser {

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
                []() {
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

    namespace ParallelArrays_ImprovedUser {

        struct AuthInfo {
            std::string m_username;
            std::string m_password;
            std::string m_securityQuestion;
            std::string m_securityAnswer;
        };

        struct User {
            std::string               m_name;
            std::unique_ptr<AuthInfo> m_authInfo;
            short                     m_level{};
            bool                      m_isPlaying{};
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
                []() {
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

    namespace ParallelArrays_ParallelUserData {

        // =============================================
        // Some utility functions for creating test data

        static auto createLevelsAndPlayingUsers(size_t count)
        {
            auto vec1 = std::vector<short>(count);
            auto vec2 = std::vector<bool>(count); // Note: std::vector<bool> is special

            for (size_t i{}; i != count; ++i) {
                vec1[i] = std::rand() % 100;
                vec2[i] = static_cast<bool>(std::rand() % 2);
            }

            return std::pair{ vec1, vec2 };
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

            const auto& [userLevels, playingUsers] = createLevelsAndPlayingUsers(NumObjects);

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

    namespace Structuring_Data_For_Cache_Efficiency {

        constexpr size_t Iterations{ 100 };
        constexpr size_t Width{ 1'000 };
        constexpr size_t Height{ 1'000 };

        static uint8_t modifyGreenIntensity(uint8_t green) {
            green += 5;
            return green;
        }

        namespace Array_of_Structures_AoS {

            struct Pixel
            {
                uint8_t m_red;
                uint8_t m_green;
                uint8_t m_blue;
            };

            static void test_aos_single()
            {
                std::println("Running Array of Structures (AoS) ...");

                // array of structures (AoS)
                std::vector<Pixel> pixels(Width * Height);

                ScopedTimer watch{};

                for (size_t k = 0; k < Iterations; k++) {
                    // example loop for modifying green channel intensity
                    for (size_t i = 0; i < Width * Height; i++) {
                        pixels[i].m_green = modifyGreenIntensity(pixels[i].m_green);
                    }
                }
            }

            static void test_aos()
            {
                test_aos_single();
                test_aos_single();
                test_aos_single();
                test_aos_single();
            }
        }

        namespace Structure_of_Arrays_SoA {

            // better cache usage with Structure of Arrays (SoA)
            struct Pixels
            {
                std::vector<uint8_t> red;
                std::vector<uint8_t> green;
                std::vector<uint8_t> blue;
            };

            static void test_soa_single()
            {
                std::println("Running Structure of Arrays (SoA) ...");

                Pixels pixels;
                pixels.red.resize(Width * Height);
                pixels.green.resize(Width * Height);
                pixels.blue.resize(Width * Height);

                ScopedTimer watch{};

                for (size_t k = 0; k < Iterations; k++) {

                    // example loop for modifying green channel intensity
                    for (size_t i = 0; i < Width * Height; i++) {
                        pixels.green[i] = modifyGreenIntensity(pixels.green[i]);
                    }
                }
            }

            static void test_soa()
            {
                test_soa_single();
                test_soa_single();
                test_soa_single();
                test_soa_single();
            }
        }
    }
}

// =================================================================

void test_parallel_arrays()
{
    using namespace DataStructuresAndAlgorithms;

    using namespace SmallVsBigObjects;
    test_parallel_arrays_01();
    test_parallel_arrays_02();

    //using namespace ParallelArrays_OriginalUser;
    //test_parallel_arrays_with_original_users();

    //using namespace ParallelArrays_ImprovedUser;
    //test_parallel_arrays_with_improved_users();

    //using namespace ParallelArrays_ParallelUserData;
    //test_parallel_arrays_with_parallel_user_data();

    //using namespace Structuring_Data_For_Cache_Efficiency;
    //Array_of_Structures_AoS::test_aos();
    //std::println();
    //Structure_of_Arrays_SoA::test_soa();
}

// ===========================================================================
// End-of-File
// ===========================================================================

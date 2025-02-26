// ===========================================================================
// DataStructuresAndAlgorithms.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <iostream>
#include <new>
#include <print>
#include <string>
#include <unordered_map>

#include <cstdlib>

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <processthreadsapi.h>

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

    namespace CacheLines {

        static void test_examine_cache_line_size() {

            constexpr auto cachelineSize = std::hardware_destructive_interference_size;

            std::println("Cache Line Size: {}", cachelineSize);
        }

        static void test_examine_l1_cache_size() {

            // https://gist.github.com/kimwalisch/16c34ae16447b245464a

            typedef BOOL(WINAPI* LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

            auto handle = GetModuleHandle(L"kernel32");

            auto glpi = (LPFN_GLPI) GetProcAddress(handle, "GetLogicalProcessorInformation");
            if (glpi == NULL)
                return;

            DWORD bufferBytes = 0;
            int cacheSize = 0;

            // calculate buffer length
            BOOL ret = glpi(0, &bufferBytes);

            std::size_t size = bufferBytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
            
            SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = new SYSTEM_LOGICAL_PROCESSOR_INFORMATION[size];
            
            // retrieve array of SYSTEM_LOGICAL_PROCESSOR_INFORMATION structures
            ret = glpi(buffer, &bufferBytes);

            for (std::size_t i = 0; i < size; i++)
            {
                if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
                {
                    cacheSize = (int)buffer[i].Cache.Size;
                    break;
                }
            }

            delete[] buffer;

            auto cacheSizeKB = cacheSize / 1024;
            std::println("L1 Cache Size: {} kB", cacheSizeKB);
        }

        static void test_examine_cache_sizes() {

      //      test_examine_cache_line_size();
            test_examine_l1_cache_size();
        }

    }

    namespace CacheLinesAndCacheMisses {

        constexpr auto cachelineSize = std::hardware_destructive_interference_size;

        constexpr auto capacityL1CacheSize = 32768; // L1 data cache size
        
        constexpr auto Size = capacityL1CacheSize / sizeof(int);
        // constexpr auto Size = 10;

        using MatrixType = std::array<std::array<size_t, Size>, Size>;

        static MatrixType matrix;

        static auto initMatrix(MatrixType& matrix) {

            ScopedTimer watch{};

            size_t value{};

            for (size_t i{}; i != Size; ++i) {
                for (size_t j{}; j != Size; ++j) {
                    //matrix[i][j] = value++;      // no "cache thrashing"
                    matrix[j][i] = value++;    // remove comment: demonstrates "cache thrashing"
                }
            }
        }

        static void test_cache_thrashing() {
            initMatrix(matrix);
        }
    }
}

// =================================================================

void data_structures_and_algorithms()
{
//    using namespace DataStructuresAndAlgorithms::Hashing;
//    test_hashing_01();
//    test_hashing_02();

    using namespace DataStructuresAndAlgorithms::CacheLines;
    test_examine_cache_sizes();

    using namespace DataStructuresAndAlgorithms::CacheLines;
    ////test_cache_thrashing();
    //test_cache_thrashing();
    //// test_examine_cache_lines();
}

// ===========================================================================
// End-of-File
// ===========================================================================

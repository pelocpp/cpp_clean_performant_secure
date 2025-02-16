// ===========================================================================
// SecureProgrammingAdvices.cpp
// Advices for Secure Programming
// ===========================================================================

#include <string.h>

#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <print>
#include <string>

namespace SecureProgrammingAdvices {

    namespace PreferCppToC {

        static void test_prefer_cpp_to_c_01() {

            // use std::string
            std::string first{ "Hello " };
            std::string second{ "World" };
            std::string result{ first + second };
            std::println("{}", result);
        }

        static void test_prefer_cpp_to_c_02() {

            char first[] = "Hello ";
            char second[] = "World";
            char result[20];

            strcpy_s(result, 20, first);
            strcat_s(result, 20, second);
            std::printf("%s\n", result);
        }
    }

    namespace TakeCareOfOverflow {

        static void test_take_care_of_overflow() {

            std::uint32_t a = std::numeric_limits<std::uint32_t>::max();
            std::uint32_t b = std::numeric_limits<std::uint32_t>::max() - 2;

            std::println("a:                                 {}", a);
            std::println("b:                                 {}", b);
            std::println("Incorrect (overflow and wrapping): {}", (a + b) / 2);
            std::println("Correct:                           {}", std::midpoint(a, b));
        }
    }

    namespace UseAlgorithms {

        static void test_use_algorithms_01() {

            std::string str{ "Hello World" }; // use also "Hello:World"
            std::println("{}", str);

            auto isColon = [] (int ch) { return ch == ':'; };

            auto pos = std::find_if(str.begin(), str.end(), isColon);

            // if found - delete everything afterwards
            if (pos != str.end()) {
                str.erase(pos, str.end());
            }

            std::println("{}", str);
        }

        static void test_use_algorithms_02() {

            char str[] = "Hello World";    // use also "Hello:World"
            std::printf("%s\n", str);

            const char* pos = strstr(str, ":");

            if (pos != NULL) {

                // if found - poke '\0' into string - this deletes everything afterwards
                size_t ofs = pos - str;
                str[ofs] = '\0';
            }

            std::printf("%s\n", str);
        }
    }

    namespace SafeDowncasting {

        class Spiderman {};
        class Ironman {};

        static void test_safe_downcasting_01() {

            Spiderman* ptr = new Spiderman;

            Ironman* ptr2 = NULL;
            
            ptr = (Spiderman*) ptr;
        }

        static void test_safe_downcasting_02() {

            Spiderman* ptr = new Spiderman;

            Ironman* ptr2 = nullptr;

            // compile error: 'static_cast': cannot convert from 'Spiderman *' to 'Ironman *'
            // ptr2 = static_cast<Ironman*>(ptr);
        }
    }

    namespace DontUseNewExplicitely {

        static void test_dont_use_new_explicitely() {

            {
                std::string s{ "Hello World" };
                std::println("{}", s);
            }
            //  <== GC happens here 
        }
    }

    namespace GivePrimitiveDatatypesSemantics {

        enum class Direction : char {
            NORTH = 'N',
            EAST = 'E',
            WEST = 'W',
            SOUTH = 'S'
        };

        static std::ostream& operator<<(std::ostream& os, const Direction& obj) {
            os << static_cast<std::underlying_type<Direction>::type>(obj);
            return os;
        }

        static void test_give_primitive_datatypes_semantics() {

            std::cout << "\t" << Direction::NORTH << "\n"
                << "\t" << Direction::EAST << "\n"
                << "\t" << Direction::WEST << "\n"
                << "\t" << Direction::SOUTH << "\n";
        
            // NACHZIEHEN !!!
            //std::println("{}", Direction::NORTH);
            //std::println("{}", Direction::EAST);
            //std::println("{}", Direction::WEST);
            //std::println("{}", Direction::SOUTH);
        
        }
    }

    namespace UseStringLiterals {

        static void test_use_string_Literals() {

            using namespace std::literals::string_literals;

            auto heroes = { "Spiderman"s, "Ironman"s, "Wonder Woman"s };

            for (auto const& hero : heroes) {
                std::println("{:12} ({})", hero, hero.size());
            }
        }
    }

    namespace UseUserDefinedLiterals {

        class Hours{
        private:
            unsigned long long m_hours = 0;

        public:
            Hours() : Hours(0) {}
            explicit Hours (unsigned long long hours) : m_hours(hours) {}
        };
        
        struct Days {
        private:
            unsigned long long m_hours = 0;

        public:
            Days() : Days(0) {}
            explicit Days(unsigned long long hours) : m_hours(hours) {}
        };

        static Hours operator"" _hours(unsigned long long hours) {
            return Hours{ hours };
        }

        static Days operator"" _days(unsigned long long hours) {
            return Days{ hours };
        }

        static void test_use_user_defined_literals() {

            auto h = 23_hours;

            auto d = 7_days;

            // auto wrong = h + d;   // doesn't compile: Errir

            // binary '+': 'Hours' does not define this operator or a conversion to a type acceptable to the predefined operator
        }
    }

    namespace UseUserDefinedLiteralsWithConversion {

        using hours = unsigned long long;

        static constexpr hours operator"" _hours(unsigned long long hours) {
            return hours;
        }

        static constexpr hours operator"" _days(unsigned long long hours) {
            return hours * 24;
        }

        static constexpr hours operator"" _weeks(unsigned long long hours) {
            return hours * 7 * 24;
        }

        static void test_use_user_defined_literals() {

            auto hours = 12_hours;

            auto days = 2_days;

            auto weeks = 3_weeks;

            auto totalHours = weeks + days + hours;
        }

        static void test_use_user_defined_literals_constexpr() {

            constexpr auto hours = 12_hours;

            constexpr auto days = 2_days;

            constexpr auto weeks = 3_weeks;

            constexpr auto totalHours = weeks + days + hours; // 12 + 2*24 + 3*7*24 = 564
        }
    }
}

// ===========================================================================

void secure_programming_advices()
{
    using namespace SecureProgrammingAdvices;

    //PreferCppToC::test_prefer_cpp_to_c_01();
    //PreferCppToC::test_prefer_cpp_to_c_02();


    TakeCareOfOverflow::test_take_care_of_overflow();

    //UseAlgorithms::test_use_algorithms_01();
    //UseAlgorithms::test_use_algorithms_02();

    //SafeDowncasting::test_safe_downcasting_01();
    //SafeDowncasting::test_safe_downcasting_02();

   // DontUseNewExplicitely::test_dont_use_new_explicitely();

    //GivePrimitiveDatatypesSemantics::test_give_primitive_datatypes_semantics();

   // UseStringLiterals::test_use_string_Literals();
}

// ===========================================================================
// End-of-File
// ===========================================================================

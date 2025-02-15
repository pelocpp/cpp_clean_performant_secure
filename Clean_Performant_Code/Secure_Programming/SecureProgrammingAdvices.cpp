// ===========================================================================
// SecureProgrammingAdvices.cpp
// ===========================================================================

#include <string.h>

#include <iostream>
#include <algorithm>
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

        std::ostream& operator<<(std::ostream& os, const Direction& obj) {
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

}

// ===========================================================================

void secure_programming_advices()
{
    using namespace SecureProgrammingAdvices;

    //PreferCppToC::test_prefer_cpp_to_c_01();
    //PreferCppToC::test_prefer_cpp_to_c_02();

    //UseAlgorithms::test_use_algorithms_01();
    //UseAlgorithms::test_use_algorithms_02();

    //SafeDowncasting::test_safe_downcasting_01();
    //SafeDowncasting::test_safe_downcasting_02();

   // DontUseNewExplicitely::test_dont_use_new_explicitely();

    GivePrimitiveDatatypesSemantics::test_give_primitive_datatypes_semantics();
}

// ===========================================================================
// End-of-File
// ===========================================================================

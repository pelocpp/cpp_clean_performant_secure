// ===========================================================================
// SecureProgrammingAdvices.cpp
// Advices for Secure Programming
// ===========================================================================

#include <complex>
#include <cstdint>
#include <format>
#include <iostream>
#include <limits>
#include <numeric>
#include <print>
#include <string>

namespace SecureProgrammingAdvices {

    namespace PreferCppToC {

        // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#cpl1-prefer-c-to-c

        // CPL.1: Prefer C++ to C

        static void test_prefer_cpp_to_c_01() {

            // use 'std::string'
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

        static void test_prefer_cpp_to_c() {

            test_prefer_cpp_to_c_01();
            test_prefer_cpp_to_c_02();
        }
    }

    namespace TakeCareOfBufferOverflow {

        static void test_take_care_of_buffer_overflow_01() {

            char buffer[16];

            const char* str = "This is way too long for this buffer";
            std::println("Source:      >{}<", str);

            auto length = strlen(str);
            auto size = std::size(buffer);

            // strncpy_s(buffer, size, str, length);      // crashes
            strncpy_s(buffer, size, str, size - 1);       //  copy with adjusted boundary

            buffer[size - 1] = '\0';

            std::println("Destination: >{}<", buffer);
        }

        static void test_take_care_of_buffer_overflow_02() {

            constexpr int Size = 64;

            char buffer[Size];

            auto bytesWritten = 0;

            bytesWritten = snprintf(buffer, Size, "The half of %d is %d", 60, 60/2);

            if (bytesWritten >= 0 && bytesWritten < Size) {    // check returned value

                bytesWritten = snprintf(buffer + bytesWritten, Size - bytesWritten, ", and the half of that is %d.", 60/2/2);
            }

            std::println("Buffer: >{}< // Bytes written: {}", buffer, bytesWritten);
        }

        static void test_take_care_of_buffer_overflow() {

            test_take_care_of_buffer_overflow_01();
            test_take_care_of_buffer_overflow_02();
        }
    }

    namespace TakeCareOfArithmeticOverflow {

        static void test_take_care_of_arithmetic_overflow() {

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

            // solved with STL algorithm

            std::string str{ "Hello World" }; // use also "Hello:World"
            std::println("{}", str);

            auto isColon = [](int ch) { return ch == ':'; };

            auto pos = std::find_if(str.begin(), str.end(), isColon);

            // if found - delete everything afterwards
            if (pos != str.end()) {
                str.erase(pos, str.end());
            }

            std::println("{}", str);
        }

        static void test_use_algorithms_02() {

            // solved with CRT function 'strstr' // no STL algorithm

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

        static void corrupt_stack(const char* input) {

            char buffer[32];
            std::copy(input, input + 32, buffer);
            std::println("{}", buffer);  // std::copy works, std::println fails, no '\0'
        }

        static void test_use_algorithms_03() {

            corrupt_stack("This is way too long for this buffer");
        }

        static void test_use_algorithms() {

            test_use_algorithms_01();
            test_use_algorithms_02();
            test_use_algorithms_03();
        }
    }

    namespace SafeDowncasting {

        class Spiderman {};
        class Ironman {};

        static void test_safe_downcasting_01() {

            Spiderman* ptr = new Spiderman;

            Ironman* ptr2 = NULL;

            ptr = (Spiderman*)ptr;
        }

        static void test_safe_downcasting_02() {

            Spiderman* ptr = new Spiderman;

            Ironman* ptr2 = nullptr;

            // compile error: 'static_cast': cannot convert from 'Spiderman *' to 'Ironman *'
            // ptr2 = static_cast<Ironman*>(ptr);
        }

        static void test_safe_downcasting() {
            test_safe_downcasting_01();
            test_safe_downcasting_02();
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

    namespace DeclareSingleArgumentConstructorsExplicit {

        class String {
        public:
            /*explicit*/ String(size_t length) : m_length{ length } {};   // Bad
            // ...

        private:
            size_t m_length;
            // ...
        };

        static void test_declare_single_argument_constructors_explicit() {

            String s = '!';  // Uhhh: String of length 33
        }
    }

    namespace GivePrimitiveDatatypesSemantics {

        static void test_use_string_literals() {

            using namespace std::literals::string_literals;

            auto heroes = { "Spiderman"s, "Ironman"s, "Wonder Woman"s };

            for (auto const& hero : heroes) {
                std::println("{:12} ({})", hero, hero.size());
            }
        }

        enum class RainbowColors : char
        {
            Violet = 'V',
            Indigo = 'I',
            Blue   = 'B',
            Green  = 'G',
            Yellow = 'Y',
            Orange = 'O',
            Red    = 'R'
        };

        enum class EyeColors
        {
            Blue,
            Green,
            Brown
        };

        static void test_use_class_enums() {

            std::cout << static_cast<std::underlying_type_t<RainbowColors>>(RainbowColors::Green) << std::endl;
            std::cout << static_cast<std::underlying_type_t<RainbowColors>>(RainbowColors::Orange) << std::endl;
            std::cout << static_cast<std::underlying_type_t<EyeColors>>(EyeColors::Blue) << std::endl;
            std::cout << static_cast<std::underlying_type_t<EyeColors>>(EyeColors::Green) << std::endl;
        }
    }

    namespace GivePrimitiveDatatypesSemantics {

        // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Renum-class

        // Enum.3: Prefer class enums over "plain" enums

        enum class Direction : char
        {
            NORTH = 'N',
            EAST = 'E',
            WEST = 'W',
            SOUTH = 'S'
        };

        static std::ostream& operator<<(std::ostream& os, Direction obj) {
            os << static_cast<std::underlying_type<Direction>::type>(obj);
            return os;
        }
    }
}

namespace std
{
    using namespace SecureProgrammingAdvices::GivePrimitiveDatatypesSemantics;

    // formatter for user defined enum type 'class Direction'
    template<>
    struct std::formatter<Direction> : public std::formatter<char>
    {
        auto format(Direction obj, format_context& ctx) const {

            auto ch = static_cast<std::underlying_type<Direction>::type>(obj);

            // delegate the rest of formatting to the character formatter
            return std::formatter<char>::format(ch, ctx);
        }
    };
}

namespace SecureProgrammingAdvices {

    namespace GivePrimitiveDatatypesSemantics {

        static void test_give_primitive_datatypes_semantics() {

            std::cout << "\t" << Direction::NORTH << "\n"
                << "\t" << Direction::EAST << "\n"
                << "\t" << Direction::WEST << "\n"
                << "\t" << Direction::SOUTH << "\n";
        
            // oder "modern"
            std::println("{:>4}", Direction::NORTH);
            std::println("{:>4}", Direction::EAST);
            std::println("{:>4}", Direction::WEST);
            std::println("{:>4}", Direction::SOUTH);
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

            // auto wrong = h + d;   // doesn't compile: Error

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

    namespace UseOverride {

        // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c128-virtual-functions-should-specify-exactly-one-of-virtual-override-or-final

        // C.128: Virtual functions should specify exactly one of virtual, override, or final

        struct Button {
            // ...
            virtual void onClick() {
                std::println("Button!");
            }
        };

        struct SuperButton : public Button {
            // ...
            void onClick() override {
                std::println("Super Button!");
            }
        };

        static void test_use_override() {
            Button button;
            button.onClick();

            SuperButton superButton;
            superButton.onClick();
        }
    }

    namespace UseConst {

        // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#con2-by-default-make-member-functions-const

        // Con.2:By default, make member functions `const`

        class Point
        {
        private:
            int m_x = 0;
            int m_y = 0;

        public:
            Point() : Point{ 0, 0 } {};
            Point(int x, int y) : m_x{ x }, m_y{ y } {}

            int x() const { return m_x; }
            int y() const { return m_y; }

            bool is_valid() const {
                return m_x >= 0 && m_y >= 0;
            }
        };

        static void test_use_const() {
            Point point;
            bool valid = point.is_valid();
        }
    }

    namespace UseNodiscardAttribute {

        class Point
        {
        private:
            int m_x = 0;
            int m_y = 0;

        public:
            Point() : Point{ 0, 0 } {};
            Point(int x, int y) : m_x{ x }, m_y{ y } {}

            int x() const { return m_x; }
            int y() const { return m_y; }

            [[nodiscard]] bool is_valid() const {
                return m_x >= 0 && m_y >= 0;
            }
        };

        static void test_use_nodiscard() {
            Point point;
            // point.is_valid();   // REMOVE COMMENT  // warning: ignoring return value
        }
    }
}

void secure_programming_advices()
{
    using namespace SecureProgrammingAdvices;

    PreferCppToC::test_prefer_cpp_to_c();
    TakeCareOfBufferOverflow::test_take_care_of_buffer_overflow();
    TakeCareOfArithmeticOverflow::test_take_care_of_arithmetic_overflow();
    UseAlgorithms::test_use_algorithms();
    SafeDowncasting::test_safe_downcasting();
    DontUseNewExplicitely::test_dont_use_new_explicitely();
    GivePrimitiveDatatypesSemantics::test_use_string_literals();
    GivePrimitiveDatatypesSemantics::test_use_class_enums();
    GivePrimitiveDatatypesSemantics::test_give_primitive_datatypes_semantics();
    DeclareSingleArgumentConstructorsExplicit::test_declare_single_argument_constructors_explicit();
    UseOverride::test_use_override();
    UseConst::test_use_const();
    UseNodiscardAttribute::test_use_nodiscard();
}

// ===========================================================================
// End-of-File
// ===========================================================================

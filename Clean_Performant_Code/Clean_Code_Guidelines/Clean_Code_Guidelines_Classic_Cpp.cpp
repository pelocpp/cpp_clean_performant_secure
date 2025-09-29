// ===========================================================================
// Clean_Code_Guidelines_Classic_Cpp.cpp
// ===========================================================================

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <print>
#include <stdexcept>
#include <vector>

#if ! defined(_MSC_VER)
#include <experimental/propagate_const>
#endif

namespace CleanCodeGuidelines_ClassicCpp {

    namespace ClassVsStructInvariants {

        // =======================================================================
        // Invariants

        // https://www.geeksforgeeks.org/what-is-class-invariant/

        struct Point2D
        {
            int m_x;
            int m_y;

            Point2D() : Point2D{ 0, 0 } {}
            Point2D(int x, int y) : m_x{ x }, m_y{ y } {}
        };

        class Game
        {
        private:
            int     m_width;       // right bound 
            int     m_height;      // height bound 
            Point2D m_position;    // position of a figure on the game board

        public:
            Game(int width, int height, Point2D position)
                : m_width{}, m_height{}, m_position{ m_position }
            {
                // ensure that the object was constructed correctly 
                checkInvariant();
            }

            void moveTo(Point2D inc)
            {
                m_position.m_x += inc.m_x;
                m_position.m_y += inc.m_y;

                // ensure that the figure wasn't moved out of play bounds 
                checkInvariant();
            }

            // calling 'checkInvariant' is unnecessary because this is an accessor method 
            Point2D getLoc() const
            {
                return m_position;
            }

        private:
            void checkInvariant() const
            {
                if (!(m_position.m_x >= 0 && m_position.m_x <= m_width)) {
                    throw std::out_of_range("Position exceeds width of game board!");
                }
                 
                if (!(m_position.m_y >= 0 && m_position.m_y <= m_height)) {
                    throw std::out_of_range("Position exceeds height of game board!");
                }
            }
        };

        static void guidelines_invariants()
        {
            Game game{ 30, 20, Point2D{ 5, 5 } };

            // will throw assert error because the position is out of bounds
            game.moveTo(Point2D{ 20, 20 });

            // game is being spawned out of bounds 
            Game anotherGame{ 10, 10, Point2D{ 12, 8 } };
        }
    }

    namespace UseInferfaces {

        struct ICloneable
        {
            virtual ~ICloneable() {};
            virtual std::shared_ptr<ICloneable> clone() const = 0;
        };
    }

    namespace DefaultedConstructors {

        // =======================================================================
        // The new syntax "= default" in C++11

        class A
        {
        public:
            int m_x;
            A() {}
        };

        class B
        {
        public:
            int m_x;
            B() = default;
            B(int x) : m_x{ x } {}
        };

        static void guidelines_defaulted_constructor()
        {
            [[maybe_unused]] A a;
            [[maybe_unused]] B b;
        }
    }

    namespace InitializationOfStructs {

        namespace Variant_01 {

            struct Point2D
            {
                double m_x;
                double m_y;
            };

            static void test_variant_01() {

                Point2D point;
                std::println("x: {} - y: {}", point.m_x, point.m_y);
            }
        }

        namespace Variant_02 {

            struct Point2D
            {
                double m_x{};
                double m_y{};
            };

            static void test_variant_02() {

                Point2D point;

                point.m_x = 1.0;
                point.m_y = 2.0;

                Point2D copy{ point };   // automatically generated copy-c'tor

                Point2D anotherPoint;
                anotherPoint = copy;     // automatically generated assignment operator
            }
        }

        namespace Variant_03 {

            struct Point2D
            {
                double m_x{};
                double m_y{};

                Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
            };

            static void test_variant_03() {

                // Point2D point;  // error: does not compile
                Point2D anotherPoint{ 1.0, 2.0 };
            }
        }

        namespace Variant_04 {

            struct Point2D
            {
                double m_x;   // no more need for default initialization
                double m_y;   // no more need for default initialization

                Point2D() : m_x{}, m_y{} {}
                Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
            };

            static void test_variant_04() {

                Point2D point; 
                Point2D anotherPoint{ 1.0, 2.0 };
            }
        }

        namespace Variant_05 {

            struct Point2D
            {
                double m_x;   // no more need for default initialization
                double m_y;   // no more need for default initialization

                Point2D() : Point2D{ 0.0, 0.0 }  {}   // delegate work to another constructor
                Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
            };

            static void test_variant_05() {

                Point2D point;
                Point2D anotherPoint{ 1.0, 2.0 };
            }
        }

        namespace Variant_06 {

            struct Point2D
            {
                double m_x{};
                double m_y{};

                Point2D() = default;
                Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
            };

            static void test_variant_06() {

                Point2D point;
                Point2D anotherPoint{ 1.0, 2.0 };
            }
        }

        namespace Variant_07 {

            struct Point2D
            {
                double m_x;
                double m_y;
            };

            // taking advantage of struct Point2D beeing an 'aggregate' type:
            //   no user-declared constructors
            //   no inherited constructors
            //   no private non-static data members
            //   no virtual base classes
            //   ... some more issues

            static void test_variant_07() {

                Point2D point{};
                Point2D anotherPoint{ 1.0, 2.0 };
            }
        }
    
        static void guidelines_initialization_of_structs()
        {
            Variant_01::test_variant_01();
            Variant_02::test_variant_02();
            Variant_03::test_variant_03();
            Variant_04::test_variant_04();
            Variant_05::test_variant_05();
            Variant_06::test_variant_06();
            Variant_07::test_variant_07();
        }
    }

    namespace InitializationOfObjects {

        namespace Variant_01 {

            class SimpleString
            {
            private:
                char*       m_data{};   // pointer to the characters of the string (nullptr)
                std::size_t m_elems{};  // number of elements (zero)
            
            public:
                // c'tors / d'tor
                SimpleString() = default; // empty string

                SimpleString(const char* s)
                    : m_elems{ std::strlen(s) }
                {
                    m_data = new char[size() + 1];      // need space for terminating '\0'
                    std::copy(s, s + size(), m_data);
                    m_data[size()] = '\0';
                }

                ~SimpleString() {
                    delete[] m_data;
                }

                // getter
                std::size_t size() const { return m_elems; }
                bool empty() const { return size() == 0; }
                const char* data() const { return m_data; }

                // operators (no index-checking intentionally)
                char operator[](std::size_t n) const { return m_data[n]; }
                char& operator[](std::size_t n) { return m_data[n]; }
            };

            static void test_variant_01() {

                SimpleString s{ "Hello World" };
                std::println("{}", s.data());

                SimpleString s2;
                s2 = s;
                std::println("{}", s2.data());
            }
        }

        namespace Variant_02 {

            class SimpleString
            {
            private:
                char*       m_data{};   // pointer to the characters of the string (nullptr)
                std::size_t m_elems{};  // number of elements (zero)

            public:
                // c'tors / d'tor
                SimpleString() = default; // empty string

                SimpleString(const char* s)
                    : m_elems{ std::strlen(s) }
                {
                    m_data = new char[size() + 1];      // need space for terminating '\0'
                    std::copy(s, s + size(), m_data);
                    m_data[size()] = '\0';
                }

                // copy-constructor
                SimpleString(const SimpleString& other)
                    : m_data{ new char[other.size() + 1] }, m_elems{ other.size() }
                {
                    std::copy(other.m_data, other.m_data + other.size(), m_data);
                    m_data[size()] = '\0';
                }

                ~SimpleString() {
                    delete[] m_data;
                }

                // assignment operator
                SimpleString& operator=(const SimpleString& other)
                {
                    delete[] m_data;
                    m_data = new char[other.size() + 1];
                    std::copy(other.m_data, other.m_data + other.size(), m_data);
                    m_elems = other.size();
                    m_data[size()] = '\0';
                    return *this;
                }

                // getter
                std::size_t size() const { return m_elems; }
                bool empty() const { return size() == 0; }
                const char* data() const { return m_data; }

                // operators (no index-checking intentionally)
                char operator[](std::size_t n) const { return m_data[n]; }
                char& operator[](std::size_t n) { return m_data[n]; }
            };

            static void test_variant_02() {

                SimpleString s{ "Hello World" };
                std::println("{}", s.data());

                SimpleString s2{ s };
                std::println("{}", s2.data());
            }
        }

        namespace Variant_03 {

            class SimpleString
            {
            private:
                char*       m_data{};   // pointer to the characters of the string (nullptr)
                std::size_t m_elems{};  // number of elements (zero)

            public:
                // c'tors / d'tor
                SimpleString() = default; // empty string

                SimpleString(const char* s) 
                    : m_elems{ std::strlen(s) }
                {
                    m_data = new char[size() + 1];      // need space for terminating '\0'
                    std::copy(s, s + size(), m_data);
                    m_data[size()] = '\0';
                }

                // copy-constructor
                SimpleString(const SimpleString& other)
                    : m_data{ new char[other.size() + 1] }, m_elems{ other.size() }
                {
                    std::copy(other.m_data, other.m_data + other.size(), m_data);
                    m_data[size()] = '\0';
                }

                ~SimpleString() {
                    delete[] m_data;
                }

                // assignment operator
                SimpleString& operator=(const SimpleString& other)
                {
                    char* tmp = new char[other.size() + 1];
                    delete[] m_data;
                    m_data = tmp;
                    std::copy(other.m_data, other.m_data + other.size(), m_data);
                    m_elems = other.size();
                    m_data[size()] = '\0';
                    return *this;
                }

                // getter
                std::size_t size() const { return m_elems; }
                bool empty() const { return size() == 0; }
                const char* data() const { return m_data; }

                // operators (no index-checking intentionally)
                char operator[](std::size_t n) const { return m_data[n]; }
                char& operator[](std::size_t n) { return m_data[n]; }
            };

            static void test_variant_03() {

                SimpleString s{ "Hello World" };
                std::println("{}", s.data());

                SimpleString s2;
                s2 = s;
                std::println("{}", s2.data());
            }
        }

        namespace Variant_04 {

            class SimpleString
            {
            private:
                char*       m_data{};   // pointer to the characters of the string (nullptr)
                std::size_t m_elems{};  // number of elements (zero)

            public:
                // c'tors / d'tor
                SimpleString() = default; // empty string

                SimpleString(const char* s)
                    : m_elems{ std::strlen(s) }
                {
                    m_data = new char[size() + 1];      // need space for terminating '\0'
                    std::copy(s, s + size(), m_data);
                    m_data[size()] = '\0';
                }

                // copy-constructor
                SimpleString(const SimpleString& other)
                    : m_data{ new char[other.size() + 1] }, m_elems{ other.size() }
                {
                    std::copy(other.m_data, other.m_data + other.size(), m_data);
                    m_data[size()] = '\0';
                }

                ~SimpleString() {
                    delete[] m_data;
                }

                // assignment operator
                SimpleString& operator=(const SimpleString& other)
                {
                    // prevent self-assignment
                    if (this == &other) {
                        return *this;
                    }

                    char* tmp = new char[other.size() + 1];
                    delete[] m_data;
                    m_data = tmp;
                    std::copy(other.m_data, other.m_data + other.size(), m_data);
                    m_elems = other.size();
                    m_data[size()] = '\0';
                    return *this;
                }

                // getter
                std::size_t size() const { return m_elems; }
                bool empty() const { return size() == 0; }
                const char* data() const { return m_data; }

                // operators (no index-checking intentionally)
                char operator[](std::size_t n) const { return m_data[n]; }
                char& operator[](std::size_t n) { return m_data[n]; }
            };

            static void test_variant_04() {

                SimpleString s{ "Hello World" };
                s = s;
                std::println("{}", s.data());
            }
        }

        static void guidelines_initialization_of_objects()
        {
            // Variant_01::test_variant_01();  // crashes intentionally
            Variant_02::test_variant_02();
            Variant_03::test_variant_03();
            Variant_04::test_variant_04();
        }
    }

    namespace SmallFocusedFunctions {

        // =======================================================================
        // Write Small, Focused Functions

        static bool isValidUserName(const std::string& username) {

            const auto MinLength{ 8 };
            const auto MaxLength{ 30 };

            const std::string ValidCharacters{
                "abcdefghijklmnopqrstuvwxyz0123456789_-."
            };

            if (username.length() < MinLength || username.length() > MaxLength) {
                return false;
            }

            for (char ch : username) {
                if (ValidCharacters.find(ch) == std::string::npos) {
                    return false;
                }
            }

            return true;
        }

        static void guidelines_small_focused_functions()
        {
            std::string user{ "jack1980" };
            auto isValid{ isValidUserName(user) };
        }
    }

    namespace UseConstLiberally{

        // =======================================================================
        // Use 'const' liberally

        class Rectangle
        {
        private:
            double m_width;
            double m_height;

        public:
            Rectangle(double width, double height)
                : m_width{ width }, m_height{ height }
            {
            }

            double area() const {
                return  m_width * m_height;
            }

            void scale(double factor) {
                m_width *= factor;
                m_height *= factor;
            }
        };

        static void printArea(const Rectangle& rect) {
            std::println("Area: {}", rect.area());
        }

        static void guidelines_use_const_liberally()
        {
            Rectangle rect{ 10.0, 20.0 };
            printArea(rect);
        }
    }

    namespace PreferExceptionsOverErrorCodes {

        // =======================================================================
        // Prefer Exceptions over Error Codes

        static void error_handling_01() {

            try {
                std::ifstream file("data.txt");

                if (!file.is_open()) {
                    throw std::runtime_error("Could not open file");
                }

                // Process the file
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        static void error_handling_02() {

            std::ifstream file("data.txt");

            if (!file.is_open()) {
                // Handle the error, maybe return an error code  
            }

            // Process the file
        }

        static void guidelines_error_handling()
        {
            error_handling_01();
            error_handling_02();
        }
    }

    namespace MethodsReturnType_CopiedToCaller {

        // =======================================================================
        // Return Type of Methods: By Copy
         
        class Person
        {
        private:
            std::string   m_name;
            size_t        m_age;

        public:
            Person() = default;
            Person(const std::string& name, size_t age)
                : m_name{ name }, m_age{ age }
            {}

            std::string getName() const { return m_name; }
            size_t getAge() const { return m_age; }
        };

        static void guidelines_return_type() {
            Person jack{ "Jack", 50 };
            std::string name{ jack.getName() };
            std::println("Name: {}", name);
        }
    }

    namespace MethodsReturnType_LifetimeByVoucher {

        // =======================================================================
        // Return Type of Methods: By Reference

        class Person
        {
        private:
            std::string   m_name;
            size_t        m_age;

        public:
            Person() = default;
            Person(const std::string& name, size_t age)
                : m_name{ name }, m_age{ age }
            {}

            const std::string& getName() const { return m_name; }
            size_t getAge() const { return m_age; }
        };

        static void guidelines_return_type() {
            Person jack{ "Jack", 50 };
            const std::string& name{ jack.getName() };
            std::println("Name: {}", name);
        }
    }

    namespace CompositionOverInheritance {

        // =======================================================================
        // Prefer Composition over Inheritance
         
        class Vector3D{};

        class Transform
        {
            Vector3D m_position;
            Vector3D m_rotation;
            Vector3D m_scale;
        };

        class Character
        {
        public:
            // functions that might use *m_transform* if they need it

        private:
            Transform m_transform;
        };

        class Obstacle
        {
        public:
            // functions that might use *m_transform* if they need it

        private:
            Transform m_transform;
        };

        static void guidelines_prefer_composition_over_inheritance() {}
    }

    // =======================================================================
    // Prevent Implicit Conversions

    namespace PreventImplicitConversions {

        // demonstrating implicit conversion
        class Distance
        {
        private:
            int m_meters;

        public:
            /*explicit*/ Distance(int meters)             // put explicit into comment / remove comment
                : m_meters{ meters }
            {}

            void display() const {
                std::println("Distance: {}", m_meters);
            }
        };

        static void printDistance(const Distance& d) {
            d.display();
        }

        static void guidelines_implicit_conversion()
        {
            auto meters{ 10 };
            printDistance(meters); // implicit conversion: int ==> Distance
            // or
            // printDistance(static_cast<Distance>(meters));  
        }
    }

    namespace IfForStatementWithInitializer {

        static void guidelines_if_statement_with_initializer()
        {
            std::map<int, std::string> m;

            if (auto it = m.find(10); it != m.end()) {
                auto size = it->second.size();
            }
        }

        static void guidelines_for_statement_with_initializer()
        {
            std::vector<int> numbers = { 1, 2, 3, 4, 5 };

            for (int i{}; auto n : numbers) {

                std::println("{:02}: {}", i, n);
                ++i;
            }
        }

        static void guidelines_if_for_statement_with_initializer()
        {
            guidelines_if_statement_with_initializer();
            guidelines_for_statement_with_initializer();
        }
    }
}

void clean_code_guidelines_classic_cpp()
{
    CleanCodeGuidelines_ClassicCpp::ClassVsStructInvariants::guidelines_invariants();    // crashes intentionally
    CleanCodeGuidelines_ClassicCpp::DefaultedConstructors::guidelines_defaulted_constructor();
    CleanCodeGuidelines_ClassicCpp::InitializationOfStructs::guidelines_initialization_of_structs();
    CleanCodeGuidelines_ClassicCpp::InitializationOfObjects::guidelines_initialization_of_objects();
    CleanCodeGuidelines_ClassicCpp::SmallFocusedFunctions::guidelines_small_focused_functions();
    CleanCodeGuidelines_ClassicCpp::UseConstLiberally::guidelines_use_const_liberally();
    CleanCodeGuidelines_ClassicCpp::PreferExceptionsOverErrorCodes::guidelines_error_handling();
    CleanCodeGuidelines_ClassicCpp::MethodsReturnType_CopiedToCaller::guidelines_return_type();
    CleanCodeGuidelines_ClassicCpp::MethodsReturnType_LifetimeByVoucher::guidelines_return_type();
    CleanCodeGuidelines_ClassicCpp::CompositionOverInheritance::guidelines_prefer_composition_over_inheritance();
    CleanCodeGuidelines_ClassicCpp::PreventImplicitConversions::guidelines_implicit_conversion();
    CleanCodeGuidelines_ClassicCpp::IfForStatementWithInitializer::guidelines_if_for_statement_with_initializer();
}

// ===========================================================================
// End-of-File
// ===========================================================================

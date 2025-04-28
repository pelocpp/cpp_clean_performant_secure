// ===========================================================================
// Guidelines_Core_Cpp.cpp
// ===========================================================================

#include <algorithm> 
#include <cstddef>
#include <cstring> 
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex> 
#include <print>
#include <stdexcept>
#include <utility>
#include <vector>

#if ! defined(_MSC_VER)
#include <experimental/propagate_const>
#endif

namespace GuidelinesCoreCpp {

    namespace GuidelinesCoreCpp_ClassVsStructInvariants {

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

    namespace GuidelinesCoreCpp_UseInferfaces {

        struct ICloneable
        {
            virtual ~ICloneable() {};
            virtual std::shared_ptr<ICloneable> clone() const = 0;
        };
    }

    namespace GuidelinesCoreCpp_DefaultedConstructors {

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

    namespace GuidelinesCoreCpp_InitializationOfStructs {

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

    namespace GuidelinesCoreCpp_InitializationOfObjects {

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

    namespace GuidelinesCoreCpp_CopySwapIdiom {

        namespace Variant_01 {

            class SimpleString
            {
            private:
                char* m_data{};   // pointer to the characters of the string (nullptr)
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

                // swap-idiom
                void swap(SimpleString& other) noexcept
                {
                    std::swap(m_data, other.m_data);      // swap data member
                    std::swap(m_elems, other.m_elems);    // swap data member
                }

                // refined copy assignment operator
                SimpleString& operator=(SimpleString other) {

                    this->swap(other);
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

            static void test_variant_01() {

                SimpleString s1{ "Hello World" };
                SimpleString s2{ "More Hello World" };
                s1 = s2;
                std::println("s1: {}", s1.data());
                std::println("s2: {}", s2.data());
            }
        }

        static void guidelines_copy_swap_idiom()
        {
            Variant_01::test_variant_01();
        }
    }

    namespace GuidelinesCoreCpp_MoveOperations {

        namespace Variant_01 {

            class SimpleString
            {
            private:
                char* m_data{};   // pointer to the characters of the string (nullptr)
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

                // swap-idiom
                void swap(SimpleString& other) noexcept
                {
                    std::swap(m_data, other.m_data);      // swap data member
                    std::swap(m_elems, other.m_elems);    // swap data member
                }

                // refined copy assignment operator
                SimpleString& operator=(SimpleString other) {

                    this->swap(other);
                    return *this;
                }

                // move operations
                //SimpleString(SimpleString&& other) noexcept
                //{
                //    m_data = std::move(other.m_data);
                //    m_elems = std::move(other.m_elems);
                //    other.m_data = nullptr;
                //    other.m_elems = 0;
                //}

                // Alternative:
                SimpleString(SimpleString&& other) noexcept
                {
                    m_data = std::exchange(other.m_data,  nullptr);
                    m_elems = std::exchange(other.m_elems, 0);
                }

                SimpleString& operator=(SimpleString&& other) noexcept {

                    SimpleString tmp{ std::move(other) };
                    tmp.swap(*this);
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

            static void test_variant_01() {

                SimpleString s1{ "Hello World" };
                SimpleString s2{ "More Hello World" };
                s1 = s2;
                std::println("s1: {}", s1.data());
                std::println("s2: {}", s2.data());
            }
        }

        static void guidelines_move_operations()
        {
            Variant_01::test_variant_01();
        }
    }

    namespace GuidelinesCoreCpp_SmallFocusedFunctions {

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

    namespace GuidelinesCoreCpp_UseConstLiberally{

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

    namespace GuidelinesCoreCpp_PreferExceptionsOverErrorCodes {

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

    namespace GuidelinesCoreCpp_MethodsReturnType_CopiedToCaller {

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

    namespace GuidelinesCoreCpp_MethodsReturnType_LifetimeByVoucher {

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

    namespace GuidelinesCoreCpp_CompositionOverInheritance {

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

    namespace GuidelinesCoreCpp_PreventImplicitConversions {

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

    namespace GuidelinesCoreCpp_Keyword_Auto {

        // =======================================================================
        // Using Keyword 'auto'
        // First Example
         
        static void guidelines_keyword_auto_01()
        {
            std::map<int, std::string> aMap{ { 1, "Hello"  } };

            std::map<int, std::string>::iterator it{ aMap.begin() };

            // std::pair<int, std::string>& entry1{ *it };  // Why this line DOES NOT compile ???

            auto& entry2{ *it };
        }

        class Person
        {
        private:
            std::string   m_name;

        public:
            Person() = default;
            Person(const std::string& name) : m_name{ name } {}

            const std::string& getName() const { return m_name; }
        };

        static void guidelines_keyword_auto_02()
        {
            Person jack{ "Jack" };
            auto name = jack.getName();
        }

        // =======================================================================
        // Using Keyword 'auto'
        // Second Example

        static void guidelines_keyword_auto_03()
        {
            std::vector<int> numbers{ 1, 2, 3, 4, 5 };

            auto print = [](int value) { std::print("{} ", value); };

            std::function<void(int)> morePrint = [](int value) { std::print("{} ", value); };

            std::for_each(
                numbers.begin(),
                numbers.end(),
                [](int value) { std::print("{} ", value); }
            );

            std::println();

            std::for_each(
                numbers.begin(),
                numbers.end(),
                print
            );

            std::println();

            std::for_each(
                numbers.begin(),
                numbers.end(),
                morePrint
            );
        }

        static void guidelines_keyword_auto()
        {
            guidelines_keyword_auto_01();
            guidelines_keyword_auto_02();
            guidelines_keyword_auto_03();
        }
    }

    namespace GuidelinesCoreCpp_Keyword_Auto_Left_to_Right_Initialization_Syntax {

        // =======================================================================
        // Left to Right Initialization Syntax using keyword 'auto'

        class X {};

        class Foo{
        public:
            Foo() = default;
            Foo(int) {};
        };

        static Foo createFooOject() { return {}; }

        static void guidelines_keyword_auto_left_to_right_initialization_syntax()
        {
            auto var1 = 0;
            auto var2 = Foo{};
            auto var3 = createFooOject();
            auto var4 = std::mutex{};

            auto name = std::string{ "Hans" };
            auto anotherName = "Sepp";

            auto ptr = std::make_unique<Foo>(123);

            auto myLambda = [](auto n, auto m) { return n + m; };

            //X x1();
            // auto x2 = X();
        }
    }

    namespace GuidelinesCoreCpp_Keyword_Const_Auto_References {

        static std::pair<int, int> func(int x) { 
            return{ x, x }; 
        }

        static auto another_func() {

            const std::pair<int, int>& cv1 = func(1);  // temporary object of type std::pair<int,int>
            const std::pair<int, int>& cv2 = func(2);  // temporary object of type std::pair<int,int>
            const std::pair<int, int>& cv3 = func(3);  // temporary object of type std::pair<int,int>

            // Temporary Lifetime Extension: 
            return cv1.first + cv2.first + cv3.first;;
            // cv1, cv2 and cv3 are going out of scope, temporaries will be destroyed right now
        }

        static void guidelines_keyword_const_auto_references_01()
        {
            auto result = another_func();
            std::println("Value: {}", result);
        }

        // --------------------------------------------------------------------------------------

        // class defined with new syntax with auto
        //class Foo
        //{
        //public:
        //    auto val() const {
        //        return m_value;
        //    }
        //    auto& cref() const {
        //        return m_value;
        //    }
        //    auto& mref() {
        //        return m_value;
        //    }

        //private:
        //    int m_value{};
        //};

        // class defined with traditional syntax with explicit type
        class Foo
        {
        public:
            int val() const {
                return m_value;
            }

            const int& cref() const {
                return m_value;
            }

            int& mref() {
                return m_value;
            }

        private:
            int m_value{};
        };

        static void guidelines_keyword_const_auto_references_02()
        {
            auto foo = Foo{};
            auto& cref = foo.cref(); // cref is a const reference
            auto& mref = foo.mref(); // mref is a mutable reference
        }

        static void guidelines_keyword_const_auto_references()
        {
            guidelines_keyword_const_auto_references_01();
            guidelines_keyword_const_auto_references_02();
        }
    }

    namespace GuidelinesCoreCpp_Const_Propagation_for_Pointer {

        class Foo {
        public:
            Foo(int* ptr) : m_ptr{ ptr } {}

            auto set_value_behind_pointer(int value) const {

                *m_ptr = value;  // compiles despite function being declared const!
            }

            auto print() const
            {
                std::println("{}", *m_ptr);
            }

        private:
            int* m_ptr{};
        };

#if ! defined(_MSC_VER)
        class FooImproved {
        public:
            FooImproved(int* ptr) : m_ptr{ ptr } {}

            auto set_ptr(int* ptr) const
            {
                m_ptr = ptr;             // Will not compile, as expected
            }

            auto set_val(int value) const 
            {
                m_value = value;         // Will not compile, as expected
            }

            auto print() const
            {
                std::println("{}", *m_ptr);
            }

            auto set_value_behind_pointer(int value) const
            {
                *m_ptr = value;              // Will not compile, const is propagated
            }

        private:
            std::experimental::propagate_const<int*> m_ptr{ nullptr };
            int m_value{};
        };
#endif

        static void guidelines_keyword_const_propagation_for_pointers_01()
        {
            auto i = 0;

            const auto foo = Foo{ &i };

            foo.print();
            foo.set_value_behind_pointer(123);
            foo.print();
        }

#if ! defined(_MSC_VER)
        static void guidelines_keyword_const_propagation_for_pointers_02()
        {
            auto i = 0;

            const auto foo = FooImproved{ &i };

            foo.print();
            foo.set_value_behind_pointer(123);
            foo.print();
        }
#endif

        static void guidelines_keyword_const_propagation_for_pointers()
        {
            guidelines_keyword_const_propagation_for_pointers_01();

#if ! defined(_MSC_VER)
            guidelines_keyword_const_propagation_for_pointers_02();
#endif
        }

// #endif
    }

    namespace GuidelinesCoreCpp_StrongTyping {

        enum class Suit
        {
            Hearts,   // Herz
            Diamonds, // Karo
            Clubs,    // Kreuz
            Spades    // Pik
        };

        static std::string to_string(const Suit& suit)
        {
            using namespace std::literals;

            switch (suit)
            {
            case Suit::Hearts:
                return "Hearts"s;
            case Suit::Diamonds:
                return "Diamonds"s;
            case Suit::Clubs:
                return "Clubs"s;
            case Suit::Spades:
                return "Spades"s;
            default:
                return "?"s;
            }
        }

        namespace Example_Without_StrongTyping {

            struct Card
            {
                int value{};
                Suit suit{};
            };

            static std::ostream& operator<< (std::ostream& os, const Card& card)
            {
                os << to_string(card.suit) << " " << card.value;
                return os;
            }

            static void guidelines_strong_typing()
            {
                Card card{ 2, Suit::Diamonds };
                std::cout << card << std::endl;
            }
        }

        namespace Example_With_StrongTyping {

            class FaceValue
            {
            private:
                int m_value;

            public:
                explicit FaceValue(int value) : m_value{ value }
                {
                    if (m_value < 7 || m_value > 14)
                    {
                        throw std::invalid_argument("Face value invalid");
                    }
                }

                int value() const
                {
                    return m_value;
                }
            };

            class Card
            {
            private:
                FaceValue m_value{ 7 };
                Suit m_suit{};

            public:
                Card() = default;
                Card(FaceValue value, Suit suit) : m_value{ value }, m_suit{ suit } {}

                FaceValue value() const { return m_value; }
                Suit suit() const { return m_suit; }
            };

            static std::ostream& operator<< (std::ostream& os, const FaceValue& faceValue)
            {
                int value{ faceValue.value() };
                if (value >= 7 && value <= 10) {
                    os << value;
                }
                else
                {
                    switch (value)
                    {
                    case 11:
                        os << "Jack";   // Bube
                        break;
                    case 12:
                        os << "Queen";  // Dame
                        break;
                    case 13:
                        os << "King";   // K�nig
                        break;
                    case 14:
                        os << "Ace";    // Ass
                        break;
                    }
                }

                return os;
            }

            static std::ostream& operator<< (std::ostream& os, const Card& card)
            {
                os << to_string(card.suit()) << " " << card.value();
                return os;
            }
            
            static void guidelines_strong_typing()
            {
                // Card card{ 2, Suit::Diamonds };              // does not compile (!!!)
                
                Card defaultCard{};                             // compiles and runs
                std::cout << defaultCard << std::endl;

                Card pikAss{ FaceValue{ 14 }, Suit::Spades };   // compiles and runs
                std::cout << pikAss << std::endl;
            }
        }

        static void guidelines_keyword_const_auto_references()
        {
            Example_Without_StrongTyping::guidelines_strong_typing();
            Example_With_StrongTyping::guidelines_strong_typing();
        }
    }
}

void guidelines_core_cpp()
{
    using namespace GuidelinesCoreCpp;

    // GuidelinesCoreCpp_ClassVsStructInvariants::guidelines_invariants();  // crashes intentionally
    GuidelinesCoreCpp_DefaultedConstructors::guidelines_defaulted_constructor();
    GuidelinesCoreCpp_InitializationOfStructs::guidelines_initialization_of_structs();
    GuidelinesCoreCpp_InitializationOfObjects::guidelines_initialization_of_objects();
    GuidelinesCoreCpp_CopySwapIdiom::guidelines_copy_swap_idiom();
    GuidelinesCoreCpp_SmallFocusedFunctions::guidelines_small_focused_functions();
    GuidelinesCoreCpp_UseConstLiberally::guidelines_use_const_liberally();
    GuidelinesCoreCpp_PreferExceptionsOverErrorCodes::guidelines_error_handling();
    GuidelinesCoreCpp_MethodsReturnType_CopiedToCaller::guidelines_return_type();
    GuidelinesCoreCpp_MethodsReturnType_LifetimeByVoucher::guidelines_return_type();
    GuidelinesCoreCpp_CompositionOverInheritance::guidelines_prefer_composition_over_inheritance();
    GuidelinesCoreCpp_PreventImplicitConversions::guidelines_implicit_conversion();
    GuidelinesCoreCpp_Keyword_Auto::guidelines_keyword_auto();
    GuidelinesCoreCpp_Keyword_Auto_Left_to_Right_Initialization_Syntax::guidelines_keyword_auto_left_to_right_initialization_syntax();
    GuidelinesCoreCpp_Keyword_Const_Auto_References::guidelines_keyword_const_auto_references();
    GuidelinesCoreCpp_Const_Propagation_for_Pointer::guidelines_keyword_const_propagation_for_pointers();
    GuidelinesCoreCpp_StrongTyping::guidelines_keyword_const_auto_references();
}

// ===========================================================================
// End-of-File
// ===========================================================================

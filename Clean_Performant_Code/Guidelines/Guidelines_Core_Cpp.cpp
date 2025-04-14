// ===========================================================================
// Guidelines_Core_Cpp.cpp
// ===========================================================================

#include <fstream>
#include <iostream>
#include <map>
#include <print>
#include <stdexcept>

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

                //assert(m_position.m_x >= 0 && m_position.m_x <= m_width);
                //assert(m_position.m_y >= 0 && m_position.m_y <= m_height);
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
            A a;
            B b;
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

                Point2D() : m_x{  }, m_y{  } {}
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
            auto& name = jack.getName();
        }

        static void guidelines_keyword_auto()
        {
            guidelines_keyword_auto_01();
            guidelines_keyword_auto_02();
        }
    }
}

void guidelines_core_cpp()
{
    using namespace GuidelinesCoreCpp;

    // GuidelinesCoreCpp_ClassVsStructInvariants::guidelines_invariants();  // crashes intentionally
    //GuidelinesCoreCpp_DefaultedConstructors::guidelines_defaulted_constructor();


    GuidelinesCoreCpp_InitializationOfStructs::guidelines_initialization_of_structs();

    //GuidelinesCoreCpp_SmallFocusedFunctions::guidelines_small_focused_functions();
    //GuidelinesCoreCpp_UseConstLiberally::guidelines_use_const_liberally();
    //GuidelinesCoreCpp_PreferExceptionsOverErrorCodes::guidelines_error_handling();
    //GuidelinesCoreCpp_MethodsReturnType_CopiedToCaller::guidelines_return_type();
    //GuidelinesCoreCpp_MethodsReturnType_LifetimeByVoucher::guidelines_return_type();
    //GuidelinesCoreCpp_CompositionOverInheritance::guidelines_prefer_composition_over_inheritance();
    //GuidelinesCoreCpp_PreventImplicitConversions::guidelines_implicit_conversion();
    //GuidelinesCoreCpp_Keyword_Auto::guidelines_keyword_auto();
}

// ===========================================================================
// End-of-File
// ===========================================================================

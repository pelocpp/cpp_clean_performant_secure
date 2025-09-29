// ===========================================================================
// Clean_Code_Guidelines_Modern_Cpp.cpp
// ===========================================================================

#include <algorithm> 
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex> 
#include <print>
#include <string>

namespace CleanCodeGuidelines_ModernCpp {

    namespace Keyword_Auto {

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

    namespace Keyword_Auto_Left_to_Right_Initialization_Syntax {

        // =======================================================================
        // Left to Right Initialization Syntax using keyword 'auto'

        class X {};

        class Foo {
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

    namespace Keyword_Const_Auto_References {

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
        class Foo
        {
        public:
            auto val() const {
                return m_value;
            }
            auto& cref() const {
                return m_value;
            }
            auto& mref() {
                return m_value;
            }

        private:
            int m_value{};
        };

        // class defined with traditional syntax with explicit type
        //class Foo
        //{
        //public:
        //    int val() const {
        //        return m_value;
        //    }

        //    const int& cref() const {
        //        return m_value;
        //    }

        //    int& mref() {
        //        return m_value;
        //    }

        //private:
        //    int m_value{};
        //};

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

    namespace CopySwapIdiom {

        namespace Variant_01 {

            class SimpleString
            {
            private:
                char* m_data{};         // pointer to the characters of the string (nullptr)
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

    namespace MoveOperations {

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
                    m_data = std::exchange(other.m_data, nullptr);
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


    namespace Const_Propagation_for_Pointer {

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
    }

    namespace StrongTyping {

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
                        os << "King";   // König
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

void clean_code_guidelines_modern_cpp()
{
    CleanCodeGuidelines_ModernCpp::CopySwapIdiom::guidelines_copy_swap_idiom();
    CleanCodeGuidelines_ModernCpp::Keyword_Auto::guidelines_keyword_auto();
    CleanCodeGuidelines_ModernCpp::Keyword_Auto_Left_to_Right_Initialization_Syntax::guidelines_keyword_auto_left_to_right_initialization_syntax();
    CleanCodeGuidelines_ModernCpp::Keyword_Const_Auto_References::guidelines_keyword_const_auto_references();
    CleanCodeGuidelines_ModernCpp::Const_Propagation_for_Pointer::guidelines_keyword_const_propagation_for_pointers();
    CleanCodeGuidelines_ModernCpp::StrongTyping::guidelines_keyword_const_auto_references();
}

// ===========================================================================
// End-of-File
// ===========================================================================

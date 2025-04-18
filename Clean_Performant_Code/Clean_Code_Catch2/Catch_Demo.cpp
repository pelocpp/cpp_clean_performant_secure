// ===========================================================================
// Catch_Demo.cpp / Catch2 Demo
// ===========================================================================

#include <iostream>

#include "Catch2/catch.hpp"

unsigned int Factorial(unsigned int number) {
    return number <= 1 ? number : Factorial(number - 1) * number;
}

// ===============================================================
// test caces
// note: requires vs. check
 
TEST_CASE ("Factorials are computed", "[factorial]") {

    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(4) == 24);
    REQUIRE(Factorial(5) == 120);

    CHECK(Factorial(1) == 1);
    CHECK(Factorial(2) == 3);
    CHECK(Factorial(3) == 6);
    CHECK(Factorial(4) == 24);
    CHECK(Factorial(5) == 120);
}

// ===============================================================
// note: use of tag(s)

TEST_CASE("More Factorials are computed", "[factorial]") {

    CHECK(Factorial(6)  == 720);
    CHECK(Factorial(7)  == 5040);
    CHECK(Factorial(8)  == 40320);
    CHECK(Factorial(9)  == 362'880);
    CHECK(Factorial(10) == 3'628'800);
}

// ===============================================================
// note: use of sections
// note: there is a nested section

TEST_CASE("vectors can be sized and resized", "[vector]") {

    std::cout << "Setting up Test Case: " << std::endl;

    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SECTION("resizing bigger changes size and capacity") {
        v.resize(10);

        std::cout << "Section 1 - resize" << std::endl;

        REQUIRE(v.size() == 10);
        CHECK(v.size() == 10);   // test with another value
        REQUIRE(v.capacity() >= 10);
    }

    SECTION("resizing smaller changes size but not capacity") {
        v.resize(0);

        std::cout << "Section 2 - no shrink_to_fit" << std::endl;

        // shrink_to_fit <=================

        REQUIRE(v.size() == 0);
        
        REQUIRE(v.capacity() >= 5);
    }

    SECTION("reserving bigger changes capacity but not size") {
        
        v.reserve(10);   // resize ()

        std::cout << "Section 3" << std::endl;

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 10);

        // added
        SECTION("reserving smaller again does not change capacity") {
            
            v.reserve(7);

            REQUIRE(v.capacity() >= 10);
        }
    }

    SECTION("reserving smaller does not change size or capacity") {
        
        v.reserve(0);

        std::cout << "Section 4 - once again reserve" << std::endl;

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
    }
}

// ===============================================================
// note: behaviour driven development stype testing

SCENARIO("bdd vectors can be sized and resized", "[bdd_vector]") {

    GIVEN("A vector with some items") {

        std::vector<int> v(5);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);

        WHEN("the size is increased") {
            v.resize(10);

            THEN("the size and capacity change") {
                REQUIRE(v.size() == 10);
                REQUIRE(v.capacity() >= 10);
            }
        }
        WHEN("the size is reduced") {
            v.resize(0);

            THEN("the size changes but not capacity") {
                REQUIRE(v.size() == 10);
                REQUIRE(v.capacity() >= 5);
            }
        }
        WHEN("more capacity is reserved") {
            v.reserve(10);

            THEN("the capacity changes but not the size") {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 10);
            }
        }
        WHEN("less capacity is reserved") {
            v.reserve(0);

            THEN("neither size nor capacity are changed") {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 5);
            }
        }
    }
}

// ===============================================================
// note: test case with parameters
// parameter has name 'TestType'

TEMPLATE_TEST_CASE("templated vectors", "[simple_template]", int, std::string, (std::tuple<int, float>)) {

    std::vector<TestType> vec;

    REQUIRE(vec.size() == 0);

    TestType elem{};   // default c'tor

    vec.push_back(elem);
    REQUIRE(vec.size() == 1);
}

// ===============================================================
// note: test case with parameters
// parameter has name 'TestType'

TEMPLATE_TEST_CASE("more vectors", "[template]", int, std::string, (std::tuple<int, float>)) {

    std::vector<TestType> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SECTION("resizing bigger changes size and capacity") {
        v.resize(10);

        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= 10);
    }
    SECTION("resizing smaller changes size but not capacity") {
        v.resize(0);

        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 5);

        SECTION("We can use the 'swap trick' to reset the capacity") {
            std::vector<TestType> empty;
            empty.swap(v);

            REQUIRE(v.capacity() == 0);
        }
    }
    SECTION("reserving smaller does not change size or capacity") {
        v.reserve(0);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
    }
}

// ===============================================================
// matchers

TEST_CASE("Bugs, bugs, bugs", "[Bug]")
{
    std::string str1 = "Bugs as a service";
    std::string str2 = "C++ as a service";
    std::string str3 = "C++ is no service";

    auto match_expression = Catch::EndsWith("as a service");

    REQUIRE_THAT(str1, match_expression);
    REQUIRE_THAT(str2, match_expression);
    REQUIRE_THAT(str3, match_expression);
}

TEST_CASE("More Bugs, bugs, bugs", "[Checked_Bug]")
{
    std::string str = "Bugs as a service";

    auto match_expression1 = Catch::EndsWith("as a service");
    CHECK_THAT(str, match_expression1);

    //auto match_expression2 = Catch::EndsWith("as a matcher");
    //CHECK_THAT(str, match_expression2);
}

TEST_CASE("Floating Point Arithmetic", "[Floating]")
{
    float f = 123.0F;

    auto floatMatcher = Catch::WithinAbs(f, 0.00001F);

    CHECK_THAT(f, floatMatcher);

    f += 0.5;
    CHECK_THAT(f, floatMatcher);
}

// ===============================================================

TEST_CASE("Sporadic Bug", "[Sporadic_Bug]")
{

   // for (int i = 0; ............)
    // Programm ganz normal
    // ......................................


    CHECK(Factorial(5) == 120);
}

// ===========================================================================
// End-of-File
// ===========================================================================

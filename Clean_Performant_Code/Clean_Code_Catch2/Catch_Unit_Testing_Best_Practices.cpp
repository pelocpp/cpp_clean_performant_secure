// ===========================================================================
// Catch_Unit_Testing_Best_Practices.cpp / Unit Testing Best Practices
// ===========================================================================

// https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-best-practices
//
// https://medium.com/@kaanfurkanc/unit-testing-best-practices-3a8b0ddd88b5
//
// https://www.ibm.com/think/insights/unit-testing-best-practices
//
//
// https://stackoverflow.com/questions/3459287/whats-the-difference-between-a-mock-stub
//
//

#include "Catch2/catch.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>

// ===========================================================================
// Follow test naming standards

static bool IsPrime(size_t number)
{
    if (number <= 2) {
        return number == 2;
    }

    if (number % 2 == 0) {
        return false;
    }

    // check odd divisors from 3 to the square root of the number
    size_t end{ static_cast<size_t>(std::ceil(std::sqrt(number))) };
    for (size_t i{ 3 }; i <= end; i += 2) {

        if (number % i == 0) {
            return false;  // number not prime
        }
    }

    return true; // found prime number
}

TEST_CASE("IsPrime // checks if argument is a prime number // returns true", "[IsPrime__ChecksIfNumberIsPrime__Returns_True]")
{
    size_t number = 5;
    auto expected = true;
    auto actual = IsPrime(number);
    CHECK(expected == actual);

    number = 11;
    expected = true;
    actual = IsPrime(number);
    CHECK(expected == actual);

    number = 101;
    expected = true;
    actual = IsPrime(number);
    CHECK(expected == actual);
}

// ===========================================================================
// Use AAA Pattern

TEST_CASE("More IsPrime // checks if argument is a prime number // returns true", "[IsPrime__Demonstrate_AAA_Pattern__Returns_True]")
{
    // arrange
    size_t number = 5;
    auto expected = true;

    // act
    auto actual = IsPrime(number);

    // assert
    CHECK(expected == actual);
}

// ===========================================================================
// Mocks and Stubs

class User
{
private:
    std::string m_name;

public:
    explicit User(const std::string& name) : m_name(name) {}

    const std::string& getName() const { return m_name; }
};

class UserRepository
{
public:
    virtual ~UserRepository() = default;
    virtual std::shared_ptr<User> findById(int id) = 0;
    virtual void save(const User& user) = 0;
};

class UserService
{
private:
    UserRepository& m_repo;

public:
    explicit UserService(UserRepository& repo) : m_repo(repo) {}

    std::string getUserName(int id)
    {
        auto user = m_repo.findById(id);
        return (user != nullptr) ? user->getName() : "";
    }

    void registerUser(const std::string& name) {
        m_repo.save(User{ name });
    }
};

// ---------------------------------------------------------------------------
// Implementing a Stub

class UserRepositoryStub : public UserRepository
{
public:
    std::shared_ptr<User> findById(int id) override {
        return std::make_shared<User>("Alice");  // there is always "Alice" returned
    }

    void save(const User& user) override {
        // leave blank, not relevant for this test
    }
};

TEST_CASE("getUserName", "[getUserName]")
{
    // arrange
    UserRepositoryStub stub;
    UserService service{ stub };

    // act
    auto name{ service.getUserName(1) };

    // assert
    CHECK(name == "Alice");
}

// ---------------------------------------------------------------------------
// Implementing a Mock

class UserRepositoryMock : public UserRepository
{
private:
    int m_saveCalled;
    std::optional<std::string> m_savedName;

public:
    UserRepositoryMock () : m_saveCalled{} {}

    std::shared_ptr<User> findById(int) override {
        return nullptr;  // no implementation, not needed for this test
    }

    void save(const User& user) override {
        m_saveCalled++;
        m_savedName = user.getName();
    }

    int getSaveCalled() const { return m_saveCalled; }
    std::optional<std::string> getSavedName() const { return m_savedName; }
};

TEST_CASE("UserService calls save on UserRepository", "[registerUser]")
{
    // arrange
    UserRepositoryMock mock;
    UserService service{ mock };

    // act
    service.registerUser("Bob");

    // assert
    REQUIRE(mock.getSaveCalled() == 1);
    REQUIRE(mock.getSavedName() == "Bob");
}

// ===========================================================================
// Avoid Using Complex Logic

TEST_CASE("Bad Example for testing IsPrime", "[Avoid_Using_Complex_Logic]")
{
    // arrange
    int testCases[] { 2, 3, 5 };
    bool expected = true;

    // act and Assert
    for(auto number : testCases)
    {
        // act
        auto actual = IsPrime(number);

        // assert
        CHECK(expected == actual);
    }
}

TEST_CASE("Good Example for testing IsPrime", "[Avoid_Using_Complex_Logic_Improved]")
{
    // arrange
    int number = GENERATE(2, 3, 5);
    bool expected = true;

    // act
    auto actual = IsPrime(number);

    // assert
    REQUIRE(actual == expected);
}

// ===========================================================================
// Avoid Multiple Acts

struct Calculator
{
    static int add(int x, int y) { return x + y; }
    static int sub(int x, int y) { return x - y; }
    static int mul(int x, int y) { return x * y; }
    static int div(int x, int y) { return x / y; }
};

TEST_CASE("Bad Example for testing several methods", "[AdditionAndSubtraction_IntegerNumbers_ReturnsSumAndDifference]")
{
    // arrange
    auto number1{ 9 };
    auto number2{ 5 };
    auto expected1{ 14 };
    auto expected2{ 4 };

    // act 
    auto actual1 = Calculator::add(number1, number2);
    auto actual2 = Calculator::sub(number1, number2);

    // assert
    CHECK(actual1 == expected1);
    CHECK(actual2 == expected2);
}

TEST_CASE("Good Example for testing a single method", "[Add_TwoNumbers_ReturnsSum]") 
{
    // arrange
    auto [a, b, expected] = GENERATE(
        std::tuple{ 1, 4, 5 },
        std::tuple{ 2, 5, 7 },
        std::tuple{ 3, 6, 9 }
    );

    // act 
    auto actual = Calculator::add(a, b);

    // assert
    CHECK(expected == actual);
}


// ===========================================================================
// End-of-File
// ===========================================================================

// ===========================================================================
// Catch_Unit_Testing_Best_Practices.cpp / Unit Testing Best Practices
// ===========================================================================

#include "Catch2/catch.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>

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


TEST_CASE("getUserName // checks existence of a user // returns name of user or empty string", "[getUserName__ChecksExistenceOfUser__Returns_Name_or_EmptyString]")
{
    // arrange
    UserRepositoryStub stub;
    UserService service(stub);

    // act
    auto name = service.getUserName(1);

    // assert
    CHECK(name == "Alice");
}

//void dummyTest()
//{
//
//    UserRepositoryStub stub;
//
//    UserService service(stub);
//
//    auto name = service.getUserName(1);
//
//    assert(name == "Alice");
//}

// ---------------------------------------------------------------------------
// Implementing a Mock

// WEITER ....


// ===========================================================================
// End-of-File
// ===========================================================================

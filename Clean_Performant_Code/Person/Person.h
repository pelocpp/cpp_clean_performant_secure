// ===========================================================================
// Person.h // Test Helper Class
// ===========================================================================

#include <print>

class Person
{
private:
    std::string m_first;
    std::string m_last;
    size_t      m_age;

public:
    // Rule-of-Six methods
    Person() : m_age{ }
    {
        // std::println("default c'tor Person");
    }

    Person(std::string first, std::string last, size_t age)
        : m_first{ first }, m_last{ last }, m_age{ age }
    {
        // std::println("user defined c'tor Person");
    }

    Person(const Person& other)
        : m_first{ other.m_first }, m_last{ other.m_last }, m_age{ other.m_age }
    {
        // std::println("copy c'tor Person");
    }

    Person(Person&& other) noexcept
        : m_first{ std::move(other.m_first) }, m_last{ std::move(other.m_last) }, m_age{ std::move(other.m_age) }
    {
        // std::println("move c'tor Person");
    }

    Person& operator= (const Person& other)
    {
        // std::println("copy operator= Person");

        m_first = other.m_first;
        m_last = other.m_last;
        m_age = other.m_age;

        return *this;
    }

    Person& operator= (Person&& other) noexcept
    {
        // std::println("move operator= Person");

        m_first = std::move(other.m_first);
        m_last = std::move(other.m_last);
        m_age = std::move(other.m_age);

        return *this;
    }

    ~Person() {
        // std::println("d'tor Person");
    }

    // operators - needed for hashing
    bool operator== (const Person& other) const {

        return
            m_first == other.m_first &&
            m_last == other.m_last &&
            m_age == other.m_age;
    }

    // getter
    std::string getFirstname() const { return m_first; }
    std::string getLastname() const { return m_last; }
    std::size_t getAge() const { return m_age; }
};

namespace std
{
    // =======================================================================
    // std::println support
    template<>
    class formatter<Person>
    {
    private:
        int m_width; // specified width of the field

    public:
        constexpr auto parse(std::format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const Person& p, std::format_context& ctx) const {
            return std::format_to(ctx.out(),
                "{} {} [Age {}]", p.getFirstname(), p.getLastname(), p.getAge()
            );
        }
    };
}

// ===========================================================================
// End-of-File
// ===========================================================================

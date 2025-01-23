// ===========================================================================
// CompileTimeProgramming.cpp
// ===========================================================================

#include <iostream>
#include <print>

namespace MetaprogrammingWithTemplates {

    template <typename T>
    auto powerN(const T& v, size_t n) {
        auto product = T{ 1 };
        for (size_t i{}; i != n; ++i) {
            product *= v;
        }
        return product;
    }

    static void metaprogramming_01()
    {
        auto x = powerN<float>(1.5f, 3);   // x is a float
        auto y = powerN<int>(3, 4);        // y is an int

        std::println("powerN: {}", x);
        std::println("powerN: {}", y);
    }

    static void metaprogramming_02()
    {
        auto x = powerN(2.5, 5);           // x is a float
        auto y = powerN(3, 6);             // y is an int

        std::println("powerN: {}", x);
        std::println("powerN: {}", y);
    }

    // =============================================================

    template <typename T>
    class Rectangle
    {
    private:
        T m_x{};
        T m_y{};
        T m_width{};
        T m_height{};

    public:
        Rectangle(T x, T y, T width, T height)
            : m_x{ x }, m_y{ y }, m_width{ width }, m_height{ height }
        {
        }

        auto getX() const { return m_x; }
        auto getY() const { return m_y; }
        auto width() const { return m_width; }
        auto height() const { return m_height; }
        auto area() const { return m_width * m_height; }
        auto circumference() const { return 2 * (m_width * m_height); }
    };
}

namespace std
{
    using namespace MetaprogrammingWithTemplates;

    template <typename T>
    struct formatter<Rectangle<T>>
    {
        constexpr auto parse(std::format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const Rectangle<T>& rect, std::format_context& ctx) const {

            auto x{ rect.getX() };
            auto y{ rect.getY() };
            auto width{ rect.width() };
            auto height{ rect.height() };

            return std::format_to(
                ctx.out(),
                "Rectangle at {}:{} [width: {} / height: {}]",
                x, y, width, height
            );
        }
    };
}

namespace MetaprogrammingWithTemplates {

    template <typename T>
    auto isSquare(const Rectangle<T>& rect) {
        return rect.width() == rect.height();
    }

    static void metaprogramming_03()
    {
        auto r1 = Rectangle<float>{ 1.0f, 2.0f, 3.0f, 4.0f };
        auto r2 = Rectangle{ -2, -2, 4, 4 };

        std::println("{}", r1);
        std::println("{}", r2);

        auto b1{ isSquare(r1) };
        auto b2{ isSquare(r2) };

        std::println("isSquare: {}", b1);
        std::println("isSquare: {}", b2);
    }
}

namespace MetaprogrammingWithTemplates {

    template <int N, typename T>
    auto morePowerN(const T& v) {
        auto product = T{ 1 };
        for (size_t i{}; i != N; ++i) {
            product *= v;
        }
        return product;
    }

    // (explicit) template specialization 
    template<>
    auto morePowerN<2, int>(const int& v) {
        return v * v;
    }

    static void metaprogramming_04()
    {
        auto x1 = morePowerN<2>(4);
        auto x2 = morePowerN<3>(4);

        std::println("{}", x1);
        std::println("{}", x2);

        auto x3 = morePowerN<3,double>(4.5);
        auto x4 = morePowerN<3,float>(4.5);

        std::println("{}", x3);
        std::println("{}", x4);

        auto x5 = morePowerN<2>(4);
        std::println("{}", x5);
    }
}

namespace MetaprogrammingWithTemplates {

    // Abbreviated function templates
    auto powerN(const auto& v, int n) {
        auto product = decltype(v){ 1 };
        for (size_t i{}; i != n; ++i) {
            product *= v;
        }
        return product;
    }

    static void metaprogramming_05()
    {
        auto x = powerN<float>(1.5f, 3);   // x is a float
        auto y = powerN<int>(3, 4);        // y is an int

        std::println("powerN: {}", x);
        std::println("powerN: {}", y);
    }
}

// =================================================================

void compile_time_programming()
{
    using namespace MetaprogrammingWithTemplates;

    //metaprogramming_01();
    //metaprogramming_02();
    //metaprogramming_03();
    //metaprogramming_04();
    metaprogramming_05();
}

// ===========================================================================
// End-of-File
// ===========================================================================

// ===========================================================================
// CompileTimeHashStrings.cpp
// ===========================================================================

#include <iostream>
#include <print>

namespace MetaprogrammingHashExample {

    // =======================================================================
    // Metaprogramming: Hash Strings at Compile Time

    static constexpr auto hashFunction(const char* str) -> size_t {
        auto sum{ size_t{ 0 } };
        for (auto ptr{ str }; *ptr != '\0'; ++ptr)
            sum += *ptr;
        return sum;
    }

    static void metaprogramming_01()
    {
        constexpr auto hash = hashFunction("abc");
    }
}

namespace MetaprogrammingHashExample {

    // =======================================================================
    // Metaprogramming: Example PrehashedString

    // https://stackoverflow.com/questions/43435279/what-does-const-charan-mean

    // https://stackoverflow.com/questions/30981499/meaning-of-the-syntax-const-char-x

    // HIER SEHR GUT BESCHRIEBEN !!!!!!!!!!!!!!

    // Arrays of unknown bound
    // https://en.cppreference.com/w/cpp/language/array 

    class PrehashedString
    {
    private:
        size_t      m_hash{};
        size_t      m_size{};
        const char* m_strptr{ nullptr };

    public:
        template <size_t N>
        constexpr PrehashedString(const char(&str)[N])
            : m_hash{ hashFunction(&str[0]) },
            m_size{ N - 1 },       // The subtraction is to avoid null at end
            m_strptr{ &str[0] } {

            size_t dummy = N;
        }

        auto operator==(const PrehashedString& other) const {
            return
                m_size == 
                    other.m_size &&
                    std::equal(c_str(), c_str() + m_size, other.c_str());
        }

        auto operator!=(const PrehashedString& other) const {
            return !(*this == other);
        }

        constexpr auto size()     const { return m_size; }
        constexpr auto get_hash() const { return m_hash; }
        constexpr auto c_str()    const -> const char* { return m_strptr; }
    };

    static void metaprogramming_02()
    {
        auto prehashed_string = PrehashedString{ "my_string" };

        // This does not compile
        // The prehashed_string object would be broken if the str is modified
        //auto str = std::string{ "my_string" };
        //auto prehashed_string2 = PrehashedString{ str.c_str() };

        // This does not compile.
        // The prehashed_string object would be broken if the strptr is deleted
        //auto* strptr = new char[5];
        //auto prehashed_string = PrehashedString{ strptr };
    }
}

namespace std
{
    // =======================================================================
    // Supporting std::println

   using namespace MetaprogrammingHashExample;

    //template <typename T>
    //struct formatter<Rectangle<T>>
    //{
    //    constexpr auto parse(std::format_parse_context& ctx) {
    //        return ctx.begin();
    //    }

    //    auto format(const Rectangle<T>& rect, std::format_context& ctx) const {

    //        auto x{ rect.getX() };
    //        auto y{ rect.getY() };
    //        auto width{ rect.width() };
    //        auto height{ rect.height() };

    //        return std::format_to(
    //            ctx.out(),
    //            "Rectangle at {}:{} [width: {} / height: {}]",
    //            x, y, width, height
    //        );
    //    }
    //};
}

// =================================================================

void compile_time_hash_strings()
{
    using namespace MetaprogrammingHashExample;

    metaprogramming_01();
    metaprogramming_02();
    //metaprogramming_03();
    //metaprogramming_04();
    //metaprogramming_05();
    //metaprogramming_06();
    //metaprogramming_07();
}

// ===========================================================================
// End-of-File
// ===========================================================================

// ===========================================================================
// Guidelines_Core_Cpp.cpp
// ===========================================================================

#include <cassert>
#include <print>

namespace GuidelinesCoreCpp {

    // =======================================================================
    // Invariants

    // https://www.geeksforgeeks.org/what-is-class-invariant/

    struct Point2D
    {
        Point2D() : Point2D{ 0, 0 } {}
        Point2D(int x, int y) : m_x{ x }, m_y{ y } {}

        int m_x{};
        int m_y{};
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
            assert(m_position.m_x >= 0 && m_position.m_x <= m_width);
            assert(m_position.m_y >= 0 && m_position.m_y <= m_height);
        }
    };

    static void guidelines_inheritance_invariants()
    {
        Game game{ 30, 20, Point2D{ 5, 5 } };

        // will throw assert error because the position is out of bounds
        game.moveTo(Point2D{ 20, 20 });

        // game is being spawned out of bounds 
        Game anotherGame{ 10, 10, Point2D{ 12, 8 } };
    }

    // =======================================================================
    // The new syntax "= default" in C++11

    class A
    {
    public:
        int x;
        A() {}
    };

    class B
    {
    public:
        int m_x;
        B(int x) : m_x(x) {}
        B() = default;
    };

    static void guidelines_defaulted_constructor()
    {
        int x = 5;

        A a;

        B b;

    }
}

void guidelines_core_cpp()
{
    using namespace GuidelinesCoreCpp;

 //   guidelines_inheritance_invariants();
    guidelines_defaulted_constructor();

}

// ===========================================================================
// End-of-File
// ===========================================================================

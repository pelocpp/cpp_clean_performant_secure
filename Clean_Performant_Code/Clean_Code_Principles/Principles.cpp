// ===========================================================================
// Principles.cpp
// ===========================================================================

#include <print>

namespace Clean_Code_Principles {

    namespace YAGNI {

        static int sum(int x, int y)
        {
            return x + y;
        }

        static int sumTwoAndThree()
        {
            return 2 + 3;
        }
    }

    namespace Lose_Coupling_Violating {

        class Lamp {
        public:
            void on() { std::println("On"); }
            void off() { std::println("Off"); }
        };

        class Switch {
        private:
            Lamp& m_lamp;
            bool  m_state;

        public:
            Switch(Lamp& lamp) : m_lamp(lamp), m_state{ false } {}
            void toggle() {
                if (m_state) {
                    m_state = false;
                    m_lamp.off();
                }
                else {
                    m_state = true;
                    m_lamp.on();
                }
            }
        };

        static void principles_lose_coupling_violating()
        {
            Lamp lamp;
            Switch switcher{ lamp };
            switcher.toggle();
            switcher.toggle();
            switcher.toggle();
        }
    }

    namespace Lose_Coupling_Respecting {

        class ISwitchable {
        public:
            virtual void on() = 0;
            virtual void off() = 0;
        };

        class Switch {
        private:
            ISwitchable& m_switcherable;
            bool         m_state;

        public:
            Switch(ISwitchable& switchable)
                : m_switcherable(switchable), m_state{ false }
            {}
            void toggle() {
                if (m_state) {
                    m_state = false;
                    m_switcherable.off();
                }
                else {
                    m_state = true;
                    m_switcherable.on();
                }
            }
        };

        class Lamp : public ISwitchable {
        public:
            void on() override { std::println("On"); }
            void off() override { std::println("Off"); }
        };

        static void principles_lose_coupling_respecting()
        {
            Lamp lamp;
            Switch switcher{ lamp };
            switcher.toggle();
            switcher.toggle();
            switcher.toggle();
        }
    }
}

// =================================================================

void clean_code_principles()
{
    using namespace Clean_Code_Principles;

    Lose_Coupling_Violating::principles_lose_coupling_violating();
    Lose_Coupling_Respecting::principles_lose_coupling_respecting();
}

// ===========================================================================
// End-of-File
// ===========================================================================

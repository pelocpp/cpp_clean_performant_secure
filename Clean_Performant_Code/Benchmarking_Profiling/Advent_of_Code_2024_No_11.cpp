// ===========================================================================
// Advent_of_Code_2024_No_11.cpp // Benchmarking // Profiling
// Puzzle 11 of Advent of Code 2024
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <forward_list>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

// ===========================================================================
// Eine Ausf�hrung von Schritt zeit, dass in Methode 'splitPebble'
// ein HotSpot vorhanden ist.
// 
// Es bietet sich ein Redesign von 'splitPebble' an:
// Die Ausf�hrungszeiten mit 'splitPebbleEx' sind besser.
// 
// Ein zweites Redesign dieser Methode f�hrt zu besseren Ausf�hrungszeiten.
// 
// To be Done 1: Die von 'splitPebble' zeregten Zahlen in eine Map aufnehmen (sog. Memoization),
// dann die Ausf�hrungszeiten beobachten.
// 
// To be Done 1: Es werden mehrere Male std::pairs mit 2 size_t Variablen kopiert.
// In den Meldungen gibt es Warnungen hierzu:
// 'auto' doesn't deduce references. A possibly unintended copy is being made.
// Hier sollte ein Mikro-benchmark durchgef�hrt werden oder eine Laufzeitmessung.
// 
// Weitere Links:
// https://advent-of-code.xavd.id/writeups/2024/day/11/
// https://www.reddit.com/r/adventofcode/comments/1hbm0al/2024_day_11_solutions/?rdt=46542
// 
// ===========================================================================
 
// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "./Data/Puzzle11_TestData.txt" };
static std::string_view g_filenameRealData{ "./Data/Puzzle11_RealData.txt" };

// ===========================================================================
// interfaces

struct IPlutonianPebbles
{
    virtual ~IPlutonianPebbles() = default;

    virtual void   readPuzzleFromFile(const std::string_view filename) = 0;
    virtual void   printPebbles() = 0;
    virtual void   blink() = 0;
    virtual void   blinking(size_t count) = 0;
    virtual size_t size() = 0;
};

// ===========================================================================
// types

class PlutonianPebbles : public IPlutonianPebbles
{
public:
    void blinking(size_t count) override
    {
        for (size_t i{}; i != count; ++i) {
            std::println("{} blinking ...", (i + 1));
            blink();
        }
    }

    static std::pair<size_t, size_t> splitPebble(size_t pebble)
    {
        std::string s{ std::to_string(pebble) };

        size_t length{ s.size() };

        length /= 2;
        std::string leftPart{ s.substr(0,length) };
        std::string rightPart{ s.substr(length) };

        // convert two halfs to size_t
        size_t left{}, right{};
        std::istringstream iss{ leftPart };
        iss >> left;
        iss = std::istringstream{ rightPart };
        iss >> right;

        return { left , right };
    }

    static bool hasEvenDigits(size_t pebble)
    {
        size_t numDigits{};
        while (pebble != 0) {
            pebble /= 10;
            ++numDigits;
        }

        return (numDigits % 2) == 0;
    }

    // redesigned version of hasEvenDigits
    static std::pair<bool, size_t> hasEvenDigitsEx(size_t pebble)
    {
        size_t numDigits{};
        while (pebble != 0) {
            pebble /= 10;
            ++numDigits;
        }
        
        return { (numDigits % 2) == 0 , numDigits };
    }

    // First redesigned version of splitPebble
    static std::pair<size_t, size_t> splitPebbleEx(size_t pebble, size_t numDigits)
    {
        size_t left{};
        size_t right{};

        switch (numDigits)
        {
        case 2:
            left = pebble / 10;
            right = pebble % 10;
            break;

        case 4:
            left = pebble / 100;
            right = pebble % 100;
            break;

        case 6:
            left = pebble / 1000;
            right = pebble % 1000;
            break;

        case 8:
            left = pebble / 10000;
            right = pebble % 10000;
            break;

        case 10:
            left = pebble / 100000;
            right = pebble % 100000;
            break;

        case 12:
            left = pebble / 1000000;
            right = pebble % 1000000;
            break;

        case 14:
            left = pebble / 10000000;
            right = pebble % 10000000;
            break;

        case 16:
            left = pebble / 100000000;
            right = pebble % 100000000;
            break;

        case 18:
            left = pebble / 1000000000;
            right = pebble % 1000000000;
            break;
        }

        return { left , right };
    }

    // Second redesigned version of splitPebble
    static std::pair<size_t, size_t> splitPebbleExEx(size_t pebble, size_t numDigits)
    {
        static size_t pow10[10]
        {
            1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000
        };

        size_t denominator{ pow10[numDigits / 2 - 1] };

        size_t left{ pebble / denominator };
        size_t right{ pebble % denominator };

        return { left , right };
    }
};

// ===========================================================================
// brute force approach: using linked list
class PlutonianPebblesPartI : public PlutonianPebbles
{
protected:
    std::forward_list<size_t> m_pebbles{};
    size_t                    m_size;

public:
    PlutonianPebblesPartI() : m_pebbles{}, m_size{} {}

    void readPuzzleFromFile(const std::string_view filename) override
    {
        std::ifstream file{ filename.data() };
        std::string line{};

        if (file.is_open()) {

            // read a single line
            std::getline(file, line);

            std::stringstream ss{ line };
            std::string token;

            auto pos{ m_pebbles.before_begin() };

            // extract all pebbles from this line
            while (ss >> token) {

                // convert std::string to size_t
                size_t pebble{};
                std::istringstream iss{ token };
                iss >> pebble;

                // insert pebble at end of list
                m_pebbles.insert_after(pos, pebble);
                m_size++;

                ++pos;
            }

            file.close();
        }
        else {

            std::println("Unable to open file {} !", filename);
        }
    }

    void printPebbles() override
    {
        for (const auto pebble : m_pebbles) {
            std::print("{} ", pebble);
        }
        std::println();
    }

    void blink() override
    {
        auto pos{ m_pebbles.begin() };

        while (pos != m_pebbles.end()) {

            size_t pebble{ *pos };

            if (pebble == 0) {
                *pos = 1;
            }
            else if (hasEvenDigits(pebble)) {

                const auto [leftHalf, rightHalf] = splitPebble(pebble);
                *pos = leftHalf;
                m_pebbles.insert_after(pos, rightHalf);
                m_size++;

                ++pos; // skip new right half
            }
            //else if (auto result = hasEvenDigitsEx(pebble); result.first) {

            //    const auto [leftHalf, rightHalf] = splitPebbleEx(pebble, result.second);  // <== splitPebbleEx or splitPebbleExEx
            //    *pos = leftHalf;
            //    m_pebbles.insert_after(pos, rightHalf);
            //    m_size++;

            //    ++pos; // skip new right half
            //}
            else {
                *pos *= 2024;
            }

            ++pos;
        }
    }

    size_t size() override
    {
        return m_size;
    }
};

// ===========================================================================
// part two

class PlutonianPebblesPartII : public PlutonianPebbles
{
private:
    std::unordered_map<size_t, size_t> m_countedPebbles;
    size_t                             m_size;

public:
    PlutonianPebblesPartII() : m_countedPebbles{}, m_size{} {}

    void readPuzzleFromFile(const std::string_view filename) override {

        std::ifstream file{ filename.data() };
        std::string line{};

        if (file.is_open()) {

            // read a single line
            std::getline(file, line);

            std::stringstream ss{ line };
            std::string token;

            // extract all pebbles from this line
            while (ss >> token) {

                // convert std::string to size_t
                size_t pebble{};
                std::istringstream iss{ token };
                iss >> pebble;

                // insert pebble into dictionary
                m_countedPebbles[pebble] += 1;
                m_size++;
            }

            file.close();
        }
        else {

            std::println("Unable to open file {} !", filename);
        }
    }

    void blink() override
    {
        std::unordered_map<size_t, size_t> pebbles{ };

        for (auto [pebble, count] : m_countedPebbles) {

            if (pebble == 0) {
                pebbles[1] += count;
            }
            else if (hasEvenDigits(pebble)) {  // TODO : schneller berechnen

                const auto [leftHalf, rightHalf] = splitPebble(pebble);

                // add two new pebbles
                pebbles[leftHalf] += count;
                pebbles[rightHalf] += count;
            }
            else {
                pebbles[pebble * 2024] += count;
            }
        }

        // update dictionary
        m_countedPebbles = pebbles;
    }

    size_t size() override
    {
        m_size = 0;

        for (auto [pebble, count] : m_countedPebbles) {
            m_size += count;
        }

        return m_size;
    }

    void printPebbles() override
    {
        for (const auto [pebble, count] : m_countedPebbles) {

            if (count != 0) {
                std::print("{} [{}] ", pebble, count);
            }


        }
        std::println();
    }
};

// ===========================================================================
// part one

static void puzzle_11_part_one()
{
    ScopedTimer watch{};
    PlutonianPebblesPartI pebbles;
    pebbles.printPebbles();
    pebbles.readPuzzleFromFile(g_filenameRealData);
    std::println("Size: {}", pebbles.size());
    pebbles.blinking(25);
    std::println("Size: {}", pebbles.size());   // expected 213625 (25 times blinking) // ca 2.000 msecs (debug)
}

// ===========================================================================
// part two

static void puzzle_11_part_two()
{
    ScopedTimer watch{};
    PlutonianPebblesPartII pebbles;
    pebbles.readPuzzleFromFile(g_filenameRealData);
    pebbles.printPebbles();
    std::println("Size: {}", pebbles.size());
    pebbles.blinking(75);
    std::println("Size: {}", pebbles.size());   // expected 252442982856820    // ca 880 msecs (debug)
}

// ===========================================================================
// main

void performance_profiling_puzzle_11()
{
    puzzle_11_part_one();
    //puzzle_11_part_two();
}

// ===========================================================================
// End-of-File
// ===========================================================================

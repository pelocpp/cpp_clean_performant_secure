// ===========================================================================
// Advent_of_Code_2024_No_11.cpp // Benchmarking // Profiling
// Puzzle 11 of Advent of Code 2024
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <algorithm>
#include <cassert>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>


// TBD: Die zerlegten Zahlen ebenfalls in eine Map aufnehmen ... dann muss nicht so oft zerlegt werden

// https://advent-of-code.xavd.id/writeups/2024/day/11/

// https://www.reddit.com/r/adventofcode/comments/1hbm0al/2024_day_11_solutions/?rdt=46542

// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "Puzzle11_TestData.txt" };
static std::string_view g_filenameRealData{ "Puzzle11_RealData.txt" };

// ===========================================================================
// types

struct IPlutonianPebbles
{
    virtual void readPuzzleFromFile(const std::string_view filename) = 0;
    virtual void printPebbles() = 0;
    virtual void blink() = 0;
    virtual void blinking(size_t count) = 0;
    virtual size_t size() = 0;
};

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
        int numDigits{};
        while (pebble != 0) {
            pebble /= 10;
            ++numDigits;
        }
        return (numDigits % 2) == 0;
    }
};

class BruteForcePlutonianPebbles : public PlutonianPebbles
{
protected:
    std::forward_list<size_t> m_pebbles{};
    size_t                    m_size;

public:
    BruteForcePlutonianPebbles() : m_pebbles{}, m_size{} {}

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

class AdvancedPlutonianPebbles : public PlutonianPebbles
{
private:
    std::unordered_map<size_t, size_t> m_countedPebbles;
    size_t                             m_size;

public:
    AdvancedPlutonianPebbles() : m_countedPebbles{}, m_size{} {}

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
// forward declarations

// ===========================================================================
// input & output

// ===========================================================================
// types / logic

// ===========================================================================
// testing

static void puzzle_10_test()
{
    ScopedTimer watch{};
    BruteForcePlutonianPebbles pebbles;
    pebbles.readPuzzleFromFile(g_filenameTestData);
    pebbles.printPebbles();
    //std::println("Size: {}", pebbles.size());
    pebbles.blinking(5);
    //std::println("Size: {}", pebbles.size());   // expected 213625 (25 times blinking)
}

static void puzzle_11_test()
{
    ScopedTimer watch{};
    AdvancedPlutonianPebbles pebbles;
    pebbles.readPuzzleFromFile(g_filenameTestData);
    pebbles.printPebbles();
    //std::println("Size: {}", pebbles.size());
    pebbles.blinking(6);
    //std::println("Size: {}", pebbles.size());   // expected 213625 (25 times blinking)
}

static void puzzle_12_test()
{
}

// ===========================================================================
// part one

static void puzzle_11_part_one()
{
    ScopedTimer watch{};
    BruteForcePlutonianPebbles pebbles;
    pebbles.printPebbles();
    pebbles.readPuzzleFromFile(g_filenameRealData);
    std::println("Size: {}", pebbles.size());
    pebbles.blinking(25);
    std::println("Size: {}", pebbles.size());   // expected 213625 (25 times blinking) // 2000 msecs
}

// ===========================================================================
// part two

static void puzzle_11_part_two()
{
    ScopedTimer watch{};
    AdvancedPlutonianPebbles pebbles;
    pebbles.readPuzzleFromFile(g_filenameRealData);
    pebbles.printPebbles();
    std::println("Size: {}", pebbles.size());
    pebbles.blinking(75);
    std::println("Size: {}", pebbles.size());   // expected 252442982856820
}

// ===========================================================================
// main

void puzzle_11()
{
    //puzzle_10_test();
    //puzzle_11_test();
    //puzzle_12_test();

    puzzle_11_part_one();
    //puzzle_11_part_two();
}

// ===========================================================================
// End-of-File
// ===========================================================================

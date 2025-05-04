// ===========================================================================
// Advent_of_Code_2024_No_10.cpp // Profiling
// Puzzle 10 of Advent of Code 2024
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <fstream>
#include <list>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

// ===========================================================================
// global data

static std::string_view g_filenameTestData{ "Puzzle10_TestData.txt" };
static std::string_view g_filenameRealData{ "Puzzle10_RealData.txt" };

// ===========================================================================
// types

// Why can’t I compile an unordered_map with a pair as key?
// https://medium.com/@amansingh_2001/why-cant-i-compile-an-unordered-map-with-a-pair-as-key-6eb1ad66cbd8

template <size_t Size>
class TopographicMap
{
private:

    using Coord = std::pair<int, int>;

    struct CoordsHash
    {
        std::size_t operator () (const Coord& coord) const {
            return std::hash<int>{}(coord.first) ^ std::hash<int>{}(coord.second);
        }
    };

    struct CoordsCompare
    {
        bool operator () (const Coord& coord1, const Coord& coord2) const {

            const auto& [row1, col1] = coord1;
            const auto& [row2, col2] = coord2;

            if (row1 < row2) {
                return true;
            }
            else if (row1 > row2) {
                return false;
            }
            else {
                return col1 < col2;
            }
        }
    };

    using Map = std::array<std::array<int, Size>, Size>;
    using Trail = std::list<Coord>;
    using ListOfTrails = std::list<Trail>;
    using MapOfTrails = std::unordered_map<Coord, ListOfTrails, CoordsHash>;
    //using MapOfTrails = std::map<Coord, ListOfTrails, CoordsCompare>;

    Map                  m_map;            // topographic map indicating the height at each position
    Trail                m_currentTrail;   // trail being in construction
    MapOfTrails          m_trails;         // map of found trails, unique for all trail heads

public:
    TopographicMap() : m_map{} {}

    void readPuzzleFromFile(const std::string_view filename) {

        std::ifstream file{ filename.data() };

        if (file.is_open()) {

            // read line per line
            std::string line{};

            int row{ -1 };

            while (std::getline(file, line)) {

                if (line.empty()) {
                    break;
                }

                ++row;

                // copy line into map
                for (int col{}; char ch : line) {

                    m_map[row][col] = (ch == '.') ? -1 : ch - '0';
                    ++col;
                }
            }

            file.close();
        }
        else {

            std::println("Unable to open file {} !", filename);
        }
    }

    void printMap()
    {
        for (const auto& row : m_map) {
            for (const auto height : row) {
                std::print("{}", (height >= 0) ? (char)(height + '0') : '.');
            }
            std::println();
        }
    }

    void searchAllTrails() {

        for (int row{}; const auto & mapRow : m_map) {

            for (int col{}; const auto height : mapRow) {

                if (height == 0) {

                    // add key and emtpy list into trails map
                    m_trails[{ row, col }] = ListOfTrails{};

                    // found a trail head, search trails
                    searchTrail({ row , col });
                }

                ++col;
            }

            ++row;
        }
    }

    void searchTrail(const Coord& head) {

        auto [row, col] = head;

        // add key and emtpy list into trails map
        m_trails[{ row, col }] = ListOfTrails{};

        // found a trail head, search trails
        searchTrailHelper(head, head, 0, 1);
    }

    void printTrails() {

        for (const auto& entry : m_trails) {

            auto [row, col] = entry.first;
            std::println("Trail Head: {},{}", row, col);

            for (const auto& listOfTrails : entry.second) {
                printTrail(listOfTrails);
            }
            std::println();
        }
    }

    void printScores() {

        for (const auto& entry : m_trails) {

            const auto& coord = entry.first;
            const std::list<Trail>& trails = entry.second;

            std::println("Trail Head: [{},{}] - Score: {}", coord.first, coord.second, trails.size());
        }
    }

    size_t totalScore() {

        size_t score{};

        for (const auto& entry : m_trails) {

            const std::list<Trail>& trails = entry.second;
            score += trails.size();
        }

        return score;
    }

private:
    void searchTrailHelper(const Coord& head, const Coord& next, size_t height, size_t length) {

        m_currentTrail.push_back(next);

        if (length == 10) {

            // add found trail to the list of all trails,
            // if the found trail has reached a new 9-height position?

            // addTrailIf(head, next, m_currentTrail);
            addTrailIfTwo(head, next, m_currentTrail);
        }
        else {

            // determine list of possible next steps
            std::vector<Coord> steps{ nextSteps(next, height + 1) };

            // do next step
            for (const Coord& step : steps) {
                searchTrailHelper(head, step, height + 1, length + 1);
            }
        }

        m_currentTrail.pop_back();
    }

    void addTrailIf(const Coord& head, const Coord& next, const Trail& trail) {

        // add found trail to the list of all trails,
        // if the found trail has reached a new 9-height position?
        auto& [coord, listOfTrails] = *m_trails.find(head);

        if (listOfTrails.size() == 0) {

            // found first trail according to this trail head, just add this trail
            listOfTrails.push_back(m_currentTrail);
        }
        else {

            // does the found trail reach a new 9-height position?
            bool newNineHeightPosition{ true };

            for (const auto& trail : listOfTrails) {

                const auto& nineHeightPosition{ trail.back() };

                if (nineHeightPosition == next) {

                    newNineHeightPosition = false;
                    break;
                }
            }

            if (newNineHeightPosition) {
                listOfTrails.push_back(m_currentTrail);
            }
        }
    }

    void addTrailIfTwo(const Coord& head, const Coord& next, const Trail& trail) {

        // add found trail to the list of all trails
        auto& [coord, listOfTrails] = *m_trails.find(head);
        listOfTrails.push_back(m_currentTrail);
    }

    std::vector<Coord> nextSteps(Coord coord, size_t height) {

        std::vector<Coord> steps{};

        auto [row, col] = coord;

        // check upper step
        if (row != 0) {
            if (m_map[row - 1][col] == height) {
                steps.push_back({ row - 1 , col });
            }
        }

        // check step to the right
        if (col != Size - 1) {
            if (m_map[row][col + 1] == height) {
                steps.push_back({ row , col + 1 });
            }
        }

        // check step to the left
        if (col != 0) {
            if (m_map[row][col - 1] == height) {
                steps.push_back({ row , col - 1 });
            }
        }

        // check lower step
        if (row != Size - 1) {
            if (m_map[row + 1][col] == height) {
                steps.push_back({ row + 1 , col });
            }
        }

        return steps;
    }

    void printTrail(const std::list<Coord>& trail) {

        for (int count{}; const auto & coord : trail) {

            ++count;
            std::print("[{},{}]", coord.first, coord.second);
            if (count < 10) {
                std::print(" => ");
            }
        }
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
    TopographicMap<4> map;
    map.readPuzzleFromFile(g_filenameTestData);
    map.printMap();
    map.searchTrail({ 0, 0 });
    map.printTrails();
}

static void puzzle_11_test()
{
    TopographicMap<8> map;
    map.readPuzzleFromFile(g_filenameTestData);
    map.printMap();
    map.searchAllTrails();
    map.printScores();
    std::println("Total score: {}", map.totalScore());
}

static void puzzle_12_test()
{
    TopographicMap<8> map;
    map.readPuzzleFromFile(g_filenameTestData);
    map.printMap();
    map.searchAllTrails();
    map.printScores();
    std::println("Total score: {}", map.totalScore());
}

// ===========================================================================
// part one

static void puzzle_10_part_one()
{
    ScopedTimer watch{};

    TopographicMap<57> map;
    map.readPuzzleFromFile(g_filenameRealData);
    map.searchAllTrails();
    std::println("Total Rating: {}", map.totalScore());
}

// ===========================================================================
// part two

static void puzzle_10_part_two()
{
    ScopedTimer watch{};

    TopographicMap<57> map;
    map.readPuzzleFromFile(g_filenameRealData);
    map.searchAllTrails();
    std::println("Total Rating: {}", map.totalScore());
}

// ===========================================================================
// main

void performance_profiling_puzzle_10()
{
    //puzzle_10_test();
    //puzzle_11_test();
    puzzle_10_part_one();   // 754

    //puzzle_12_test();
    //puzzle_10_part_two();     // 1609
}

// ===========================================================================
// End-of-File
// ===========================================================================

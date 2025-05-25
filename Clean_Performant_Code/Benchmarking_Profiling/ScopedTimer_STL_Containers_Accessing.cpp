// ===========================================================================
// ScopedTimer_STL_Containers_Accessing.cpp
// ===========================================================================

// WEITERARBEIT:

//A) bei mir nachschauen // Algos: Wie sucht man in einem sortierten Container !!!!
//
//B) Der Fuck mit der Optimierung: 
//
//https://quick-bench.com/q/Gw2nmpHJaoJJ4pP0BiVRvqBpAHI
//
//Wo kommt das her ?????????????
//
//C) https://www.reddit.com/r/cpp/comments/1g7a3qy/writing_realistic_benchmarks_is_hard_with/
//
//D) LESEN:
//
//https://idriss.medium.com/how-to-use-quick-benchmark-tool-for-performance-legacy-in-c-a96149ed40e3
//
//E) Buch vom Fedor aufnehmen !!!
//



#include "../LoggerUtility/ScopedTimer.h"

#include <algorithm>
#include <deque>
#include <list>
#include <map>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

namespace Benchmarking_STL_Containers_Populating {

    static std::vector<std::pair<std::string, std::string>> itemsInVector = {
      {"Hello", "lol"},
      {"The", "lol"},
      {"Quick", "lol"},
      {"Brown", "lol"},
      {"Fox", "lol"},
      {"Jumps", "lol"},
      {"Over", "lol"},
      {"The", "lol"},
      {"Lazy", "lol"},
      {"Dog", "lol"},
      {"Potato", "lol"},
      {"S", "lol"},
      {"Sdflkjad lkfjalkdsjflkadfl adfka df", "lol"},
      {"Tkajsdfkljalk djfkljalkds jflkajdslkf ja", "lol"},
      {"T dskafjl kadjsflkakldsfj klad", "lol"},
      {"H alkdfjkldasj flkjasdklfj klasdf", "lol"},
      {"Theeeeeeeeeeeeeeeeeeeee", "lol"},

      {"H1ello", "lol"},
      {"T1he", "lol"},
      {"Q1uick", "lol"},
      {"B1rown", "lol"},
      {"F1ox", "lol"},
      {"J1umps", "lol"},
      {"O1ver", "lol"},
      {"T1he", "lol"},
      {"L1azy", "lol"},
      {"D1og", "lol"},
      {"P1otato", "lol"},
      {"S1", "lol"},
      {"S1dflkjad lkfjalkdsjflkadfl adfka df", "lol"},
      {"T1kajsdfkljalk djfkljalkds jflkajdslkf ja", "lol"},
      {"T1 dskafjl kadjsflkakldsfj klad", "lol"},
      {"H1 alkdfjkldasj flkjasdklfj klasdf", "lol"},
      {"T1heeeeeeeeeeeeeeeeeeeee", "lol"},

      {"H2ello", "lol"},
      {"T2he", "lol"},
      {"Q2uick", "lol"},
      {"B2rown", "lol"},
      {"F2ox", "lol"},
      {"J2umps", "lol"},
      {"O2ver", "lol"},
      {"T2he", "lol"},
      {"L2azy", "lol"},
      {"D2og", "lol"},
      {"P2otato", "lol"},
      {"S2", "lol"},
      {"S2dflkjad lkfjalkdsjflkadfl adfka df", "lol"},
      {"T2kajsdfkljalk djfkljalkds jflkajdslkf ja", "lol"},
      {"T2 dskafjl kadjsflkakldsfj klad", "lol"},
      {"H2 alkdfjkldasj flkjasdklfj klasdf", "lol"},
      {"T2heeeeeeeeeeeeeeeeeeeee", "lol"},

      {"H3ello", "lol"},
      {"T3he", "lol"},
      {"Q3uick", "lol"},
      {"B3rown", "lol"},
      {"F3ox", "lol"},
      {"J3umps", "lol"},
      {"O3ver", "lol"},
      {"T3he", "lol"},
      {"L3azy", "lol"},
      {"D3og", "lol"},
      {"P3otato", "lol"},
      {"S3", "lol"},
      {"S3dflkjad lkfjalkdsjflkadfl adfka df", "lol"},
      {"T3kajsdfkljalk djfkljalkds jflkajdslkf ja", "lol"},
      {"T3 dskafjl kadjsflkakldsfj klad", "lol"},
      {"H3 alkdfjkldasj flkjasdklfj klasdf", "lol"},
      {"T3heeeeeeeeeeeeeeeeeeeee", "lol"},

      {"H4ello", "lol"},
      {"T4he", "lol"},
      {"Q4uick", "lol"},
      {"B4rown", "lol"},
      {"F4ox", "lol"},
      {"J4umps", "lol"},
      {"O4ver", "lol"},
      {"T4he", "lol"},
      {"L4azy", "lol"},
      {"D4og", "lol"},
      {"Potato", "lol"},
      {"S4", "lol"},
      {"S4dflkjad lkfjalkdsjflkadfl adfka df", "lol"},
      {"T44kajsdfkljalk djfkljalkds jflkajdslkf ja", "lol"},
      {"T4 dskafjl kadjsflkakldsfj klad", "lol"},
      {"H4 alkdfjkldasj flkjasdklfj klasdf", "lol"},
      {"T4heeeeeeeeeeeeeeeeeeeee", "lol"},

      {"H5ello", "lol"},
      {"T5he", "lol"},
      {"Q5uick", "lol"},
      {"B5rown", "lol"},
      {"F5ox", "lol"},
      {"J5umps", "lol"},
      {"O5ver", "lol"},
      {"T5he", "lol"},
      {"L5azy", "lol"},
      {"D5og", "lol"},
      {"P5otato", "lol"},
      {"S5", "lol"},
      {"S5dflkjad lkfjalkdsjflkadfl adfka df", "lol"},
      {"T5kajsdfkljalk djfkljalkds jflkajdslkf ja", "lol"},
      {"T5 dskafjl kadjsflkakldsfj klad", "lol"},
      {"H5 alkdfjkldasj flkjasdklfj klasdf", "lol"},
      {"T5heeeeeeeeeeeeeeeeeeeee", "lol"},

      {"H5ello", "lol"},
      {"T5he", "lol"},
      {"Q5uick", "lol"},
      {"B5rown", "lol"},
      {"F5ox", "lol"},
      {"J5umps", "lol"},
      {"O5ver", "lol"},
      {"T5he", "lol"},
      {"L5azy", "lol"},
      {"D5og", "lol"},
      {"P5otato", "lol"},
      {"S5", "lol"},
      {"S5dflkjad lkfjalkdsjflkadfl adfka df", "lol"},
      {"T5kajsdfkljalk djfkljalkds jflkajdslkf ja", "lol"},
      {"T5 dskafjl kadjsflkakldsfj klad", "lol"},
      {"H5 alkdfjkldasj flkjasdklfj klasdf", "lol"},
      {"T5heeeeeeeeeeeeeeeeeeeee", "lol"},
    };

    static auto sorted = [] {
        auto sorted = itemsInVector;
        std::sort(sorted.begin(), sorted.end());
        std::reverse(sorted.begin(), sorted.end());
        return sorted;
        }();

    // static const size_t Iterations = 100'000;
    static const size_t Iterations = 100000;

    static const std::string SearchKey{ "J1umps" };

    static void AccessingMap()
    {
        std::println("AccessingMap");

        std::map<std::string, std::string> result;
        for (const auto& item : sorted) {
            result.insert(std::make_pair(item.first, item.second));
        }

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            auto pos = result.find(SearchKey);
        }
    }

    static void AccessingUnorderedMap()
    {
        std::println("AccessingUnorderedMap");

        std::unordered_map<std::string, std::string> result;
        result.reserve(sorted.size());
        for (const auto& item : sorted) {
            result[item.first] = item.second;
        }

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            auto pos = result.find(SearchKey);
        }
    }

    static void AccessingVector()
    {
        std::println("AccessingVector");

        std::vector<std::pair<std::string, std::string>> result;
        result.reserve(sorted.size());

        for (const auto& item : sorted) {
            result.push_back(std::make_pair(item.first, item.second));
        }

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            auto pos = std::find_if(
                result.begin(), 
                result.end(),
                [](const auto& elem) {
                    return std::get<0>(elem) == SearchKey;
                }
            );
        }
    }

    static void AccessingSortedVector()
    {
        std::println("AccessingSortedVector");

        std::vector<std::pair<std::string, std::string>> result;
        result.reserve(sorted.size());

        for (const auto& item : sorted) {
            result.push_back(std::make_pair(item.first, item.second));
        }

        std::sort(
            result.begin(),
            result.end(),
            [](const auto& lhs, const auto& rhs) {
                return lhs.first < rhs.first;
            }
        );

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            auto pos = std::find_if(
                result.begin(),
                result.end(),
                [](const auto& elem) {
                    return std::get<0>(elem) == SearchKey;
                }
            );
        }
    }

    static void AccessingLinkedList()
    {
        std::println("AccessingLinkedList");

        std::list<std::pair<std::string, std::string>> result;

        for (const auto& item : sorted) {
            result.push_back(std::make_pair(item.first, item.second));
        }

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            auto pos = std::find_if(
                result.begin(),
                result.end(),
                [](const auto& elem) {
                    return std::get<0>(elem) == SearchKey;
                }
            );
        }
    }

    static void AccessingDeque()
    {
        std::println("AccessingDeque");

        std::deque<std::pair<std::string, std::string>> result;

        for (const auto& item : sorted) {
            result.push_back(std::make_pair(item.first, item.second));
        }

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            auto pos = std::find_if(
                result.begin(),
                result.end(),
                [](const auto& elem) {
                    return std::get<0>(elem) == SearchKey;
                }
            );
        }
    }
}

void benchmarking_stl_containers_accessing()
{
    using namespace Benchmarking_STL_Containers_Populating;

    AccessingMap();
    AccessingUnorderedMap();
    AccessingVector();
    AccessingSortedVector();
    AccessingLinkedList();
    AccessingDeque();
}

// ===========================================================================
// End-of-File
// ===========================================================================

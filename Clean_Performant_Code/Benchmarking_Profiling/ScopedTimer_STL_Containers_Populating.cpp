// ===========================================================================
// ScopedTimer_STL_Containers_Populating.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <algorithm>
#include <deque>
#include <list>
#include <map>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

// WEITER https://idriss.medium.com/how-to-use-quick-benchmark-tool-for-performance-legacy-in-c-a96149ed40e3

namespace Benchmarking_STL_Containers_Populating {

    std::vector<std::pair<std::string, std::string>> itemsInVector = {
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

    static auto sortedItems = [] () {
        // want to sort a reversed sorted copy of 'itemsInVector'
        auto toBeSorted = itemsInVector;
        std::sort(toBeSorted.begin(), toBeSorted.end());
        std::reverse(toBeSorted.begin(), toBeSorted.end());
        return toBeSorted;
    }();

    static const size_t Iterations = 100'000;

    static void PopulateMap()
    {
        std::println("PopulateMap");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            std::map<std::string, std::string> result;
            for (const auto& item : sortedItems) {
                result.insert(std::make_pair(item.first, item.second));
            }
        }
    }

    static void PopulateUnorderedMap()
    {
        std::println("PopulateUnorderedMap");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            std::unordered_map<std::string, std::string> result;
            result.reserve(sortedItems.size());

            for (const auto& item : sortedItems) {
                result[item.first] = item.second;
            }
        }
    }

    static void PopulateVector()
    {
        std::println("PopulateVector");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            std::vector<std::pair<std::string, std::string>> result;
            result.reserve(sortedItems.size());

            for (const auto& item : sortedItems) {
                result.push_back(std::make_pair(item.first, item.second));
            }
        }
    }

    static void PopulateSortedVector()
    {
        std::println("PopulateSortedVector");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            std::vector<std::pair<std::string, std::string>> result;
            result.reserve(sortedItems.size());

            for (const auto& item : sortedItems) {
                result.push_back(std::make_pair(item.first, item.second));
            }

            std::sort(
                result.begin(), 
                result.end(),
                [](const auto& lhs, const auto& rhs) {
                    return lhs.first < rhs.first;
                }
            );
        }
    }

    static void PopulateLinkedList()
    {
        std::println("PopulateLinkedList");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            std::list<std::pair<std::string, std::string>> result;

            for (const auto& item : sortedItems) {
                result.push_back(std::make_pair(item.first, item.second));
            }
        }
    }

    static void PopulateDeque()
    {
        std::println("PopulateDeque");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {

            std::deque<std::pair<std::string, std::string>> result;

            for (const auto& item : sortedItems) {
                result.push_back(std::make_pair(item.first, item.second));
            }
        }
    }
}

void benchmarking_stl_containers_populating()
{
    using namespace Benchmarking_STL_Containers_Populating;

    PopulateMap();
    PopulateUnorderedMap();
    PopulateVector();
    PopulateSortedVector();
    PopulateLinkedList();
    PopulateDeque();
}

// ===========================================================================
// End-of-File
// ===========================================================================

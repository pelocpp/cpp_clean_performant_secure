// =====================================================================================
// PMR_09.cpp // Polymorphic Memory Resources
// =====================================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <memory_resource>
#include <print>
#include <unordered_map>
#include <utility>
#include <vector>

// =====================================================================================
// Using a pool memory resource

static void test_pmr_09_01()
{
    std::pmr::pool_options options{
        .max_blocks_per_chunk = 64,
        .largest_required_pool_block = 128
    };

    std::pmr::unsynchronized_pool_resource pool{ options };

    std::pmr::pool_options actual{ pool.options() };

    std::println("largest_required_pool_block: {}", actual.largest_required_pool_block);
    std::println("max_blocks_per_chunk: {}", actual.max_blocks_per_chunk);
}

// =====================================================================================

#ifdef _DEBUG
static constexpr int OuterIterations = 2'000;
static constexpr int InnerIterations = 200;
static constexpr int NumPushBacks = 20;
#else
static constexpr int OuterIterations = 20000;
static constexpr int InnerIterations = 200;
static constexpr int NumPushBacks = 20;
#endif

// =====================================================================================

static void test_pmr_09_02_normal()
{
    std::println("Using std::vector<std::vector<std::size_t>>");

    for (std::size_t i{}; i != OuterIterations; ++i)
    {
        std::vector<std::vector<std::size_t>> container;

        for (std::size_t j{}; j != InnerIterations; ++j)
        {
            std::vector<std::size_t> v;

            for (std::size_t k{}; k != NumPushBacks; ++k) {
                v.push_back(k);
            }

            container.push_back(std::move(v));
        }
    }
}

static void test_pmr_09_02_pmr()
{
    std::println("Using std::pmr::vector<std::pmr::vector<std::size_t>>");

    std::pmr::unsynchronized_pool_resource pool;

    for (std::size_t i{}; i != OuterIterations; ++i)
    {
        {
            std::pmr::vector<std::pmr::vector<std::size_t>> container(&pool);

            for (std::size_t j{}; j != InnerIterations; ++j)
            {
                std::pmr::vector<std::size_t> v(&pool);

                for (std::size_t k{}; k != NumPushBacks; ++k) {
                    v.push_back(k);
                }

                container.push_back(std::move(v));
            }
        }

        pool.release(); // reuse memory for next iteration
    }
}

static void test_pmr_09_02_benchmark()
{
    std::cout << "20: Start" << std::endl;

    {
        ScopedTimer watch{};
        test_pmr_09_02_normal();
    }

    {
        ScopedTimer watch{};
        test_pmr_09_02_pmr();
    }

    std::cout << "20: Done." << std::endl;
}

static void test_pmr_09_03_crash_01()
{
    std::pmr::unsynchronized_pool_resource pool;

    std::pmr::vector<std::size_t> container(&pool);

    for (std::size_t k{}; k != 10; ++k)
    {
        container.push_back(k);
    }

    pool.release(); // reuse memory for job
}

static void test_pmr_09_03_nocrash_02()
{
    std::pmr::unsynchronized_pool_resource pool;

    {
        std::pmr::vector<std::size_t> container(&pool);

        for (std::size_t k{}; k != 10; ++k)
        {
            container.push_back(k);
        }
    }

    pool.release(); // reuse memory for next job
}

// =====================================================================================
// Histogram example: using a pool memory resource

static constexpr int TopWords = 2;

namespace std
{
    template<>
    struct greater<std::pair<std::string, std::size_t>>
    {
        bool operator() (const std::pair<std::string, std::size_t>& lhs, const std::pair<std::string, std::size_t>& rhs) const {
            return lhs.second > rhs.second;
        }
    };
}

class Histogram
{
private:
    std::unordered_map<std::string, std::size_t> m_frequenciesMap;

public:
    void add(const std::string& word) {
        // if word does not exist, it is automatically inserted with value 0
        ++m_frequenciesMap[word];
    }

    void printTopScore(std::size_t top) const {

        std::vector<std::pair<std::string, std::size_t>> popular(m_frequenciesMap.begin(), m_frequenciesMap.end());

        std::partial_sort(popular.begin(), popular.begin() + top, popular.end(), std::greater{});

        for (std::size_t n{}; const auto& elem : popular) {
            std::println("{}: {}", elem.first, elem.second);

            ++n;
            if (n == top)
                break;
        }
    }

    void readWords() {

        std::string m_fileName{ "LoremIpsumHuge.txt" };

        if (m_fileName.empty()) {
            std::println("No Filename specified!");
            return;
        }

        std::ifstream file{ m_fileName.data() };
        if (!file.good()) {
            std::println("File not found!");
            return;
        }

        std::println("File {}", m_fileName);
        std::println("Starting [Standard Classes] ...");

        std::string line;
        while (std::getline(file, line))
        {
            // process single line
            std::string_view sv{ line };

            std::size_t begin{};
            std::size_t end{};

            while (end != sv.size()) {

                while (std::isalpha(sv[end]))
                    ++end;

                std::string_view word{ sv.substr(begin, end - begin) };

                std::string s{ word };
                if (std::isupper(s[0])) {
                    s[0] = std::tolower(s[0]);
                }

                add(s);

                while (end != sv.size() && (sv[end] == ' ' || sv[end] == '.' || sv[end] == ','))
                    ++end;

                begin = end;
            }
        }

        std::println("Done Reading Dictionary");
    }
};

static void test_pmr_09_04_histogram()
{
    Histogram histo{};

    {
        ScopedTimer watch{};
        histo.readWords();
        histo.printTopScore(TopWords);
    }

    std::println();
}

// =====================================================================================

namespace std
{
    template<>
    struct greater<std::pair<std::pmr::string, std::size_t>>
    {
        bool operator() (const std::pair<std::pmr::string, size_t>& lhs, const std::pair<std::pmr::string, std::size_t>& rhs) const {
            return lhs.second > rhs.second;
        }
    };
}

class HistogramPMR
{
private:
    std::pmr::unordered_map<std::pmr::string, std::size_t> m_frequenciesMap;

public:
    HistogramPMR(const std::pmr::polymorphic_allocator<>& alloc = {})
        : m_frequenciesMap{ alloc }
    {
    }

    void add(const std::pmr::string& word) {
        // if word does not exist, it is automatically inserted with value 0
        ++m_frequenciesMap[word];
    }

    void printTopScore(std::size_t top) const {

        std::pmr::vector<std::pair<std::pmr::string, std::size_t>> popular(m_frequenciesMap.begin(), m_frequenciesMap.end());

        std::partial_sort(popular.begin(), popular.begin() + top, popular.end(), std::greater{});

        for (std::size_t n{}; const auto& elem : popular)
        {
            std::println("{}: {}", elem.first, elem.second);

            ++n;
            if (n == top)
                break;
        }
    }

    void readWords() {

        std::string m_fileName{ "LoremIpsumHuge.txt" };

        if (m_fileName.empty()) {
            std::println("No Filename specified!");
            return;
        }

        std::ifstream file{ m_fileName.data() };
        if (!file.good()) {
            std::println("File not found!");
            return;
        }

        std::println("File {}", m_fileName);
        std::println("Starting [PMR Classes] ...");

        std::string line;
        while (std::getline(file, line))
        {
            // process single line
            std::string_view sv{ line };

            std::size_t begin{};
            std::size_t end{};

            while (end != sv.size()) {

                while (std::isalpha(sv[end]))
                    ++end;

                std::string_view word{ sv.substr(begin, end - begin) };

                std::pmr::string s{ word };
                if (std::isupper(s[0])) {
                    s[0] = std::tolower(s[0]);
                }

                add(s);

                while (end != sv.size() && (sv[end] == ' ' || sv[end] == '.' || sv[end] == ','))
                    ++end;

                begin = end;
            }
        }

        std::println("Done Reading Dictionary");
    }
};

static void test_pmr_09_04_histogram_pmr()
{
    std::pmr::unsynchronized_pool_resource res;
    HistogramPMR histo{ &res };

    {
        ScopedTimer watch{};
        histo.readWords();
        histo.printTopScore(TopWords);
    }

    std::println();
}

// =====================================================================================

void test_pmr_09()
{
    test_pmr_09_01();
    
    test_pmr_09_02_benchmark();

    test_pmr_09_03_crash_01();     // crashes by design
    test_pmr_09_03_nocrash_02();

    test_pmr_09_04_histogram();
    test_pmr_09_04_histogram_pmr();
}

// =====================================================================================
// End-of-File
// =====================================================================================

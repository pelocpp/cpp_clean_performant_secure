// ===========================================================================
// DataStructures.cpp
// ===========================================================================

#include <array>
#include <fstream>
#include <print>
#include <queue>
#include <stack>
#include <string>
#include <vector>

namespace DataStructuresAndAlgorithms {

    namespace Vectors {

        static void test_vector()
        {
            {
                auto vec = std::vector{ 1, 2, 3 };

                vec.push_back(4);
                vec.push_back(5);

                std::println("Size: {} - Capacity: {}", vec.size(), vec.capacity());
            }

            {
                auto vec = std::vector{ 1, 2, 3, 4, 5 };
                for (auto elem : vec) {
                    std::print("{} ", elem);
                }
                std::println();
            }

            {
                auto vec = std::vector{ -1, 5, 2, -3, 4, -5, 5 };

                std::erase(vec, 5);                                 // v: [-1, 2, -3, 4, -5]
                for (auto elem : vec) {
                    std::print("{} ", elem);
                }
                std::println();

                std::erase_if(vec, [] (auto x) { return x < 0; });  // v: [2, 4]
                for (auto elem : vec) {
                    std::print("{} ", elem);
                }
                std::println();
            }
        }

        static auto print(const std::array<int, 10>& a) {

            for (auto elem : a) {
                std::print("{} ", elem);
            }
        }

        static void test_array()
        {
            {
                auto a = std::array<int, 5>{ 1, 2, 3, 4, 5 };

                auto b = std::array{ 1, 2, 3, 4, 5 };
            }

            {
                auto a = std::array<int, 5>{ 1, 2, 3, 4, 5 };

                for (auto elem : a) {
                    std::print("{} ", elem);
                }
            }

            {
                auto a = std::array<int, 10>{};
                auto b = std::array<int, 20>{};

                print(a);
                // print(b); // does not compile, print requires an int array of size 10
            }
        }
    }

    namespace Strings {

        static void test_strings() {

            auto in = std::ifstream{ "../Data_Structures_and_Algorithms/Program.cpp", std::ios::binary | std::ios::ate };

            if (in.is_open()) {
                auto size = in.tellg();
                auto content = std::string(size, '\0');
                in.seekg(0);
                in.read(&content[0], size);   // "content" now contains the entire file
            }
        }
    }

    namespace Stacks {

        static void test_stack()
        {
            std::stack<int> s;

            s.push(1);
            s.push(2);
            s.push(3);
            s.push(4);
            s.push(5);

            std::println("Size of stack: {} ", s.size());
            std::println("Elements:");

            while (!s.empty()) {
                std::print("{} ", s.top());
                s.pop();
            }
        }
    }

    namespace Queues {

        static void test_queue()
        {
            std::queue<int> q;

            q.push(1);
            q.push(2);
            q.push(3);
            q.push(4);
            q.push(5);

            std::println("Size of queue: {} ", q.size());
            std::println("Elements:");

            while (! q.empty()) {
                std::print("{} ", q.front());
                q.pop();
            }
        }
    }

    namespace PriorityQueues {

        static void test_priority_queue()
        {
            std::priority_queue<int> pq;

            pq.push(3);
            pq.push(1);
            pq.push(2);
            pq.push(5);
            pq.push(4);

            std::println("Size of priority queue: {} ", pq.size());
            std::println("Elements:");

            while (! pq.empty()) {
                std::print("{} ", pq.top());
                pq.pop();
            }
        }
    }

    namespace FlatMap {

        //void xxx() {
        //    std::flat_map<int> fm;
        //}
    }
}

// =================================================================

void test_data_structures()
{
    //using namespace DataStructuresAndAlgorithms::Vectors;
    //test_vector();
    //test_array();

    //using namespace DataStructuresAndAlgorithms::Strings;
    //test_strings();

    //using namespace DataStructuresAndAlgorithms::Stacks;
    //test_stack();

    //using namespace DataStructuresAndAlgorithms::Queues;
    //test_queue();

    using namespace DataStructuresAndAlgorithms::PriorityQueues;
    test_priority_queue();
}

// ===========================================================================
// End-of-File
// ===========================================================================

// ===========================================================================
// Caches.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <new>
#include <numeric>
#include <print>
#include <random>
#include <thread>
#include <vector>

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <processthreadsapi.h>

namespace DataStructuresAndAlgorithms {

    namespace CacheLinesAndCacheSizes {

        static void test_examine_cache_line_size() {

            constexpr auto cachelineSize = std::hardware_destructive_interference_size;

            std::println("Cache Line Size: {}", cachelineSize);
        }

        static void test_examine_l1_cache_size() {

            // https://gist.github.com/kimwalisch/16c34ae16447b245464a

            typedef BOOL(WINAPI* LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

            auto handle = GetModuleHandle(L"kernel32");

            auto glpi = (LPFN_GLPI)GetProcAddress(handle, "GetLogicalProcessorInformation");
            if (glpi == NULL)
                return;

            DWORD bufferBytes = 0;
            int cacheSize = 0;

            // calculate buffer length
            BOOL ret = glpi(0, &bufferBytes);

            std::size_t size = bufferBytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

            SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = new SYSTEM_LOGICAL_PROCESSOR_INFORMATION[size];

            // retrieve array of SYSTEM_LOGICAL_PROCESSOR_INFORMATION structures
            ret = glpi(buffer, &bufferBytes);

            for (std::size_t i = 0; i < size; i++)
            {
                if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
                {
                    cacheSize = (int)buffer[i].Cache.Size;
                    break;
                }
            }

            delete[] buffer;

            auto cacheSizeKB = cacheSize / 1024;
            std::println("L1 Cache Size: {} kB", cacheSizeKB);
        }
    }

    namespace CacheMisses {

        constexpr auto capacityL1CacheSize = 32768; // L1 Data Cache Size

        constexpr auto Size = capacityL1CacheSize / sizeof(int);

        using MatrixType = std::array<std::array<size_t, Size>, Size>;

        static MatrixType matrix;

        static auto initMatrixFast(MatrixType& matrix) {

            ScopedTimer watch{};

            size_t value{};

            for (size_t i{}; i != Size; ++i) {
                for (size_t j{}; j != Size; ++j) {
                    // matrix[i][j] = value++;           // no "cache thrashing"
                    matrix[i][j] = (i * Size) + j;
                }
            }
        }

        static auto initMatrixSlow(MatrixType& matrix) {

            ScopedTimer watch{};

            size_t value{};

            for (size_t i{}; i != Size; ++i) {
                for (size_t j{}; j != Size; ++j) {
                    matrix[j][i] = (j * Size) + i;    // demonstrating "cache thrashing" // note: matrix element needs to be calculated differently
                    //matrix[j][i] = value++;
                }
            }
        }

        static void test_cache_thrashing() {
            initMatrixFast(matrix);
            initMatrixFast(matrix);
            initMatrixFast(matrix);
            initMatrixFast(matrix);
        
            initMatrixSlow(matrix);
            initMatrixSlow(matrix);
            initMatrixSlow(matrix);
            initMatrixSlow(matrix);
        }
    }

    namespace CacheMisses_False_Sharing {

        const std::size_t Iterations = 100'000'000;  // 10000000
        // const std::size_t Iterations = 10;  // 

        struct Data {
            int x{};
            int y{};
        };

        struct DataWithAlignment {
            alignas(std::hardware_destructive_interference_size) int x{};
            alignas(std::hardware_destructive_interference_size) int y{};
        };

        template <typename TData>
        static void updateData(TData& data, int id) {

            for (int i = 0; i < Iterations; ++i) {
                if (i % 2 == 0) {
                    data.x += id;
                }
                else {
                    data.y += id;
                }
            }
        }

        static void test_false_sharing_with_false_sharing()
        {
            Data data1;
            Data data2;

            std::thread t1(updateData<Data>, std::ref(data1), 1);
            std::thread t2(updateData<Data>, std::ref(data2), 2);

            {
                ScopedTimer watch{};

                t1.join();
                t2.join();
            }
        }

        static void test_false_sharing_no_false_sharing()
        {
            DataWithAlignment data1;
            DataWithAlignment data2;

            std::thread t1(updateData<DataWithAlignment>, std::ref(data1), 1);
            std::thread t2(updateData<DataWithAlignment>, std::ref(data2), 2);

            {
                ScopedTimer watch{};

                t1.join();
                t2.join();
            }
        }

        static void test_false_sharing_short_demo()
        {
            test_false_sharing_with_false_sharing();
            test_false_sharing_with_false_sharing();
            test_false_sharing_with_false_sharing();
            test_false_sharing_with_false_sharing();
            test_false_sharing_with_false_sharing();

            std::println();

            test_false_sharing_no_false_sharing();
            test_false_sharing_no_false_sharing();
            test_false_sharing_no_false_sharing();
            test_false_sharing_no_false_sharing();
            test_false_sharing_no_false_sharing();
        }
    }

    namespace CacheMisses_False_Sharing {

        // https://curiouslyrecurringthoughts.home.blog/2019/06/10/c17-and-false-sharing/

#define FALSE_SHARING false

        static void test_false_sharing_long_demo() {

            // constexpr std::size_t numProcessors = 8;
            // constexpr std::size_t numIter = 40'000'000;

            constexpr std::size_t numProcessors = 8;
            constexpr std::size_t numIter = 10;

#if FALSE_SHARING
            std::cout << "With false sharing \n";
            struct resultType {
                int val;
            };
#else
            std::cout << "Without false sharing\n";
            struct resultType {
                alignas(std::hardware_destructive_interference_size) int val;
            };
#endif
            std::println("Sizeof: {}", sizeof(struct resultType));

            std::array<resultType, numProcessors> results{ 0 };
            std::array<std::thread, numProcessors> threads;

            {
                ScopedTimer watch{};

                for (std::size_t i = 0; i < numProcessors; ++i) {

                    auto& result = results[i];

                    threads[i] = std::thread{
                        [&result, numIter]() mutable {
                            for (std::size_t j = 0; j < numIter; ++j) {
                                result.val = (result.val + std::rand() % 10) % 50;
                                // std::println("val: {}", result.val);
                            }
                        }
                    };
                }

                std::for_each(
                    begin(threads),
                    end(threads),
                    [](std::thread& t) { t.join(); }
                );
            }

            const auto res = std::accumulate(
                cbegin(results),
                cend(results),
                resultType{ 0 },
                [](const resultType a, const resultType b) {
                    auto s = a.val + b.val;
                    return resultType{ s };
                }
            );

            std::println("Result: {}", res.val);
        }
    }

    namespace CacheMisses_False_Sharing {

        // Source Code stammt von hier:
        // https://stackoverflow.com/questions/39680206/understanding-stdhardware-destructive-interference-size-and-stdhardware-cons

        // https://stackoverflow.com/questions/57606409/what-is-true-sharing

        // http://thebeardsage.com/true-sharing-false-sharing-and-ping-ponging/

        //// !!! YOU MUST UPDATE THIS TO BE ACCURATE !!!
        //constexpr std::size_t hardware_destructive_interference_size = 64;
        //
        //// !!! YOU MUST UPDATE THIS TO BE ACCURATE !!!
        //constexpr std::size_t hardware_constructive_interference_size = 64;

        constexpr unsigned kTimingTrialsToComputeAverage = 100;
        // constexpr unsigned kInnerLoopTrials = 1000000;
        constexpr unsigned kInnerLoopTrials = 1000000;

        typedef unsigned useless_result_t;
        typedef double elapsed_secs_t;

        //////// CODE TO BE SAMPLED:

        // wraps an int, default alignment allows false-sharing
        struct naive_int {
            int value;
        };
        static_assert(alignof(naive_int) < std::hardware_destructive_interference_size, "");

        // wraps an int, cache alignment prevents false-sharing
        struct cache_int {
            alignas(std::hardware_destructive_interference_size) int value;
        };
        static_assert(alignof(cache_int) == std::hardware_destructive_interference_size, "");

        // wraps a pair of int, purposefully pushes them too far apart for true-sharing
        struct bad_pair {
            int first;
            char padding[std::hardware_constructive_interference_size];
            int second;
        };
        static_assert(sizeof(bad_pair) > std::hardware_constructive_interference_size, "");

        // wraps a pair of int, ensures they fit nicely together for true-sharing
        struct good_pair {
            int first;
            int second;
        };
        static_assert(sizeof(good_pair) <= std::hardware_constructive_interference_size, "");

        // accesses a specific array element many times
        template <typename T, typename Latch>
        useless_result_t sample_array_threadfunc(
            Latch& latch,
            unsigned thread_index,
            T& vec) {
            // prepare for computation
            std::random_device rd;
            std::mt19937 mt{ rd() };
            std::uniform_int_distribution<int> dist{ 0, 4096 };

            auto& element = vec[vec.size() / 2 + thread_index];

            latch.count_down_and_wait();

            // compute
            for (unsigned trial = 0; trial != kInnerLoopTrials; ++trial) {
                element.value = dist(mt);
            }

            return static_cast<useless_result_t>(element.value);
        }

        // accesses a pair's elements many times
        template <typename T, typename Latch>
        useless_result_t sample_pair_threadfunc(
            Latch& latch,
            unsigned thread_index,
            T& pair) {
            // prepare for computation
            std::random_device rd;
            std::mt19937 mt{ rd() };
            std::uniform_int_distribution<int> dist{ 0, 4096 };

            latch.count_down_and_wait();

            // compute
            for (unsigned trial = 0; trial != kInnerLoopTrials; ++trial) {
                pair.first = dist(mt);
                pair.second = dist(mt);
            }

            return static_cast<useless_result_t>(pair.first) +
                static_cast<useless_result_t>(pair.second);
        }

        //////// UTILITIES:

        // utility: allow threads to wait until everyone is ready
        class threadlatch {
        public:
            explicit threadlatch(const std::size_t count) :
                count_{ count }
            {
            }

            void count_down_and_wait() {
                std::unique_lock<std::mutex> lock{ mutex_ };
                if (--count_ == 0) {
                    cv_.notify_all();
                }
                else {
                    cv_.wait(lock, [&] { return count_ == 0; });
                }
            }

        private:
            std::mutex mutex_;
            std::condition_variable cv_;
            std::size_t count_;
        };

        // utility: runs a given function in N threads
        std::tuple<useless_result_t, elapsed_secs_t> run_threads(
            const std::function<useless_result_t(threadlatch&, unsigned)>& func,
            const unsigned num_threads) {
            threadlatch latch{ num_threads + 1 };

            std::vector<std::future<useless_result_t>> futures;
            std::vector<std::thread> threads;
            for (unsigned thread_index = 0; thread_index != num_threads; ++thread_index) {
                std::packaged_task<useless_result_t()> task{
                    std::bind(func, std::ref(latch), thread_index)
                };

                futures.push_back(task.get_future());
                threads.push_back(std::thread(std::move(task)));
            }

            const auto starttime = std::chrono::high_resolution_clock::now();

            latch.count_down_and_wait();
            for (auto& thread : threads) {
                thread.join();
            }

            const auto endtime = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<
                std::chrono::duration<double>>(
                    endtime - starttime
                    ).count();

            useless_result_t result = 0;
            for (auto& future : futures) {
                result += future.get();
            }

            return std::make_tuple(result, elapsed);
        }

        // utility: sample the time it takes to run func on N threads
        static void run_tests(
            const std::function<useless_result_t(threadlatch&, unsigned)>& func,
            const unsigned num_threads) {
            useless_result_t final_result = 0;
            double avgtime = 0.0;
            for (unsigned trial = 0; trial != kTimingTrialsToComputeAverage; ++trial) {
                const auto result_and_elapsed = run_threads(func, num_threads);
                const auto result = std::get<useless_result_t>(result_and_elapsed);
                const auto elapsed = std::get<elapsed_secs_t>(result_and_elapsed);

                final_result += result;
                avgtime = (avgtime * trial + elapsed) / (trial + 1);
            }

            std::cout
                << "Average time: " << avgtime
                << " seconds, useless result: " << final_result
                << std::endl;
        }

        static void test_cache_lines() {
            const auto cores = std::thread::hardware_concurrency();
            std::cout << "Hardware concurrency: " << cores << std::endl;

            std::cout << "sizeof(naive_int): " << sizeof(naive_int) << std::endl;
            std::cout << "alignof(naive_int): " << alignof(naive_int) << std::endl;
            std::cout << "sizeof(cache_int): " << sizeof(cache_int) << std::endl;
            std::cout << "alignof(cache_int): " << alignof(cache_int) << std::endl;
            std::cout << "sizeof(bad_pair): " << sizeof(bad_pair) << std::endl;
            std::cout << "alignof(bad_pair): " << alignof(bad_pair) << std::endl;
            std::cout << "sizeof(good_pair): " << sizeof(good_pair) << std::endl;
            std::cout << "alignof(good_pair): " << alignof(good_pair) << std::endl;

            {
                std::cout << "Running naive_int test." << std::endl;

                std::vector<naive_int> vec;
                vec.resize((1u << 28) / sizeof(naive_int));  // allocate 256 mibibytes

                run_tests([&](threadlatch& latch, unsigned thread_index) {
                    return sample_array_threadfunc(latch, thread_index, vec);
                    }, cores);
            }
            {
                std::cout << "Running cache_int test." << std::endl;

                std::vector<cache_int> vec;
                vec.resize((1u << 28) / sizeof(cache_int));  // allocate 256 mibibytes

                run_tests([&](threadlatch& latch, unsigned thread_index) {
                    return sample_array_threadfunc(latch, thread_index, vec);
                    }, cores);
            }
            {
                std::cout << "Running bad_pair test." << std::endl;

                bad_pair p;

                run_tests([&](threadlatch& latch, unsigned thread_index) {
                    return sample_pair_threadfunc(latch, thread_index, p);
                    }, cores);
            }
            {
                std::cout << "Running good_pair test." << std::endl;

                good_pair p;

                run_tests([&](threadlatch& latch, unsigned thread_index) {
                    return sample_pair_threadfunc(latch, thread_index, p);
                    }, cores);
            }
        }
    }
}

// =================================================================

void test_caches()
{
    using namespace DataStructuresAndAlgorithms;

    //CacheLinesAndCacheSizes::test_examine_cache_line_size();
    //CacheLinesAndCacheSizes::test_examine_l1_cache_size();

    CacheMisses::test_cache_thrashing();

    //CacheMisses_False_Sharing::test_cache_lines();

    //CacheMisses_False_Sharing::test_false_sharing_short_demo();  // short demo
    //CacheMisses_False_Sharing::test_false_sharing_long_demo();  // long demo
}

// ===========================================================================
// End-of-File
// ===========================================================================

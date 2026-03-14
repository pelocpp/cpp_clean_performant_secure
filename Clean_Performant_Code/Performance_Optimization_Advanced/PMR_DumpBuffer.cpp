// =====================================================================================
// PMR_DumpBuffer.cpp
// =====================================================================================

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <print>
#include <span>

// =====================================================================================
// dumpBuffer

void dumpBuffer(std::span<std::uint8_t> buffer)
{
    std::println("===========================================================================");
    std::println("Memory Dump");
    std::println("Buffer: {:#X}", reinterpret_cast<intptr_t>(buffer.data()));
    std::println("Bytes:  {}", buffer.size());
    std::println("---------------------------------------------------------------------------");

    constexpr auto elementsPerLine{ static_cast<std::size_t>(16) };

    std::size_t idx{};

    while (idx < buffer.size()) {

        std::size_t first{ idx };
        std::size_t last{ idx + std::min(elementsPerLine, buffer.size() - first) };

        std::print("{:04x} |", idx);

        for (std::size_t k{ first }; k != last; ++k) {

            if (k % 8 == 0) {
                std::print(" ");
            }

            std::print("{:02x} ", buffer[k]);
        }

        // complete last line with blanks, if necessary
        if (last != first + elementsPerLine) {

            std::size_t blanksNeeded{ first + elementsPerLine - last };

            if (blanksNeeded >= 8) {
                std::print(" ");
            }

            for (std::size_t k{ }; k != blanksNeeded; ++k) {

                std::print("   ");
            }
        }

        std::print("|");

        for (std::size_t k{ first }; k != last; ++k) {

            if (k % 8 == 0) {
                std::print(" ");
            }

            if (std::isprint(buffer[k])) {
                std::print("{}", static_cast<char> (buffer[k]));
            }
            else {
                std::print(".");
            }
        }

        idx += elementsPerLine;

        std::println();
    }

    std::println("---------------------------------------------------------------------------");
}

static void test_pmr_dump_buffer_01()
{
    static constexpr std::size_t NumBytes = 32;

    static std::array<std::uint8_t, NumBytes> g_memory{};

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::span span{ g_memory };

    dumpBuffer(span);
}

static void test_pmr_dump_buffer_02()
{
    static constexpr std::size_t NumBytes = 6;

    static std::array<std::uint8_t, NumBytes> g_memory{};

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::span span{ g_memory };

    dumpBuffer(span);
}


void test_pmr_dump_buffer()
{
    test_pmr_dump_buffer_01();
    test_pmr_dump_buffer_02();
}

// =====================================================================================
// End-of-File
// =====================================================================================

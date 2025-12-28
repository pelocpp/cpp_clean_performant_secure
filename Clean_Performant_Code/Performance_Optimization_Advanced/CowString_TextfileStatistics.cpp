// =====================================================================================
// CowString_TextfileStatistics.cpp // Copy-on-Write
// =====================================================================================

#include "CowString_TextfileStatistics.h"

#include <print>   // std::println

constexpr std::string_view fileNameSmall       { "LoremIpsumSmall.txt" };
constexpr std::string_view fileNameLarge       { "LoremIpsumLarge.txt" };
constexpr std::string_view fileNameHuge        { "LoremIpsumHuge.txt" };
constexpr std::string_view fileNameVeryHuge    { "LoremIpsumVeryHuge.txt" };
constexpr std::string_view fileNameExtremeHuge { "LoremIpsumExtremeHuge.txt" };

void main_cow_textfile_statistics_01()
{
    TextfileStatistics stats;
    stats.setFilename(fileNameVeryHuge);
    stats.countWordFrequencies();
    std::println();
    stats.countWordFrequenciesCOW();
    std::println();
}

void main_cow_textfile_statistics_02()
{
    TextfileStatistics stats;
    stats.setFilename(fileNameVeryHuge);
    stats.computeMostFrequentWords();
    std::println();
    stats.computeMostFrequentWordsCOW();
    std::println();
}

// =====================================================================================
// End-of-File
// =====================================================================================

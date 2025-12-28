// =====================================================================================
// CowString_TextfileStatistics.h // Copy-on-Write
// =====================================================================================

#pragma once

#include <string_view>               // std::string_view

class TextfileStatistics
{
private:
    std::string_view                 m_fileName;

public:
    // c'tor
    TextfileStatistics               ();

    // getter / setter
    void setFilename                 (std::string_view fileName);
    std::string_view getFilename     ();

    // public interface
    void countWordFrequencies        ();
    void countWordFrequenciesCOW     ();
    void computeMostFrequentWords    ();
    void computeMostFrequentWordsCOW ();
};

// =====================================================================================
// End-of-File
// =====================================================================================

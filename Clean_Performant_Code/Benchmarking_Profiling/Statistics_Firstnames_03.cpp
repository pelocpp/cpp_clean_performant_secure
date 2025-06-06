// ===========================================================================
// Statistics_Firstnames_03.cpp // Profiling
// From: Marc Gregoire, "Professional C++", 6.th Edition
// Variant 03: More improvements
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <fstream>
#include <map>
#include <print>
#include <stdexcept>
#include <string>

namespace NamesStatistics_V03 {

    // ===========================================================================
    // Types

    class NameDB
    {
    public:

        NameDB();

        // Reads list of names in nameFile to populate the database.
        // Throws invalid_argument if nameFile cannot be opened or read.
        void init(const std::string& nameFile);

        // Returns the rank of the name (1st, 2nd, etc).
        // Returns -1 if the name is not found.
        int getNameRank(const std::string& name) const;

        // Returns the number of entries with a given name.
        // Returns -1 if the name is not found.
        int getAbsoluteNumber(const std::string& name) const;

    private:
        std::map<std::string, int> m_names;

        // Helper member functions
        //bool incrementIfExists(const std::string& name);
        //void addNewName(const std::string& name);
    };

    // ===========================================================================
    // Implementation

    NameDB::NameDB() {}

    // Reads list of names in nameFile to populate the database.
    // Throws invalid_argument if nameFile cannot be opened or read.
    void NameDB::init(const std::string& nameFile) {
        // Open the file and check for errors.
        std::ifstream inputFile{ nameFile };
        if (!inputFile) {
            throw std::invalid_argument{ "Unable to open file" };
        }

        // Read the names one at a time.
        std::string name;
        while (inputFile >> name) {
            m_names[name] += 1;
        }
    }

    // Returns the rank of the name.
    // First looks up the name to obtain the number of babies with that name.
    // Then iterates through all the names, counting all the names with a higher
    // count than the specified name. Returns that count as the rank.
    int NameDB::getNameRank(const std::string& name) const
    {
        int num{ getAbsoluteNumber(name) };

        // Check if we found the name.
        if (num == -1) {
            return -1;
        }

        // Now count all the names in the map that have 
        // count higher than this one. If no name has a higher count,
        // this name is rank number 1. Every name with a higher count
        // decreases the rank of this name by 1.
        int rank{ 1 };
        for (auto& entry : m_names) {
            if (entry.second > num) {
                ++rank;
            }
        }

        return rank;
    }

    // Returns the count associated with the given name.
    int NameDB::getAbsoluteNumber(const std::string& name) const
    {
        auto res{ m_names.find(name) };
        if (res != end(m_names)) {
            return res->second;
        }

        return -1;
    }
}

// ===========================================================================
// Test Frame

void performance_profiling_names_statistics_03()
{
    using namespace NamesStatistics_V03;

    ScopedTimer watch{};

    NameDB boys{};
    boys.init("Names_Long.txt");

    std::println("{}", boys.getNameRank("Daniel"));
    std::println("{}", boys.getNameRank("Jacob"));
    std::println("{}", boys.getNameRank("William"));
}

// ===========================================================================
// End-of-File
// ===========================================================================

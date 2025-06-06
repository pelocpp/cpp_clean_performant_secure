// ===========================================================================
// Statistics_Firstnames_01.cpp // Profiling
// From: Marc Gregoire, "Professional C++", 6.th Edition
// ===========================================================================

// ===========================================================================
// Types

#include <fstream>
#include <print>
#include <string>
#include <vector>

namespace NamesStatistics_V01 {

    class NameDB
    {
    public:

        NameDB();

        // Reads list of names in nameFile to populate the database.
        // Throws invalid_argument if nameFile cannot be opened or read.
        bool init(const std::string& nameFile);

        // Returns the rank of the name (1st, 2nd, etc).
        // Returns -1 if the name is not found.
        int getNameRank(const std::string& name) const;

        // Returns the number of entries with a given name.
        // Returns -1 if the name is not found.
        int getAbsoluteNumber(const std::string& name) const;

    private:
        std::vector<std::pair<std::string, int>> m_names;

        // Helper member functions
        bool nameExists(const std::string& name) const;
        void incrementNameCount(const std::string& name);
        void addNewName(const std::string& name);
    };

    // ===========================================================================
    // Implementation

    NameDB::NameDB() {}

    // Reads the names from the file and populates the database.
    // The database is a vector of name/count pairs, storing the
    // number of times each name shows up in the raw data.
    bool NameDB::init(const std::string& nameFile)
    {
        // Open the file and check for errors.
        std::ifstream inputFile{ nameFile };
        if (!inputFile) {
            throw std::invalid_argument{ "Unable to open file" };
        }

        // Read the names one at a time.
        std::string name;
        while (inputFile >> name) {
            // Look up the name in the database so far.
            if (nameExists(name)) {
                // If the name exists in the database, just increment the count.
                incrementNameCount(name);
            }
            else {
                // If the name doesn't yet exist, add it with a count of 1.
                addNewName(name);
            }
        }

        return true;
    }


    // Returns true if the name exists in the database, false otherwise.
    bool NameDB::nameExists(const std::string& name) const
    {
        // Iterate through the vector of names looking for the name.
        for (auto& entry : m_names) {
            if (entry.first == name) {
                return true;
            }
        }
        return false;
    }

    // Precondition: name exists in the vector of names.
    // Postcondition: the count associated with name is incremented.
    void NameDB::incrementNameCount(const std::string& name)
    {
        for (auto& entry : m_names) {
            if (entry.first == name) {
                entry.second += 1;
                return;
            }
        }
    }

    // Adds a new name to the database.
    void NameDB::addNewName(const std::string& name)
    {
        m_names.emplace_back(name, 1);
    }

    // Returns the rank of the name.
    // First looks up the name to obtain the number of babies with that name.
    // Then iterates through all the names, counting all the names with a higher
    // count than the specified name. Returns that count as the rank.
    int NameDB::getNameRank(const std::string& name) const
    {
        // Make use of the getAbsoluteNumber() member function.
        int num{ getAbsoluteNumber(name) };

        // Check if we found the name.
        if (num == -1) {
            return -1;
        }

        // Now count all the names in the vector that have a
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
        for (auto& entry : m_names) {
            if (entry.first == name) {
                return entry.second;
            }
        }
        return -1;
    }
}

// ===========================================================================
// Test Frame

void performance_profiling_names_statistics_01()
{
    using namespace NamesStatistics_V01;

    NameDB boys{};
    boys.init("Names_Long.txt");

    std::println("{}", boys.getNameRank("Daniel"));
    std::println("{}", boys.getNameRank("Jacob"));
    std::println("{}", boys.getNameRank("William"));
}

// ===========================================================================
// End-of-File
// ===========================================================================

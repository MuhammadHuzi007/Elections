#ifndef CSV_READER_H
#define CSV_READER_H

#include "ElectionData.h"
#include <string>
#include <vector>

/**
 * @brief Utility class for reading election data from CSV files
 * 
 * Handles parsing of CSV files and populating ElectionData structure
 */
class CSVReader {
public:
    /**
     * @brief Read election data from a CSV file
     * @param filename Path to CSV file
     * @param data Reference to ElectionData object to populate
     * @return true if successful, false otherwise
     * Time Complexity: O(n) where n is number of lines in file
     */
    static bool readFromFile(const std::string& filename, ElectionData& data);

    /**
     * @brief Parse a single CSV line into an ElectionRecord
     * @param line CSV line string
     * @return ElectionRecord object
     */
    static ElectionRecord parseLine(const std::string& line);

private:
    /**
     * @brief Split a string by delimiter
     * @param str String to split
     * @param delimiter Delimiter character
     * @return Vector of tokens
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);

    /**
     * @brief Trim whitespace from string
     * @param str String to trim
     * @return Trimmed string
     */
    static std::string trim(const std::string& str);
};

#endif // CSV_READER_H


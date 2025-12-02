#ifndef CSV_READER_H
#define CSV_READER_H

#include "ElectionData.h"
#include <string>
#include <vector>

/**
 * @brief Simple class for reading CSV files
 */
class CSVReader {
public:
    /**
     * @brief Read election data from a CSV file
     */
    static bool readFromFile(const std::string& filename, ElectionData& data);

    /**
     * @brief Parse one CSV line into an ElectionRecord
     */
    static ElectionRecord parseLine(const std::string& line);

private:
    /**
     * @brief Split a string by comma
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);

    /**
     * @brief Remove spaces from start and end
     */
    static std::string trim(const std::string& str);

    /**
     * @brief Convert string to integer (simple version)
     */
    static int stringToInt(const std::string& str);
};

#endif // CSV_READER_H


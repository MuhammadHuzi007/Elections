#ifndef ELECTION_DATA_H
#define ELECTION_DATA_H

#include "ElectionRecord.h"
#include <vector>
#include <string>

/**
 * @brief Simple data structure for storing and managing election data
 * 
 * Uses a simple vector to store all records.
 * All operations use linear search - easy to understand!
 * 
 * Time Complexity:
 * - Insert: O(1)
 * - Search: O(n) where n is number of records
 * - Get records: O(n)
 */
class ElectionData {
private:
    // Simple vector to store all records
    std::vector<ElectionRecord> records;

public:
    /**
     * @brief Add a new election record
     */
    void addRecord(const ElectionRecord& record);

    /**
     * @brief Get all records for a specific election (country + year)
     */
    std::vector<ElectionRecord> getElectionRecords(const std::string& country, int year);

    /**
     * @brief Get all records for a specific party in an election
     */
    std::vector<ElectionRecord> getPartyRecords(const std::string& country, int year, const std::string& party);

    /**
     * @brief Get total number of records
     */
    int getTotalRecords();

    /**
     * @brief Get all records
     */
    std::vector<ElectionRecord> getAllRecords();

    /**
     * @brief Clear all data
     */
    void clear();
};

#endif // ELECTION_DATA_H


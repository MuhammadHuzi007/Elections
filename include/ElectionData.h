#ifndef ELECTION_DATA_H
#define ELECTION_DATA_H

#include "ElectionRecord.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

/**
 * @brief Custom data structure for efficiently storing and managing election data
 * 
 * This class uses a combination of data structures:
 * - Vector for maintaining insertion order and fast iteration
 * - Hash maps for O(1) lookups by various keys
 * 
 * Time Complexity:
 * - Insert: O(1) average case
 * - Update: O(1) average case
 * - Retrieve: O(1) average case
 * - Statistics: O(n) where n is number of records
 * 
 * Space Complexity: O(n) where n is total number of records
 */
class ElectionData {
private:
    // Main storage - maintains insertion order
    std::vector<ElectionRecord> records;

    // Hash maps for fast lookups
    // Key: "country_year_constituency_candidate"
    std::unordered_map<std::string, size_t> recordIndexMap;

    // Key: "country_year"
    std::unordered_map<std::string, std::vector<size_t>> electionIndexMap;

    // Key: "country_year_party"
    std::unordered_map<std::string, std::vector<size_t>> partyIndexMap;

    // Key: "country_year_constituency"
    std::unordered_map<std::string, std::vector<size_t>> constituencyIndexMap;

    /**
     * @brief Generates a unique key for a record
     */
    std::string generateRecordKey(const std::string& country, int year,
                                  const std::string& constituency, 
                                  const std::string& candidate) const;

    /**
     * @brief Generates a key for election lookup
     */
    std::string generateElectionKey(const std::string& country, int year) const;

    /**
     * @brief Generates a key for party lookup
     */
    std::string generatePartyKey(const std::string& country, int year,
                                const std::string& party) const;

    /**
     * @brief Generates a key for constituency lookup
     */
    std::string generateConstituencyKey(const std::string& country, int year,
                                       const std::string& constituency) const;

public:
    /**
     * @brief Default constructor
     */
    ElectionData();

    /**
     * @brief Destructor
     */
    ~ElectionData();

    /**
     * @brief Insert a new election record
     * @param record The election record to insert
     * @return true if inserted successfully, false if record already exists
     * Time Complexity: O(1) average case
     */
    bool insertRecord(const ElectionRecord& record);

    /**
     * @brief Update an existing election record
     * @param record The updated election record
     * @return true if updated successfully, false if record doesn't exist
     * Time Complexity: O(1) average case
     */
    bool updateRecord(const ElectionRecord& record);

    /**
     * @brief Retrieve a specific record
     * @param country Country name
     * @param year Election year
     * @param constituency Constituency name
     * @param candidate Candidate name
     * @return Pointer to record if found, nullptr otherwise
     * Time Complexity: O(1) average case
     */
    const ElectionRecord* getRecord(const std::string& country, int year,
                                    const std::string& constituency,
                                    const std::string& candidate) const;

    /**
     * @brief Get all records for a specific election
     * @param country Country name
     * @param year Election year
     * @return Vector of records for that election
     * Time Complexity: O(k) where k is number of records for that election
     */
    std::vector<ElectionRecord> getElectionRecords(const std::string& country, 
                                                   int year) const;

    /**
     * @brief Get all records for a specific party in an election
     * @param country Country name
     * @param year Election year
     * @param party Party name
     * @return Vector of records for that party
     * Time Complexity: O(k) where k is number of records for that party
     */
    std::vector<ElectionRecord> getPartyRecords(const std::string& country,
                                                int year,
                                                const std::string& party) const;

    /**
     * @brief Get all records for a specific constituency
     * @param country Country name
     * @param year Election year
     * @param constituency Constituency name
     * @return Vector of records for that constituency
     * Time Complexity: O(k) where k is number of records for that constituency
     */
    std::vector<ElectionRecord> getConstituencyRecords(const std::string& country,
                                                       int year,
                                                       const std::string& constituency) const;

    /**
     * @brief Get total number of records
     * @return Total count
     * Time Complexity: O(1)
     */
    size_t getTotalRecords() const { return records.size(); }

    /**
     * @brief Get all records (for iteration)
     * @return Const reference to all records
     */
    const std::vector<ElectionRecord>& getAllRecords() const { return records; }

    /**
     * @brief Clear all data
     * Time Complexity: O(n)
     */
    void clear();
};

#endif // ELECTION_DATA_H


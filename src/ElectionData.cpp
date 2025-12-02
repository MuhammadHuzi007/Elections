#include "../include/ElectionData.h"

// Add a new record to the list
void ElectionData::addRecord(const ElectionRecord& record) {
    records.push_back(record);
}

// Get all records for a specific election (country + year)
std::vector<ElectionRecord> ElectionData::getElectionRecords(const std::string& country, int year) {
    std::vector<ElectionRecord> result;
    
    // Simple loop through all records
    for (int i = 0; i < records.size(); i++) {
        if (records[i].country == country && records[i].year == year) {
            result.push_back(records[i]);
        }
    }
    
    return result;
}

// Get all records for a specific party in an election
std::vector<ElectionRecord> ElectionData::getPartyRecords(const std::string& country, int year, const std::string& party) {
    std::vector<ElectionRecord> result;
    
    // Simple loop through all records
    for (int i = 0; i < records.size(); i++) {
        if (records[i].country == country && records[i].year == year && records[i].party == party) {
            result.push_back(records[i]);
        }
    }
    
    return result;
}

// Get total number of records
int ElectionData::getTotalRecords() {
    return records.size();
}

// Get all records
std::vector<ElectionRecord> ElectionData::getAllRecords() {
    return records;
}

// Clear all data
void ElectionData::clear() {
    records.clear();
}


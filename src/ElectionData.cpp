#include "../include/ElectionData.h"
#include <sstream>

ElectionData::ElectionData() {
    // Initialize with default capacity
    records.reserve(1000);
}

ElectionData::~ElectionData() {
    clear();
}

std::string ElectionData::generateRecordKey(const std::string& country, int year,
                                            const std::string& constituency,
                                            const std::string& candidate) const {
    std::ostringstream oss;
    oss << country << "_" << year << "_" << constituency << "_" << candidate;
    return oss.str();
}

std::string ElectionData::generateElectionKey(const std::string& country, int year) const {
    std::ostringstream oss;
    oss << country << "_" << year;
    return oss.str();
}

std::string ElectionData::generatePartyKey(const std::string& country, int year,
                                          const std::string& party) const {
    std::ostringstream oss;
    oss << country << "_" << year << "_" << party;
    return oss.str();
}

std::string ElectionData::generateConstituencyKey(const std::string& country, int year,
                                                  const std::string& constituency) const {
    std::ostringstream oss;
    oss << country << "_" << year << "_" << constituency;
    return oss.str();
}

bool ElectionData::insertRecord(const ElectionRecord& record) {
    std::string key = generateRecordKey(record.country, record.year,
                                        record.constituency, record.candidate);

    // Check if record already exists
    if (recordIndexMap.find(key) != recordIndexMap.end()) {
        return false; // Record already exists
    }

    // Add to main storage
    size_t index = records.size();
    records.push_back(record);

    // Update all index maps
    recordIndexMap[key] = index;

    std::string electionKey = generateElectionKey(record.country, record.year);
    electionIndexMap[electionKey].push_back(index);

    std::string partyKey = generatePartyKey(record.country, record.year, record.party);
    partyIndexMap[partyKey].push_back(index);

    std::string constituencyKey = generateConstituencyKey(record.country, record.year,
                                                         record.constituency);
    constituencyIndexMap[constituencyKey].push_back(index);

    return true;
}

bool ElectionData::updateRecord(const ElectionRecord& record) {
    std::string key = generateRecordKey(record.country, record.year,
                                       record.constituency, record.candidate);

    auto it = recordIndexMap.find(key);
    if (it == recordIndexMap.end()) {
        return false; // Record doesn't exist
    }

    size_t index = it->second;
    ElectionRecord& oldRecord = records[index];

    // Update the record
    oldRecord = record;

    // If party or constituency changed, update index maps
    // (In a more complex scenario, we'd need to remove from old indices)
    // For simplicity, we'll assume updates don't change these fields often

    return true;
}

const ElectionRecord* ElectionData::getRecord(const std::string& country, int year,
                                              const std::string& constituency,
                                              const std::string& candidate) const {
    std::string key = generateRecordKey(country, year, constituency, candidate);

    auto it = recordIndexMap.find(key);
    if (it == recordIndexMap.end()) {
        return nullptr;
    }

    return &records[it->second];
}

std::vector<ElectionRecord> ElectionData::getElectionRecords(const std::string& country,
                                                             int year) const {
    std::vector<ElectionRecord> result;
    std::string key = generateElectionKey(country, year);

    auto it = electionIndexMap.find(key);
    if (it != electionIndexMap.end()) {
        result.reserve(it->second.size());
        for (size_t index : it->second) {
            result.push_back(records[index]);
        }
    }

    return result;
}

std::vector<ElectionRecord> ElectionData::getPartyRecords(const std::string& country,
                                                          int year,
                                                          const std::string& party) const {
    std::vector<ElectionRecord> result;
    std::string key = generatePartyKey(country, year, party);

    auto it = partyIndexMap.find(key);
    if (it != partyIndexMap.end()) {
        result.reserve(it->second.size());
        for (size_t index : it->second) {
            result.push_back(records[index]);
        }
    }

    return result;
}

std::vector<ElectionRecord> ElectionData::getConstituencyRecords(const std::string& country,
                                                                int year,
                                                                const std::string& constituency) const {
    std::vector<ElectionRecord> result;
    std::string key = generateConstituencyKey(country, year, constituency);

    auto it = constituencyIndexMap.find(key);
    if (it != constituencyIndexMap.end()) {
        result.reserve(it->second.size());
        for (size_t index : it->second) {
            result.push_back(records[index]);
        }
    }

    return result;
}

void ElectionData::clear() {
    records.clear();
    recordIndexMap.clear();
    electionIndexMap.clear();
    partyIndexMap.clear();
    constituencyIndexMap.clear();
}


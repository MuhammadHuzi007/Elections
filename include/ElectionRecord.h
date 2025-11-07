#ifndef ELECTION_RECORD_H
#define ELECTION_RECORD_H

#include <string>

/**
 * @brief Represents a single election record for a candidate
 * 
 * This structure stores all information about a candidate's performance
 * in a specific constituency during an election.
 */
struct ElectionRecord {
    std::string country;
    int year;
    std::string constituency;
    std::string candidate;
    std::string party;
    int votes;
    bool elected;

    // Default constructor
    ElectionRecord() : year(0), votes(0), elected(false) {}

    // Parameterized constructor
    ElectionRecord(const std::string& country, int year, 
                   const std::string& constituency, const std::string& candidate,
                   const std::string& party, int votes, bool elected)
        : country(country), year(year), constituency(constituency),
          candidate(candidate), party(party), votes(votes), elected(elected) {}

    // Equality operator for comparisons
    bool operator==(const ElectionRecord& other) const {
        return country == other.country && year == other.year &&
               constituency == other.constituency && candidate == other.candidate;
    }

    // Less than operator for sorting
    bool operator<(const ElectionRecord& other) const {
        if (year != other.year) return year < other.year;
        if (country != other.country) return country < other.country;
        if (constituency != other.constituency) return constituency < other.constituency;
        return votes > other.votes; // Sort by votes descending
    }
};

#endif // ELECTION_RECORD_H


#ifndef ELECTION_ANALYZER_H
#define ELECTION_ANALYZER_H

#include "ElectionData.h"
#include <string>
#include <vector>
#include <map>

/**
 * @brief Structure to hold party statistics
 */
struct PartyStats {
    std::string party;
    int totalVotes;
    int seatsWon;
    double voteShare; // Percentage
    int candidatesCount;

    PartyStats() : totalVotes(0), seatsWon(0), voteShare(0.0), candidatesCount(0) {}
};

/**
 * @brief Structure to hold election statistics
 */
struct ElectionStats {
    std::string country;
    int year;
    int totalVotes;
    int totalSeats;
    int totalCandidates;
    int constituencies;
    double turnout; // Percentage (if available)
    std::vector<PartyStats> partyStats;

    ElectionStats() : year(0), totalVotes(0), totalSeats(0), 
                     totalCandidates(0), constituencies(0), turnout(0.0) {}
};

/**
 * @brief Structure for comparative analysis between two elections
 */
struct ComparativeAnalysis {
    std::string country;
    int year1;
    int year2;
    int voteChange; // Difference in total votes
    double voteChangePercent;
    std::map<std::string, int> partyVoteChanges; // Party name -> vote change
    std::map<std::string, int> partySeatChanges; // Party name -> seat change
    std::vector<std::string> newParties; // Parties that appeared in year2
    std::vector<std::string> disappearedParties; // Parties that disappeared
};

/**
 * @brief Class for analyzing election data
 * 
 * Provides various analytical functions for election data analysis
 */
class ElectionAnalyzer {
public:
    /**
     * @brief Calculate comprehensive statistics for an election
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return ElectionStats object
     * Time Complexity: O(n) where n is number of records for that election
     */
    static ElectionStats calculateElectionStats(const ElectionData& data,
                                               const std::string& country,
                                               int year);

    /**
     * @brief Calculate total votes for an election
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return Total votes
     * Time Complexity: O(n) where n is number of records for that election
     */
    static int calculateTotalVotes(const ElectionData& data,
                                   const std::string& country,
                                   int year);

    /**
     * @brief Calculate total seats won in an election
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return Total seats won
     * Time Complexity: O(n) where n is number of records for that election
     */
    static int calculateTotalSeats(const ElectionData& data,
                                  const std::string& country,
                                  int year);

    /**
     * @brief Calculate party-wise vote shares
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return Vector of PartyStats sorted by votes descending
     * Time Complexity: O(n log n) where n is number of parties
     */
    static std::vector<PartyStats> calculatePartyVoteShares(const ElectionData& data,
                                                           const std::string& country,
                                                           int year);

    /**
     * @brief Get seat distribution by party
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return Map of party name to seats won
     * Time Complexity: O(n) where n is number of records for that election
     */
    static std::map<std::string, int> getSeatDistribution(const ElectionData& data,
                                                          const std::string& country,
                                                          int year);

    /**
     * @brief Rank parties by votes
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return Vector of PartyStats sorted by total votes descending
     * Time Complexity: O(n log n) where n is number of parties
     */
    static std::vector<PartyStats> rankPartiesByVotes(const ElectionData& data,
                                                      const std::string& country,
                                                      int year);

    /**
     * @brief Rank parties by seats
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return Vector of PartyStats sorted by seats descending
     * Time Complexity: O(n log n) where n is number of parties
     */
    static std::vector<PartyStats> rankPartiesBySeats(const ElectionData& data,
                                                      const std::string& country,
                                                      int year);

    /**
     * @brief Perform comparative analysis between two elections
     * @param data ElectionData object
     * @param country Country name
     * @param year1 First election year
     * @param year2 Second election year
     * @return ComparativeAnalysis object
     * Time Complexity: O(n1 + n2) where n1, n2 are records for each election
     */
    static ComparativeAnalysis compareElections(const ElectionData& data,
                                                const std::string& country,
                                                int year1,
                                                int year2);

    /**
     * @brief Get trend analysis for a party across multiple elections
     * @param data ElectionData object
     * @param country Country name
     * @param party Party name
     * @param years Vector of years to analyze
     * @return Map of year to PartyStats
     * Time Complexity: O(n * m) where n is years, m is records per year
     */
    static std::map<int, PartyStats> getPartyTrend(const ElectionData& data,
                                                   const std::string& country,
                                                   const std::string& party,
                                                   const std::vector<int>& years);

    /**
     * @brief Get top N candidates by votes in an election
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @param n Number of top candidates
     * @return Vector of ElectionRecord sorted by votes descending
     * Time Complexity: O(n log n) where n is number of candidates
     */
    static std::vector<ElectionRecord> getTopCandidates(const ElectionData& data,
                                                       const std::string& country,
                                                       int year,
                                                       int n);

    /**
     * @brief Get winning candidates (elected) for an election
     * @param data ElectionData object
     * @param country Country name
     * @param year Election year
     * @return Vector of ElectionRecord for elected candidates
     * Time Complexity: O(n) where n is number of records for that election
     */
    static std::vector<ElectionRecord> getWinningCandidates(const ElectionData& data,
                                                            const std::string& country,
                                                            int year);
};

#endif // ELECTION_ANALYZER_H


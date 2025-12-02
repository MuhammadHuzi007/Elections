#ifndef ELECTION_ANALYZER_H
#define ELECTION_ANALYZER_H

#include "ElectionData.h"
#include <string>
#include <vector>

/**
 * @brief Simple structure to hold party statistics
 */
struct PartyStats {
    std::string party;
    int totalVotes;
    int seatsWon;
    double voteShare;
    int candidatesCount;

    PartyStats() {
        totalVotes = 0;
        seatsWon = 0;
        voteShare = 0.0;
        candidatesCount = 0;
    }
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
    double turnout;
    std::vector<PartyStats> partyStats;

    ElectionStats() {
        year = 0;
        totalVotes = 0;
        totalSeats = 0;
        totalCandidates = 0;
        constituencies = 0;
        turnout = 0.0;
    }
};

/**
 * @brief Structure for seat distribution
 */
struct SeatInfo {
    std::string party;
    int seats;
};

/**
 * @brief Structure for party changes between elections
 */
struct PartyChange {
    std::string party;
    int voteChange;
    int seatChange;
};

/**
 * @brief Structure for comparative analysis
 */
struct ComparativeAnalysis {
    std::string country;
    int year1;
    int year2;
    int voteChange;
    double voteChangePercent;
    std::vector<PartyChange> partyChanges;
    std::vector<std::string> newParties;
    std::vector<std::string> disappearedParties;
};

/**
 * @brief Structure for party trend
 */
struct PartyTrend {
    int year;
    PartyStats stats;
};

/**
 * @brief Simple class for analyzing election data
 */
class ElectionAnalyzer {
public:
    /**
     * @brief Calculate total votes for an election
     */
    static int calculateTotalVotes(ElectionData& data, const std::string& country, int year);

    /**
     * @brief Calculate total seats won
     */
    static int calculateTotalSeats(ElectionData& data, const std::string& country, int year);

    /**
     * @brief Calculate party-wise vote shares
     */
    static std::vector<PartyStats> calculatePartyVoteShares(ElectionData& data, const std::string& country, int year);

    /**
     * @brief Calculate comprehensive election statistics
     */
    static ElectionStats calculateElectionStats(ElectionData& data, const std::string& country, int year);

    /**
     * @brief Get seat distribution by party
     */
    static std::vector<SeatInfo> getSeatDistribution(ElectionData& data, const std::string& country, int year);

    /**
     * @brief Rank parties by votes
     */
    static std::vector<PartyStats> rankPartiesByVotes(ElectionData& data, const std::string& country, int year);

    /**
     * @brief Get top N candidates by votes
     */
    static std::vector<ElectionRecord> getTopCandidates(ElectionData& data, const std::string& country, int year, int n);

    /**
     * @brief Get winning candidates (elected)
     */
    static std::vector<ElectionRecord> getWinningCandidates(ElectionData& data, const std::string& country, int year);

    /**
     * @brief Compare two elections
     */
    static ComparativeAnalysis compareElections(ElectionData& data, const std::string& country, int year1, int year2);

    /**
     * @brief Get party trend across multiple years
     */
    static std::vector<PartyTrend> getPartyTrend(ElectionData& data, const std::string& country, const std::string& party, std::vector<int> years);
};

#endif // ELECTION_ANALYZER_H

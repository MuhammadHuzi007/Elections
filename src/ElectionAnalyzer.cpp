#include "../include/ElectionAnalyzer.h"
#include <algorithm>
#include <set>
#include <cmath>

ElectionStats ElectionAnalyzer::calculateElectionStats(const ElectionData& data,
                                                       const std::string& country,
                                                       int year) {
    ElectionStats stats;
    stats.country = country;
    stats.year = year;

    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);

    if (records.empty()) {
        return stats;
    }

    // Calculate totals
    stats.totalVotes = calculateTotalVotes(data, country, year);
    stats.totalSeats = calculateTotalSeats(data, country, year);
    stats.totalCandidates = static_cast<int>(records.size());

    // Count unique constituencies
    std::set<std::string> constituencies;
    for (const auto& record : records) {
        constituencies.insert(record.constituency);
    }
    stats.constituencies = static_cast<int>(constituencies.size());

    // Calculate party statistics
    stats.partyStats = calculatePartyVoteShares(data, country, year);

    return stats;
}

int ElectionAnalyzer::calculateTotalVotes(const ElectionData& data,
                                          const std::string& country,
                                          int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    int total = 0;

    for (const auto& record : records) {
        total += record.votes;
    }

    return total;
}

int ElectionAnalyzer::calculateTotalSeats(const ElectionData& data,
                                         const std::string& country,
                                         int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    int seats = 0;

    for (const auto& record : records) {
        if (record.elected) {
            seats++;
        }
    }

    return seats;
}

std::vector<PartyStats> ElectionAnalyzer::calculatePartyVoteShares(const ElectionData& data,
                                                                   const std::string& country,
                                                                   int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    std::map<std::string, PartyStats> partyMap;

    int totalVotes = calculateTotalVotes(data, country, year);

    // Aggregate party data
    for (const auto& record : records) {
        if (partyMap.find(record.party) == partyMap.end()) {
            partyMap[record.party] = PartyStats();
            partyMap[record.party].party = record.party;
        }

        partyMap[record.party].totalVotes += record.votes;
        partyMap[record.party].candidatesCount++;

        if (record.elected) {
            partyMap[record.party].seatsWon++;
        }
    }

    // Calculate vote shares
    for (auto& pair : partyMap) {
        if (totalVotes > 0) {
            pair.second.voteShare = (static_cast<double>(pair.second.totalVotes) / totalVotes) * 100.0;
        }
    }

    // Convert to vector and sort by votes
    std::vector<PartyStats> result;
    for (const auto& pair : partyMap) {
        result.push_back(pair.second);
    }

    std::sort(result.begin(), result.end(),
              [](const PartyStats& a, const PartyStats& b) {
                  return a.totalVotes > b.totalVotes;
              });

    return result;
}

std::map<std::string, int> ElectionAnalyzer::getSeatDistribution(const ElectionData& data,
                                                                 const std::string& country,
                                                                 int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    std::map<std::string, int> seatMap;

    for (const auto& record : records) {
        if (record.elected) {
            seatMap[record.party]++;
        }
    }

    return seatMap;
}

std::vector<PartyStats> ElectionAnalyzer::rankPartiesByVotes(const ElectionData& data,
                                                            const std::string& country,
                                                            int year) {
    return calculatePartyVoteShares(data, country, year);
}

std::vector<PartyStats> ElectionAnalyzer::rankPartiesBySeats(const ElectionData& data,
                                                             const std::string& country,
                                                             int year) {
    std::vector<PartyStats> stats = calculatePartyVoteShares(data, country, year);

    std::sort(stats.begin(), stats.end(),
              [](const PartyStats& a, const PartyStats& b) {
                  if (a.seatsWon != b.seatsWon) {
                      return a.seatsWon > b.seatsWon;
                  }
                  return a.totalVotes > b.totalVotes;
              });

    return stats;
}

ComparativeAnalysis ElectionAnalyzer::compareElections(const ElectionData& data,
                                                       const std::string& country,
                                                       int year1,
                                                       int year2) {
    ComparativeAnalysis analysis;
    analysis.country = country;
    analysis.year1 = year1;
    analysis.year2 = year2;

    // Get statistics for both elections
    ElectionStats stats1 = calculateElectionStats(data, country, year1);
    ElectionStats stats2 = calculateElectionStats(data, country, year2);

    // Calculate vote changes
    analysis.voteChange = stats2.totalVotes - stats1.totalVotes;
    if (stats1.totalVotes > 0) {
        analysis.voteChangePercent = ((static_cast<double>(analysis.voteChange) / stats1.totalVotes) * 100.0);
    }

    // Get party statistics for both years
    std::vector<PartyStats> parties1 = calculatePartyVoteShares(data, country, year1);
    std::vector<PartyStats> parties2 = calculatePartyVoteShares(data, country, year2);

    // Create maps for easier lookup
    std::map<std::string, PartyStats> partyMap1, partyMap2;
    for (const auto& ps : parties1) {
        partyMap1[ps.party] = ps;
    }
    for (const auto& ps : parties2) {
        partyMap2[ps.party] = ps;
    }

    // Calculate party vote and seat changes
    std::set<std::string> allParties;
    for (const auto& pair : partyMap1) {
        allParties.insert(pair.first);
    }
    for (const auto& pair : partyMap2) {
        allParties.insert(pair.first);
    }

    for (const std::string& party : allParties) {
        int votes1 = (partyMap1.find(party) != partyMap1.end()) ? partyMap1[party].totalVotes : 0;
        int votes2 = (partyMap2.find(party) != partyMap2.end()) ? partyMap2[party].totalVotes : 0;
        analysis.partyVoteChanges[party] = votes2 - votes1;

        int seats1 = (partyMap1.find(party) != partyMap1.end()) ? partyMap1[party].seatsWon : 0;
        int seats2 = (partyMap2.find(party) != partyMap2.end()) ? partyMap2[party].seatsWon : 0;
        analysis.partySeatChanges[party] = seats2 - seats1;

        // Track new and disappeared parties
        if (partyMap1.find(party) == partyMap1.end() && partyMap2.find(party) != partyMap2.end()) {
            analysis.newParties.push_back(party);
        }
        if (partyMap1.find(party) != partyMap1.end() && partyMap2.find(party) == partyMap2.end()) {
            analysis.disappearedParties.push_back(party);
        }
    }

    return analysis;
}

std::map<int, PartyStats> ElectionAnalyzer::getPartyTrend(const ElectionData& data,
                                                         const std::string& country,
                                                         const std::string& party,
                                                         const std::vector<int>& years) {
    std::map<int, PartyStats> trend;

    for (int year : years) {
        std::vector<PartyStats> stats = calculatePartyVoteShares(data, country, year);

        for (const auto& ps : stats) {
            if (ps.party == party) {
                trend[year] = ps;
                break;
            }
        }
    }

    return trend;
}

std::vector<ElectionRecord> ElectionAnalyzer::getTopCandidates(const ElectionData& data,
                                                               const std::string& country,
                                                               int year,
                                                               int n) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);

    // Sort by votes descending
    std::sort(records.begin(), records.end(),
              [](const ElectionRecord& a, const ElectionRecord& b) {
                  return a.votes > b.votes;
              });

    // Return top n
    if (static_cast<size_t>(n) > records.size()) {
        n = static_cast<int>(records.size());
    }

    return std::vector<ElectionRecord>(records.begin(), records.begin() + n);
}

std::vector<ElectionRecord> ElectionAnalyzer::getWinningCandidates(const ElectionData& data,
                                                                  const std::string& country,
                                                                  int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    std::vector<ElectionRecord> winners;

    for (const auto& record : records) {
        if (record.elected) {
            winners.push_back(record);
        }
    }

    // Sort by votes descending
    std::sort(winners.begin(), winners.end(),
              [](const ElectionRecord& a, const ElectionRecord& b) {
                  return a.votes > b.votes;
              });

    return winners;
}


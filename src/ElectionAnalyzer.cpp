#include "../include/ElectionAnalyzer.h"

// Calculate total votes for an election
int ElectionAnalyzer::calculateTotalVotes(ElectionData& data, const std::string& country, int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    int total = 0;

    for (int i = 0; i < records.size(); i++) {
        total += records[i].votes;
    }

    return total;
}

// Calculate total seats won
int ElectionAnalyzer::calculateTotalSeats(ElectionData& data, const std::string& country, int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    int seats = 0;

    for (int i = 0; i < records.size(); i++) {
        if (records[i].elected) {
            seats++;
        }
    }

    return seats;
}

// Calculate party-wise statistics
std::vector<PartyStats> ElectionAnalyzer::calculatePartyVoteShares(ElectionData& data, const std::string& country, int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    std::vector<PartyStats> partyList;

    int totalVotes = calculateTotalVotes(data, country, year);

    // For each record, find or create party stats
    for (int i = 0; i < records.size(); i++) {
        // Find if party already exists in list
        int partyIndex = -1;
        for (int j = 0; j < partyList.size(); j++) {
            if (partyList[j].party == records[i].party) {
                partyIndex = j;
                break;
            }
        }

        // If party not found, create new entry
        if (partyIndex == -1) {
            PartyStats newParty;
            newParty.party = records[i].party;
            newParty.totalVotes = 0;
            newParty.seatsWon = 0;
            newParty.candidatesCount = 0;
            newParty.voteShare = 0.0;
            partyList.push_back(newParty);
            partyIndex = partyList.size() - 1;
        }

        // Update party stats
        partyList[partyIndex].totalVotes += records[i].votes;
        partyList[partyIndex].candidatesCount++;
        if (records[i].elected) {
            partyList[partyIndex].seatsWon++;
        }
    }

    // Calculate vote shares
    for (int i = 0; i < partyList.size(); i++) {
        if (totalVotes > 0) {
            partyList[i].voteShare = (partyList[i].totalVotes * 100.0) / totalVotes;
        }
    }

    // Sort by total votes (bubble sort - simple!)
    for (int i = 0; i < partyList.size() - 1; i++) {
        for (int j = 0; j < partyList.size() - i - 1; j++) {
            if (partyList[j].totalVotes < partyList[j + 1].totalVotes) {
                // Swap
                PartyStats temp = partyList[j];
                partyList[j] = partyList[j + 1];
                partyList[j + 1] = temp;
            }
        }
    }

    return partyList;
}

// Calculate election statistics
ElectionStats ElectionAnalyzer::calculateElectionStats(ElectionData& data, const std::string& country, int year) {
    ElectionStats stats;
    stats.country = country;
    stats.year = year;

    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);

    stats.totalVotes = calculateTotalVotes(data, country, year);
    stats.totalSeats = calculateTotalSeats(data, country, year);
    stats.totalCandidates = records.size();

    // Count unique constituencies
    std::vector<std::string> constituencies;
    for (int i = 0; i < records.size(); i++) {
        bool found = false;
        for (int j = 0; j < constituencies.size(); j++) {
            if (constituencies[j] == records[i].constituency) {
                found = true;
                break;
            }
        }
        if (!found) {
            constituencies.push_back(records[i].constituency);
        }
    }
    stats.constituencies = constituencies.size();

    stats.partyStats = calculatePartyVoteShares(data, country, year);

    return stats;
}

// Get seat distribution
std::vector<SeatInfo> ElectionAnalyzer::getSeatDistribution(ElectionData& data, const std::string& country, int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    std::vector<SeatInfo> seatList;

    for (int i = 0; i < records.size(); i++) {
        if (records[i].elected) {
            // Find if party already exists
            int partyIndex = -1;
            for (int j = 0; j < seatList.size(); j++) {
                if (seatList[j].party == records[i].party) {
                    partyIndex = j;
                    break;
                }
            }

            if (partyIndex == -1) {
                SeatInfo newSeat;
                newSeat.party = records[i].party;
                newSeat.seats = 1;
                seatList.push_back(newSeat);
            } else {
                seatList[partyIndex].seats++;
            }
        }
    }

    return seatList;
}

// Rank parties by votes
std::vector<PartyStats> ElectionAnalyzer::rankPartiesByVotes(ElectionData& data, const std::string& country, int year) {
    return calculatePartyVoteShares(data, country, year);
}

// Get top N candidates
std::vector<ElectionRecord> ElectionAnalyzer::getTopCandidates(ElectionData& data, const std::string& country, int year, int n) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);

    // Sort by votes using bubble sort
    for (int i = 0; i < records.size() - 1; i++) {
        for (int j = 0; j < records.size() - i - 1; j++) {
            if (records[j].votes < records[j + 1].votes) {
                // Swap
                ElectionRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    // Return top n
    std::vector<ElectionRecord> topN;
    for (int i = 0; i < n && i < records.size(); i++) {
        topN.push_back(records[i]);
    }

    return topN;
}

// Get winning candidates
std::vector<ElectionRecord> ElectionAnalyzer::getWinningCandidates(ElectionData& data, const std::string& country, int year) {
    std::vector<ElectionRecord> records = data.getElectionRecords(country, year);
    std::vector<ElectionRecord> winners;

    for (int i = 0; i < records.size(); i++) {
        if (records[i].elected) {
            winners.push_back(records[i]);
        }
    }

    // Sort by votes
    for (int i = 0; i < winners.size() - 1; i++) {
        for (int j = 0; j < winners.size() - i - 1; j++) {
            if (winners[j].votes < winners[j + 1].votes) {
                ElectionRecord temp = winners[j];
                winners[j] = winners[j + 1];
                winners[j + 1] = temp;
            }
        }
    }

    return winners;
}

// Compare two elections
ComparativeAnalysis ElectionAnalyzer::compareElections(ElectionData& data, const std::string& country, int year1, int year2) {
    ComparativeAnalysis analysis;
    analysis.country = country;
    analysis.year1 = year1;
    analysis.year2 = year2;

    ElectionStats stats1 = calculateElectionStats(data, country, year1);
    ElectionStats stats2 = calculateElectionStats(data, country, year2);

    analysis.voteChange = stats2.totalVotes - stats1.totalVotes;
    if (stats1.totalVotes > 0) {
        analysis.voteChangePercent = (analysis.voteChange * 100.0) / stats1.totalVotes;
    }

    // Compare parties
    for (int i = 0; i < stats1.partyStats.size(); i++) {
        std::string party = stats1.partyStats[i].party;
        int votes1 = stats1.partyStats[i].totalVotes;
        int seats1 = stats1.partyStats[i].seatsWon;

        // Find same party in year2
        int votes2 = 0;
        int seats2 = 0;
        bool foundInYear2 = false;

        for (int j = 0; j < stats2.partyStats.size(); j++) {
            if (stats2.partyStats[j].party == party) {
                votes2 = stats2.partyStats[j].totalVotes;
                seats2 = stats2.partyStats[j].seatsWon;
                foundInYear2 = true;
                break;
            }
        }

        PartyChange change;
        change.party = party;
        change.voteChange = votes2 - votes1;
        change.seatChange = seats2 - seats1;
        analysis.partyChanges.push_back(change);

        if (!foundInYear2) {
            analysis.disappearedParties.push_back(party);
        }
    }

    // Find new parties in year2
    for (int i = 0; i < stats2.partyStats.size(); i++) {
        std::string party = stats2.partyStats[i].party;
        bool foundInYear1 = false;

        for (int j = 0; j < stats1.partyStats.size(); j++) {
            if (stats1.partyStats[j].party == party) {
                foundInYear1 = true;
                break;
            }
        }

        if (!foundInYear1) {
            analysis.newParties.push_back(party);
        }
    }

    return analysis;
}

// Get party trend across years
std::vector<PartyTrend> ElectionAnalyzer::getPartyTrend(ElectionData& data, const std::string& country, const std::string& party, std::vector<int> years) {
    std::vector<PartyTrend> trend;

    for (int i = 0; i < years.size(); i++) {
        std::vector<PartyStats> allParties = calculatePartyVoteShares(data, country, years[i]);

        for (int j = 0; j < allParties.size(); j++) {
            if (allParties[j].party == party) {
                PartyTrend pt;
                pt.year = years[i];
                pt.stats = allParties[j];
                trend.push_back(pt);
                break;
            }
        }
    }

    return trend;
}


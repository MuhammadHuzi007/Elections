#include "../include/ElectionData.h"
#include "../include/CSVReader.h"
#include "../include/ElectionAnalyzer.h"
#include <cassert>
#include <iostream>

/**
 * @brief Test basic insertion and retrieval
 */
void testInsertAndRetrieve() {
    std::cout << "Testing insert and retrieve operations..." << std::endl;
    
    ElectionData data;
    ElectionRecord record("TestCountry", 2020, "TestConstituency", 
                         "TestCandidate", "TestParty", 1000, true);
    
    // Test insert
    assert(data.insertRecord(record) == true);
    assert(data.getTotalRecords() == 1);
    
    // Test retrieve
    const ElectionRecord* retrieved = data.getRecord("TestCountry", 2020, 
                                                     "TestConstituency", "TestCandidate");
    assert(retrieved != nullptr);
    assert(retrieved->votes == 1000);
    assert(retrieved->elected == true);
    
    // Test duplicate insert
    assert(data.insertRecord(record) == false);
    
    std::cout << "  ✓ Passed" << std::endl;
}

/**
 * @brief Test update operation
 */
void testUpdate() {
    std::cout << "Testing update operation..." << std::endl;
    
    ElectionData data;
    ElectionRecord record("TestCountry", 2020, "TestConstituency", 
                         "TestCandidate", "TestParty", 1000, false);
    
    data.insertRecord(record);
    
    // Update record
    record.votes = 2000;
    record.elected = true;
    assert(data.updateRecord(record) == true);
    
    const ElectionRecord* updated = data.getRecord("TestCountry", 2020, 
                                                   "TestConstituency", "TestCandidate");
    assert(updated != nullptr);
    assert(updated->votes == 2000);
    assert(updated->elected == true);
    
    // Test update non-existent record
    ElectionRecord fake("Fake", 2020, "Fake", "Fake", "Fake", 0, false);
    assert(data.updateRecord(fake) == false);
    
    std::cout << "  ✓ Passed" << std::endl;
}

/**
 * @brief Test election records retrieval
 */
void testGetElectionRecords() {
    std::cout << "Testing get election records..." << std::endl;
    
    ElectionData data;
    
    // Insert multiple records for same election
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const1", "Cand1", "Party1", 1000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const2", "Cand2", "Party2", 2000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2021, "Const1", "Cand3", "Party1", 1500, false));
    
    std::vector<ElectionRecord> records2020 = data.getElectionRecords("TestCountry", 2020);
    assert(records2020.size() == 2);
    
    std::vector<ElectionRecord> records2021 = data.getElectionRecords("TestCountry", 2021);
    assert(records2021.size() == 1);
    
    std::cout << "  ✓ Passed" << std::endl;
}

/**
 * @brief Test party records retrieval
 */
void testGetPartyRecords() {
    std::cout << "Testing get party records..." << std::endl;
    
    ElectionData data;
    
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const1", "Cand1", "Party1", 1000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const2", "Cand2", "Party1", 2000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const3", "Cand3", "Party2", 1500, false));
    
    std::vector<ElectionRecord> party1Records = data.getPartyRecords("TestCountry", 2020, "Party1");
    assert(party1Records.size() == 2);
    
    std::vector<ElectionRecord> party2Records = data.getPartyRecords("TestCountry", 2020, "Party2");
    assert(party2Records.size() == 1);
    
    std::cout << "  ✓ Passed" << std::endl;
}

/**
 * @brief Test statistics calculation
 */
void testStatistics() {
    std::cout << "Testing statistics calculation..." << std::endl;
    
    ElectionData data;
    
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const1", "Cand1", "Party1", 1000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const2", "Cand2", "Party1", 2000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const3", "Cand3", "Party2", 1500, false));
    
    int totalVotes = ElectionAnalyzer::calculateTotalVotes(data, "TestCountry", 2020);
    assert(totalVotes == 4500);
    
    int totalSeats = ElectionAnalyzer::calculateTotalSeats(data, "TestCountry", 2020);
    assert(totalSeats == 2);
    
    ElectionStats stats = ElectionAnalyzer::calculateElectionStats(data, "TestCountry", 2020);
    assert(stats.totalVotes == 4500);
    assert(stats.totalSeats == 2);
    assert(stats.totalCandidates == 3);
    assert(stats.constituencies == 3);
    
    std::cout << "  ✓ Passed" << std::endl;
}

/**
 * @brief Test party vote shares
 */
void testPartyVoteShares() {
    std::cout << "Testing party vote shares..." << std::endl;
    
    ElectionData data;
    
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const1", "Cand1", "Party1", 1000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const2", "Cand2", "Party1", 2000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const3", "Cand3", "Party2", 1500, false));
    
    std::vector<PartyStats> partyStats = ElectionAnalyzer::calculatePartyVoteShares(data, "TestCountry", 2020);
    
    assert(partyStats.size() == 2);
    assert(partyStats[0].party == "Party1" || partyStats[0].party == "Party2");
    assert(partyStats[0].totalVotes >= partyStats[1].totalVotes); // Should be sorted
    
    // Find Party1
    bool foundParty1 = false;
    for (const auto& ps : partyStats) {
        if (ps.party == "Party1") {
            assert(ps.totalVotes == 3000);
            assert(ps.seatsWon == 2);
            foundParty1 = true;
        }
    }
    assert(foundParty1);
    
    std::cout << "  ✓ Passed" << std::endl;
}

/**
 * @brief Test comparative analysis
 */
void testComparativeAnalysis() {
    std::cout << "Testing comparative analysis..." << std::endl;
    
    ElectionData data;
    
    // 2020 election
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const1", "Cand1", "Party1", 1000, true));
    data.insertRecord(ElectionRecord("TestCountry", 2020, "Const2", "Cand2", "Party2", 2000, true));
    
    // 2021 election
    data.insertRecord(ElectionRecord("TestCountry", 2021, "Const1", "Cand3", "Party1", 1500, true));
    data.insertRecord(ElectionRecord("TestCountry", 2021, "Const2", "Cand4", "Party2", 2500, true));
    data.insertRecord(ElectionRecord("TestCountry", 2021, "Const3", "Cand5", "Party3", 1000, false));
    
    ComparativeAnalysis analysis = ElectionAnalyzer::compareElections(data, "TestCountry", 2020, 2021);
    
    assert(analysis.voteChange == 2000); // 5000 - 3000
    assert(analysis.partyVoteChanges["Party1"] == 500); // 1500 - 1000
    assert(analysis.partyVoteChanges["Party2"] == 500); // 2500 - 2000
    assert(analysis.newParties.size() == 1); // Party3 is new
    
    std::cout << "  ✓ Passed" << std::endl;
}

/**
 * @brief Test CSV reading
 */
void testCSVReading() {
    std::cout << "Testing CSV reading..." << std::endl;
    
    ElectionData data;
    
    // Try to read a CSV file (if it exists)
    bool success = CSVReader::readFromFile("jordan_2016.csv", data);
    
    if (success) {
        assert(data.getTotalRecords() > 0);
        
        // Verify we can retrieve records
        std::vector<ElectionRecord> records = data.getElectionRecords("Jordan", 2016);
        assert(!records.empty());
        
        std::cout << "  ✓ Passed (loaded " << data.getTotalRecords() << " records)" << std::endl;
    } else {
        std::cout << "  ⚠ Skipped (CSV file not found)" << std::endl;
    }
}

/**
 * @brief Run all tests
 */
int main() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                          UNIT TESTS                                           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";

    try {
        testInsertAndRetrieve();
        testUpdate();
        testGetElectionRecords();
        testGetPartyRecords();
        testStatistics();
        testPartyVoteShares();
        testComparativeAnalysis();
        testCSVReading();
        
        std::cout << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    ALL TESTS PASSED! ✓                                      ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}


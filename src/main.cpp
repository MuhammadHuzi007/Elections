#include "../include/ElectionData.h"
#include "../include/CSVReader.h"
#include "../include/ElectionAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <limits>
#include <algorithm>

/**
 * @brief Display a separator line
 */
void printSeparator() {
    std::cout << std::string(80, '=') << std::endl;
}

/**
 * @brief Clear screen (cross-platform)
 */
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * @brief Get user input as integer
 */
int getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input! Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(); // Clear newline
    return value;
}

/**
 * @brief Get user input as string
 */
std::string getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

/**
 * @brief Get available countries and years from data
 */
std::map<std::string, std::vector<int>> getAvailableElections(ElectionData& data) {
    std::map<std::string, std::vector<int>> result;
    std::vector<ElectionRecord> records = data.getAllRecords();
    
    for (int i = 0; i < records.size(); i++) {
        std::string country = records[i].country;
        int year = records[i].year;
        
        // Check if country exists in map
        if (result.find(country) == result.end()) {
            result[country] = std::vector<int>();
        }
        
        // Check if year already added
        bool found = false;
        for (int j = 0; j < result[country].size(); j++) {
            if (result[country][j] == year) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            result[country].push_back(year);
        }
    }
    
    // Sort years for each country (bubble sort)
    for (std::map<std::string, std::vector<int>>::iterator it = result.begin(); it != result.end(); ++it) {
        std::vector<int>& years = it->second;
        for (int i = 0; i < years.size() - 1; i++) {
            for (int j = 0; j < years.size() - i - 1; j++) {
                if (years[j] > years[j + 1]) {
                    int temp = years[j];
                    years[j] = years[j + 1];
                    years[j + 1] = temp;
                }
            }
        }
    }
    
    return result;
}

/**
 * @brief Display election statistics
 */
void displayElectionStats(const ElectionStats& stats) {
    printSeparator();
    std::cout << "ELECTION STATISTICS" << std::endl;
    printSeparator();
    std::cout << "Country: " << stats.country << std::endl;
    std::cout << "Year: " << stats.year << std::endl;
    std::cout << "Total Votes: " << stats.totalVotes << std::endl;
    std::cout << "Total Seats: " << stats.totalSeats << std::endl;
    std::cout << "Total Candidates: " << stats.totalCandidates << std::endl;
    std::cout << "Constituencies: " << stats.constituencies << std::endl;
    if (stats.turnout > 0) {
        std::cout << "Turnout: " << std::fixed << std::setprecision(2) 
                  << stats.turnout << "%" << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Display party statistics
 */
void displayPartyStats(const std::vector<PartyStats>& partyStats) {
    printSeparator();
    std::cout << "PARTY-WISE STATISTICS" << std::endl;
    printSeparator();
    std::cout << std::left << std::setw(35) << "Party" 
              << std::right << std::setw(12) << "Total Votes"
              << std::setw(10) << "Vote Share"
              << std::setw(10) << "Seats Won"
              << std::setw(12) << "Candidates" << std::endl;
    std::cout << std::string(79, '-') << std::endl;

    for (const auto& ps : partyStats) {
        std::cout << std::left << std::setw(35) << ps.party
                  << std::right << std::setw(12) << ps.totalVotes
                  << std::setw(9) << std::fixed << std::setprecision(2) 
                  << ps.voteShare << "%"
                  << std::setw(10) << ps.seatsWon
                  << std::setw(12) << ps.candidatesCount << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Display comparative analysis
 */
void displayComparativeAnalysis(const ComparativeAnalysis& analysis) {
    printSeparator();
    std::cout << "COMPARATIVE ANALYSIS: " << analysis.country << std::endl;
    std::cout << "Comparing " << analysis.year1 << " vs " << analysis.year2 << std::endl;
    printSeparator();
    
    std::cout << "Overall Vote Change: " << analysis.voteChange 
              << " (" << std::fixed << std::setprecision(2) 
              << analysis.voteChangePercent << "%)" << std::endl;
    std::cout << std::endl;

    std::cout << "Party Vote Changes:" << std::endl;
    std::cout << std::left << std::setw(35) << "Party" 
              << std::right << std::setw(15) << "Vote Change"
              << std::setw(15) << "Seat Change" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (int i = 0; i < analysis.partyChanges.size(); i++) {
        std::cout << std::left << std::setw(35) << analysis.partyChanges[i].party
                  << std::right << std::setw(15) << analysis.partyChanges[i].voteChange
                  << std::setw(15) << analysis.partyChanges[i].seatChange << std::endl;
    }
    std::cout << std::endl;

    if (analysis.newParties.size() > 0) {
        std::cout << "New Parties in " << analysis.year2 << ": ";
        for (int i = 0; i < analysis.newParties.size(); i++) {
            std::cout << analysis.newParties[i];
            if (i < analysis.newParties.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    if (analysis.disappearedParties.size() > 0) {
        std::cout << "Disappeared Parties: ";
        for (int i = 0; i < analysis.disappearedParties.size(); i++) {
            std::cout << analysis.disappearedParties[i];
            if (i < analysis.disappearedParties.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Display top candidates
 */
void displayTopCandidates(const std::vector<ElectionRecord>& candidates, int n) {
    printSeparator();
    std::cout << "TOP " << n << " CANDIDATES BY VOTES" << std::endl;
    printSeparator();
    std::cout << std::left << std::setw(25) << "Candidate"
              << std::setw(30) << "Party"
              << std::setw(20) << "Constituency"
              << std::right << std::setw(10) << "Votes"
              << std::setw(10) << "Elected" << std::endl;
    std::cout << std::string(105, '-') << std::endl;

    for (const auto& record : candidates) {
        std::cout << std::left << std::setw(25) << record.candidate
                  << std::setw(30) << record.party
                  << std::setw(20) << record.constituency
                  << std::right << std::setw(10) << record.votes
                  << std::setw(10) << (record.elected ? "Yes" : "No") << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Display seat distribution
 */
void displaySeatDistribution(const std::vector<SeatInfo>& seatDist) {
    printSeparator();
    std::cout << "SEAT DISTRIBUTION" << std::endl;
    printSeparator();
    std::cout << std::left << std::setw(40) << "Party" 
              << std::right << std::setw(10) << "Seats" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    for (int i = 0; i < seatDist.size(); i++) {
        std::cout << std::left << std::setw(40) << seatDist[i].party
                  << std::right << std::setw(10) << seatDist[i].seats << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Display winning candidates
 */
void displayWinningCandidates(const std::vector<ElectionRecord>& winners) {
    printSeparator();
    std::cout << "WINNING CANDIDATES (ELECTED)" << std::endl;
    printSeparator();
    std::cout << std::left << std::setw(25) << "Candidate"
              << std::setw(30) << "Party"
              << std::setw(20) << "Constituency"
              << std::right << std::setw(10) << "Votes" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    for (const auto& record : winners) {
        std::cout << std::left << std::setw(25) << record.candidate
                  << std::setw(30) << record.party
                  << std::setw(20) << record.constituency
                  << std::right << std::setw(10) << record.votes << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Select country from available options
 */
std::string selectCountry(const std::map<std::string, std::vector<int>>& availableElections) {
    std::vector<std::string> countries;
    for (const auto& pair : availableElections) {
        countries.push_back(pair.first);
    }
    
    std::cout << "\nAvailable Countries:\n";
    for (size_t i = 0; i < countries.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << countries[i] << std::endl;
    }
    
    int choice = getIntInput("\nSelect country (enter number): ");
    while (choice < 1 || choice > static_cast<int>(countries.size())) {
        std::cout << "Invalid choice! Please select a number between 1 and " 
                  << countries.size() << ": ";
        choice = getIntInput("");
    }
    
    return countries[choice - 1];
}

/**
 * @brief Select year from available options for a country
 */
int selectYear(const std::vector<int>& years) {
    std::cout << "\nAvailable Years:\n";
    for (size_t i = 0; i < years.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << years[i] << std::endl;
    }
    
    int choice = getIntInput("\nSelect year (enter number): ");
    while (choice < 1 || choice > static_cast<int>(years.size())) {
        std::cout << "Invalid choice! Please select a number between 1 and " 
                  << years.size() << ": ";
        choice = getIntInput("");
    }
    
    return years[choice - 1];
}

/**
 * @brief Show analysis menu for a specific election
 */
void showElectionAnalysisMenu(ElectionData& data, const std::string& country, int year) {
    while (true) {
        clearScreen();
        printSeparator();
        std::cout << "ELECTION ANALYSIS: " << country << " " << year << std::endl;
        printSeparator();
        std::cout << "\nSelect Analysis Type:\n";
        std::cout << "  1. View Election Statistics\n";
        std::cout << "  2. View Party-wise Statistics\n";
        std::cout << "  3. View Seat Distribution\n";
        std::cout << "  4. View Top Candidates\n";
        std::cout << "  5. View Winning Candidates\n";
        std::cout << "  6. View All Analysis\n";
        std::cout << "  0. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter your choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                ElectionStats stats = ElectionAnalyzer::calculateElectionStats(data, country, year);
                displayElectionStats(stats);
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 2: {
                clearScreen();
                std::vector<PartyStats> parties = ElectionAnalyzer::rankPartiesByVotes(data, country, year);
                displayPartyStats(parties);
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 3: {
                clearScreen();
                std::map<std::string, int> seats = ElectionAnalyzer::getSeatDistribution(data, country, year);
                displaySeatDistribution(seats);
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 4: {
                clearScreen();
                int n = getIntInput("Enter number of top candidates to display: ");
                std::vector<ElectionRecord> top = ElectionAnalyzer::getTopCandidates(data, country, year, n);
                displayTopCandidates(top, n);
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 5: {
                clearScreen();
                std::vector<ElectionRecord> winners = ElectionAnalyzer::getWinningCandidates(data, country, year);
                displayWinningCandidates(winners);
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 6: {
                clearScreen();
                ElectionStats stats = ElectionAnalyzer::calculateElectionStats(data, country, year);
                displayElectionStats(stats);
                
                std::vector<PartyStats> parties = ElectionAnalyzer::rankPartiesByVotes(data, country, year);
                displayPartyStats(parties);
                
                std::map<std::string, int> seats = ElectionAnalyzer::getSeatDistribution(data, country, year);
                displaySeatDistribution(seats);
                
                std::vector<ElectionRecord> top = ElectionAnalyzer::getTopCandidates(data, country, year, 10);
                displayTopCandidates(top, 10);
                
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 0:
                return;
            default:
                std::cout << "Invalid choice! Press Enter to continue...";
                std::cin.get();
                break;
        }
    }
}

/**
 * @brief Show comparative analysis menu
 */
void showComparativeAnalysisMenu(const ElectionData& data, 
                                 const std::map<std::string, std::vector<int>>& availableElections) {
    clearScreen();
    printSeparator();
    std::cout << "COMPARATIVE ANALYSIS" << std::endl;
    printSeparator();
    
    std::string country = selectCountry(availableElections);
    const auto& years = availableElections.at(country);
    
    if (years.size() < 2) {
        std::cout << "\nError: Need at least 2 years for comparison. Available years: ";
        for (int y : years) std::cout << y << " ";
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
        return;
    }
    
    std::cout << "\nSelect First Year:\n";
    int year1 = selectYear(years);
    
    std::cout << "\nSelect Second Year:\n";
    int year2 = selectYear(years);
    
    if (year1 == year2) {
        std::cout << "\nError: Cannot compare the same year!\nPress Enter to continue...";
        std::cin.get();
        return;
    }
    
    clearScreen();
    ComparativeAnalysis analysis = ElectionAnalyzer::compareElections(data, country, year1, year2);
    displayComparativeAnalysis(analysis);
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

/**
 * @brief Show party trend analysis menu
 */
void showPartyTrendMenu(const ElectionData& data,
                       const std::map<std::string, std::vector<int>>& availableElections) {
    clearScreen();
    printSeparator();
    std::cout << "PARTY TREND ANALYSIS" << std::endl;
    printSeparator();
    
    std::string country = selectCountry(availableElections);
    const auto& years = availableElections.at(country);
    
    // Get all parties for this country
    std::set<std::string> parties;
    for (int year : years) {
        std::vector<PartyStats> partyStats = ElectionAnalyzer::calculatePartyVoteShares(data, country, year);
        for (const auto& ps : partyStats) {
            parties.insert(ps.party);
        }
    }
    
    std::vector<std::string> partyList(parties.begin(), parties.end());
    std::cout << "\nAvailable Parties:\n";
    for (size_t i = 0; i < partyList.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << partyList[i] << std::endl;
    }
    
    int choice = getIntInput("\nSelect party (enter number): ");
    while (choice < 1 || choice > static_cast<int>(partyList.size())) {
        std::cout << "Invalid choice! Please select a number between 1 and " 
                  << partyList.size() << ": ";
        choice = getIntInput("");
    }
    
    std::string party = partyList[choice - 1];
    
    clearScreen();
    printSeparator();
    std::cout << "PARTY TREND ANALYSIS: " << party << " (" << country << ")" << std::endl;
    printSeparator();
    
    std::map<int, PartyStats> trend = ElectionAnalyzer::getPartyTrend(data, country, party, years);
    
    std::cout << std::left << std::setw(10) << "Year"
              << std::right << std::setw(15) << "Total Votes"
              << std::setw(15) << "Vote Share"
              << std::setw(15) << "Seats Won" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    for (int year : years) {
        if (trend.find(year) != trend.end()) {
            const PartyStats& ps = trend[year];
            std::cout << std::left << std::setw(10) << year
                      << std::right << std::setw(15) << ps.totalVotes
                      << std::setw(14) << std::fixed << std::setprecision(2) 
                      << ps.voteShare << "%"
                      << std::setw(15) << ps.seatsWon << std::endl;
        }
    }
    std::cout << std::endl;
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

/**
 * @brief Main menu
 */
void showMainMenu(const ElectionData& data, 
                  const std::map<std::string, std::vector<int>>& availableElections) {
    while (true) {
        clearScreen();
        std::cout << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                  ELECTION DATA ANALYSIS SYSTEM                               ║\n";
        std::cout << "║                  Using Custom Data Structures in C++                        ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";
        
        std::cout << "Available Data:\n";
        for (const auto& pair : availableElections) {
            std::cout << "  " << pair.first << ": ";
            for (size_t i = 0; i < pair.second.size(); ++i) {
                std::cout << pair.second[i];
                if (i < pair.second.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\nMain Menu:\n";
        std::cout << "  1. Analyze Specific Election\n";
        std::cout << "  2. Compare Two Elections\n";
        std::cout << "  3. View Party Trend Analysis\n";
        std::cout << "  4. View Complexity Analysis\n";
        std::cout << "  0. Exit\n";
        
        int choice = getIntInput("\nEnter your choice: ");
        
        switch (choice) {
            case 1: {
                std::string country = selectCountry(availableElections);
                int year = selectYear(availableElections.at(country));
                showElectionAnalysisMenu(data, country, year);
                break;
            }
            case 2:
                showComparativeAnalysisMenu(data, availableElections);
                break;
            case 3:
                showPartyTrendMenu(data, availableElections);
                break;
            case 4: {
                clearScreen();
                printSeparator();
                std::cout << "DATA STRUCTURE COMPLEXITY ANALYSIS" << std::endl;
                printSeparator();
                std::cout << "Operation                    | Time Complexity | Space Complexity" << std::endl;
                std::cout << std::string(80, '-') << std::endl;
                std::cout << "Insert Record                | O(1) avg        | O(1)" << std::endl;
                std::cout << "Update Record                | O(1) avg        | O(1)" << std::endl;
                std::cout << "Retrieve Record              | O(1) avg        | O(1)" << std::endl;
                std::cout << "Get Election Records         | O(k)            | O(k)" << std::endl;
                std::cout << "Get Party Records            | O(k)            | O(k)" << std::endl;
                std::cout << "Calculate Statistics         | O(n)            | O(p)" << std::endl;
                std::cout << "Comparative Analysis         | O(n1 + n2)      | O(p1 + p2)" << std::endl;
                std::cout << "Rank Parties                 | O(n log n)      | O(p)" << std::endl;
                std::cout << "\nWhere:" << std::endl;
                std::cout << "  n = total records in election" << std::endl;
                std::cout << "  k = records matching query" << std::endl;
                std::cout << "  p = number of unique parties" << std::endl;
                std::cout << "  n1, n2 = records in each election being compared" << std::endl;
                std::cout << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 0:
                std::cout << "\nThank you for using Election Data Analysis System!\n";
                return;
            default:
                std::cout << "Invalid choice! Press Enter to continue...";
                std::cin.get();
                break;
        }
    }
}

/**
 * @brief Main function
 */
int main() {
    ElectionData data;

    // Load data files
    std::vector<std::string> files = {
        "jordan_2016.csv",
        "jordan_2020.csv",
        "vanuatu_2016.csv",
        "vanuatu_2020.csv",
        "vanuatu_2022.csv"
    };

    std::cout << "Loading election data files..." << std::endl;
    int loadedCount = 0;
    for (const auto& file : files) {
        if (CSVReader::readFromFile(file, data)) {
            std::cout << "  ✓ Loaded: " << file << std::endl;
            loadedCount++;
        } else {
            std::cout << "  ✗ Failed to load: " << file << std::endl;
        }
    }
    std::cout << "Loaded " << loadedCount << " file(s) with " 
              << data.getTotalRecords() << " total records.\n" << std::endl;
    
    std::cout << "Press Enter to continue...";
    std::cin.get();

    // Get available elections
    std::map<std::string, std::vector<int>> availableElections = getAvailableElections(data);
    
    // Show main menu
    showMainMenu(data, availableElections);
    
    return 0;
}

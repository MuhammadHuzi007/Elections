#include "../include/CSVReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

bool CSVReader::readFromFile(const std::string& filename, ElectionData& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        // Skip header line
        if (firstLine) {
            firstLine = false;
            continue;
        }

        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        try {
            ElectionRecord record = parseLine(line);
            data.insertRecord(record);
        } catch (const std::exception& e) {
            // Skip invalid lines
            continue;
        }
    }

    file.close();
    return true;
}

ElectionRecord CSVReader::parseLine(const std::string& line) {
    std::vector<std::string> tokens = split(line, ',');

    if (tokens.size() < 7) {
        throw std::runtime_error("Invalid CSV line: insufficient columns");
    }

    ElectionRecord record;

    // Parse each field
    record.country = trim(tokens[0]);
    record.year = std::stoi(trim(tokens[1]));
    record.constituency = trim(tokens[2]);
    record.candidate = trim(tokens[3]);
    record.party = trim(tokens[4]);
    record.votes = std::stoi(trim(tokens[5]));

    // Parse "Elected" field (Yes/No)
    std::string electedStr = trim(tokens[6]);
    std::transform(electedStr.begin(), electedStr.end(), electedStr.begin(), ::tolower);
    record.elected = (electedStr == "yes" || electedStr == "true" || electedStr == "1");

    return record;
}

std::vector<std::string> CSVReader::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::string CSVReader::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}


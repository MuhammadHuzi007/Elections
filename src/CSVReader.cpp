#include "../include/CSVReader.h"
#include <fstream>
#include <sstream>

// Read CSV file and load data
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
        if (line.empty()) {
            continue;
        }

        // Parse the line and add to data
        ElectionRecord record = parseLine(line);
        data.addRecord(record);
    }

    file.close();
    return true;
}

// Parse one CSV line into an ElectionRecord
ElectionRecord CSVReader::parseLine(const std::string& line) {
    std::vector<std::string> tokens = split(line, ',');

    ElectionRecord record;

    // Parse each field
    record.country = trim(tokens[0]);
    record.year = stringToInt(trim(tokens[1]));
    record.constituency = trim(tokens[2]);
    record.candidate = trim(tokens[3]);
    record.party = trim(tokens[4]);
    record.votes = stringToInt(trim(tokens[5]));

    // Parse "Elected" field (Yes/No)
    std::string electedStr = trim(tokens[6]);
    record.elected = (electedStr == "Yes" || electedStr == "yes" || electedStr == "YES");

    return record;
}

// Split string by comma
std::vector<std::string> CSVReader::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token = "";

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delimiter) {
            tokens.push_back(token);
            token = "";
        } else {
            token += str[i];
        }
    }
    tokens.push_back(token); // Add last token

    return tokens;
}

// Remove spaces from start and end of string
std::string CSVReader::trim(const std::string& str) {
    int start = 0;
    int end = str.length() - 1;

    // Find first non-space character
    while (start < str.length() && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n')) {
        start++;
    }

    // Find last non-space character
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == '\r' || str[end] == '\n')) {
        end--;
    }

    if (start > end) {
        return "";
    }

    return str.substr(start, end - start + 1);
}

// Convert string to integer
int CSVReader::stringToInt(const std::string& str) {
    int result = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result;
}


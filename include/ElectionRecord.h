#ifndef ELECTION_RECORD_H
#define ELECTION_RECORD_H

#include <string>

/**
 * @brief Simple structure to store one election record
 */
struct ElectionRecord {
    std::string country;
    int year;
    std::string constituency;
    std::string candidate;
    std::string party;
    int votes;
    bool elected;

    // Default constructor - sets default values
    ElectionRecord() {
        year = 0;
        votes = 0;
        elected = false;
    }

    // Constructor with all values
    ElectionRecord(std::string c, int y, std::string con, std::string can, 
                   std::string p, int v, bool e) {
        country = c;
        year = y;
        constituency = con;
        candidate = can;
        party = p;
        votes = v;
        elected = e;
    }
};

#endif // ELECTION_RECORD_H


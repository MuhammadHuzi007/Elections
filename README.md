# Election Data Analysis System

A comprehensive C++ application for analyzing election data using custom data structures. This system efficiently stores, queries, and analyzes election results from multiple countries and years.

## Features

- **Custom Data Structure**: Efficient hash-map based structure for O(1) average case operations
- **CSV Data Loading**: Reads election data from CSV files
- **Comprehensive Analysis**:
  - Total votes, seats, and turnout calculations
  - Party-wise vote shares and seat distributions
  - Comparative analysis between elections
  - Party trend analysis across multiple years
  - Top candidates ranking
- **Unit Tests**: Comprehensive test suite for all operations

## Data Structure Design

The system uses a hybrid data structure approach:

- **Primary Storage**: `std::vector<ElectionRecord>` for maintaining insertion order
- **Index Maps**: Multiple `std::unordered_map` for O(1) lookups:
  - Record index map (by country, year, constituency, candidate)
  - Election index map (by country, year)
  - Party index map (by country, year, party)
  - Constituency index map (by country, year, constituency)

### Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Insert Record | O(1) avg | O(1) |
| Update Record | O(1) avg | O(1) |
| Retrieve Record | O(1) avg | O(1) |
| Get Election Records | O(k) | O(k) |
| Get Party Records | O(k) | O(k) |
| Calculate Statistics | O(n) | O(p) |
| Comparative Analysis | O(n1 + n2) | O(p1 + p2) |
| Rank Parties | O(n log n) | O(p) |

Where:
- n = total records in election
- k = records matching query
- p = number of unique parties
- n1, n2 = records in each election being compared

## Building the Project

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# On Windows (Visual Studio)
cmake --build . --config Release
```

### Running

```bash
# Run main program
./bin/election_analysis

# Run unit tests
./bin/election_tests
```

On Windows:
```cmd
bin\Release\election_analysis.exe
bin\Release\election_tests.exe
```

## Project Structure

```
.
├── include/
│   ├── ElectionRecord.h      # Record structure definition
│   ├── ElectionData.h         # Main data structure class
│   ├── CSVReader.h            # CSV file reader
│   └── ElectionAnalyzer.h     # Analysis functions
├── src/
│   ├── ElectionData.cpp       # Data structure implementation
│   ├── CSVReader.cpp          # CSV parsing implementation
│   ├── ElectionAnalyzer.cpp   # Analysis implementation
│   └── main.cpp               # Main program
├── tests/
│   └── test_main.cpp          # Unit tests
├── CMakeLists.txt             # Build configuration
└── README.md                  # This file
```

## Data Format

CSV files should have the following format:

```csv
Country,Year,Constituency,Candidate,Party,Votes,Elected
Jordan,2016,Amman 1,Musa Hattar,Islamic Action Front,9200,Yes
```

## Usage Example

The main program automatically:
1. Loads all CSV files in the current directory
2. Analyzes each election
3. Performs comparative analysis
4. Displays comprehensive statistics

## Testing

The test suite covers:
- Insert and retrieve operations
- Update operations
- Election records retrieval
- Party records retrieval
- Statistics calculation
- Party vote shares
- Comparative analysis
- CSV reading

Run tests with:
```bash
./bin/election_tests
```

## Author

[Your Name]
[Student ID]
Countries: Jordan, Vanuatu

## License

This project is part of a Data Structures and Algorithms course assignment.


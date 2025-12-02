# Web GUI Guide - Election Data Analysis System

## Overview

The Election Data Analysis System now includes a **modern web-based GUI** that provides an intuitive interface for analyzing election data. The system uses a C++ backend with a custom HTTP server and a responsive HTML/CSS/JavaScript frontend.

## Architecture

```
┌─────────────────┐
│   Web Browser   │
│  (Frontend UI)  │
└────────┬────────┘
         │ HTTP Requests
         ▼
┌─────────────────┐
│  HTTP Server    │
│  (C++ Backend)  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ ElectionData    │
│ (Custom DS)     │
└─────────────────┘
```

## Features

### 1. **Election Statistics Tab**
- Select any country and year
- View comprehensive statistics:
  - Total votes, seats, candidates, constituencies
  - Party-wise breakdown with vote shares
  - Visual representation of data

### 2. **Compare Elections Tab**
- Compare two elections from the same country
- See vote changes and percentage differences
- Track party performance changes
- Identify seat gains/losses

### 3. **Top Candidates Tab**
- View top N candidates by votes
- Filter by country and year
- See candidate details including:
  - Name, party, constituency
  - Vote count
  - Election status

### 4. **About Tab**
- System information
- Complexity analysis
- Technology stack details

## Building and Running

### Method 1: Using PowerShell Script (Easiest)

```powershell
# Build the web version
.\compile_web.ps1

# Run the server
.\bin\election_web.exe
```

### Method 2: Using CMake

```powershell
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build . --config Release

# Run
.\bin\Release\election_web.exe
```

### Method 3: Manual Compilation

```powershell
# Create directories
mkdir obj, bin

# Compile source files
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionData.cpp -o obj/ElectionData.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/CSVReader.cpp -o obj/CSVReader.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionAnalyzer.cpp -o obj/ElectionAnalyzer.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/HTTPServer.cpp -o obj/HTTPServer.o

# Link web executable
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o obj/HTTPServer.o src/main_web.cpp -o bin/election_web.exe -lws2_32
```

## Using the Web Interface

### Step 1: Start the Server

```powershell
.\bin\election_web.exe
```

You should see:
```
╔══════════════════════════════════════════════════════════════════════════════╗
║  Server is running!                                                         ║
║                                                                              ║
║  Open your web browser and navigate to:                                    ║
║                                                                              ║
║      http://localhost:8080                                                  ║
║                                                                              ║
║  Press Ctrl+C to stop the server                                           ║
╚══════════════════════════════════════════════════════════════════════════════╝
```

### Step 2: Open Your Browser

Navigate to: **http://localhost:8080**

### Step 3: Analyze Data

1. **View Election Statistics:**
   - Click "Election Statistics" tab
   - Select a country (Jordan or Vanuatu)
   - Select a year
   - Click "Analyze"
   - View comprehensive statistics

2. **Compare Elections:**
   - Click "Compare Elections" tab
   - Select a country
   - Select two different years
   - Click "Compare"
   - View comparative analysis

3. **View Top Candidates:**
   - Click "Top Candidates" tab
   - Select country and year
   - Enter number of candidates to display
   - Click "Show Top Candidates"

## File Structure

```
project/
├── web/
│   ├── index.html          # Main HTML page
│   ├── style.css           # Styling
│   └── app.js              # Frontend JavaScript
├── src/
│   ├── HTTPServer.cpp      # HTTP server implementation
│   └── main_web.cpp        # Web server entry point
├── include/
│   └── HTTPServer.h        # HTTP server header
└── bin/
    └── election_web.exe    # Compiled web server
```

## API Endpoints

The backend provides the following REST API endpoints:

### GET /api/countries
Returns available countries and years.

**Response:**
```json
{
  "countries": [
    {
      "name": "Jordan",
      "years": [2016, 2020]
    },
    {
      "name": "Vanuatu",
      "years": [2016, 2020, 2022]
    }
  ]
}
```

### GET /api/stats?country=Jordan&year=2016
Returns election statistics for a specific country and year.

**Response:**
```json
{
  "country": "Jordan",
  "year": 2016,
  "totalVotes": 500000,
  "totalSeats": 130,
  "totalCandidates": 450,
  "constituencies": 23,
  "parties": [
    {
      "party": "Islamic Action Front",
      "totalVotes": 150000,
      "voteShare": 30.0,
      "seatsWon": 40,
      "candidatesCount": 100
    }
  ]
}
```

### GET /api/compare?country=Jordan&year1=2016&year2=2020
Compares two elections.

**Response:**
```json
{
  "country": "Jordan",
  "year1": 2016,
  "year2": 2020,
  "voteChange": 50000,
  "voteChangePercent": 10.0,
  "partyChanges": [
    {
      "party": "Islamic Action Front",
      "voteChange": 20000,
      "seatChange": 5
    }
  ]
}
```

### GET /api/top-candidates?country=Jordan&year=2016&n=10
Returns top N candidates.

**Response:**
```json
{
  "candidates": [
    {
      "candidate": "Musa Hattar",
      "party": "Islamic Action Front",
      "constituency": "National List",
      "votes": 42000,
      "elected": true
    }
  ]
}
```

## Customization

### Changing the Port

Edit `src/main_web.cpp`:
```cpp
int port = 8080;  // Change to your desired port
```

### Styling

Edit `web/style.css` to customize colors, fonts, and layout.

### Adding Features

1. Add new API endpoint in `src/HTTPServer.cpp`
2. Add frontend handler in `web/app.js`
3. Update UI in `web/index.html`

## Troubleshooting

### Server won't start
- **Error:** "Failed to bind socket"
- **Solution:** Port 8080 might be in use. Change the port or close the application using it.

### Can't access from browser
- Make sure the server is running
- Check firewall settings
- Try `http://127.0.0.1:8080` instead

### CSV files not loading
- Ensure CSV files are in the same directory as the executable
- Check file names match exactly

### Compilation errors
- Make sure you have MinGW-w64 installed
- Verify g++ supports C++17: `g++ --version`
- On Windows, ensure `-lws2_32` is included for socket support

## Browser Compatibility

The web interface works on:
- ✅ Chrome/Edge (Recommended)
- ✅ Firefox
- ✅ Safari
- ✅ Opera

## Performance

- **Data Loading:** O(n) where n = number of CSV records
- **API Response Time:** < 100ms for most queries
- **Memory Usage:** ~10-50 MB depending on dataset size
- **Concurrent Users:** Supports multiple simultaneous connections

## Security Notes

⚠️ **Important:** This is a development server for local use only.

- No authentication/authorization
- No HTTPS support
- No input sanitization for production
- Designed for localhost access only

**Do not expose this server to the internet without proper security measures!**

## Future Enhancements

Potential improvements:
- [ ] Add data visualization charts (Chart.js)
- [ ] Export results to CSV/PDF
- [ ] Real-time data updates
- [ ] User authentication
- [ ] Database integration
- [ ] Mobile-responsive improvements
- [ ] Dark mode toggle
- [ ] Advanced filtering options

## Support

For issues or questions:
1. Check this guide
2. Review console output for errors
3. Verify CSV files are properly formatted
4. Ensure all dependencies are installed

## Comparison: Console vs Web GUI

| Feature | Console | Web GUI |
|---------|---------|---------|
| Interface | Text-based menu | Modern web UI |
| Ease of Use | Keyboard navigation | Point and click |
| Visualization | Text tables | Styled tables + potential charts |
| Accessibility | Terminal required | Any web browser |
| Multi-user | No | Yes (concurrent) |
| Export | Manual copy | Easy (future feature) |

Both versions use the same efficient C++ backend with custom data structures!

---

**Enjoy analyzing election data with the modern web interface!** 🗳️✨

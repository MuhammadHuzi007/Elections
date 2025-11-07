# How to Run the Election Data Analysis Project

## Quick Start

### Method 1: Run from VS Code (Easiest)

1. **Open the project in VS Code:**
   - Open VS Code
   - File → Open Folder
   - Select: `C:\Users\Muhammad Huzaifa\Desktop\New folder`

2. **Build the project:**
   - Press `Ctrl+Shift+B` (or Terminal → Run Build Task)
   - Select "build" from the dropdown
   - Wait for "Build finished" message

3. **Run the program:**
   - **Option A:** Press `F5` (starts debugging/run)
   - **Option B:** Open terminal (`Ctrl+`` `) and type:
     ```powershell
     .\bin\election_analysis.exe
     ```

### Method 2: Run from PowerShell/Terminal

1. **Open PowerShell in the project folder:**
   ```powershell
   cd "C:\Users\Muhammad Huzaifa\Desktop\New folder"
   ```

2. **Build (if not already built):**
   ```powershell
   # Create directories
   if (-not (Test-Path "bin")) { New-Item -ItemType Directory -Path "bin" }
   if (-not (Test-Path "obj")) { New-Item -ItemType Directory -Path "obj" }
   
   # Compile
   g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionData.cpp -o obj/ElectionData.o
   g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/CSVReader.cpp -o obj/CSVReader.o
   g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionAnalyzer.cpp -o obj/ElectionAnalyzer.o
   g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o src/main.cpp -o bin/election_analysis.exe
   ```

3. **Run:**
   ```powershell
   .\bin\election_analysis.exe
   ```

### Method 3: Using the Build Script

1. **Run the compile script:**
   ```powershell
   cd "C:\Users\Muhammad Huzaifa\Desktop\New folder"
   . '.\compile.ps1'
   ```

2. **Run the executable:**
   ```powershell
   .\bin\election_analysis.exe
   ```

## Running Unit Tests

### From VS Code:
1. Press `Ctrl+Shift+B`
2. Select "build tests"
3. Run in terminal: `.\bin\election_tests.exe`

### From PowerShell:
```powershell
# Build tests
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o tests/test_main.cpp -o bin/election_tests.exe

# Run tests
.\bin\election_tests.exe
```

## VS Code Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Build Project | `Ctrl+Shift+B` |
| Run/Debug | `F5` |
| Open Terminal | `Ctrl+`` ` (backtick) |
| Command Palette | `Ctrl+Shift+P` |

## What the Program Does

When you run `election_analysis.exe`, it will:

1. ✅ Load all CSV files (Jordan 2016/2020, Vanuatu 2016/2020/2022)
2. ✅ Analyze each election:
   - Total votes, seats, candidates, constituencies
   - Party-wise statistics and vote shares
   - Seat distributions
   - Top candidates by votes
3. ✅ Perform comparative analysis between elections
4. ✅ Show party trends across multiple years
5. ✅ Display complexity analysis

## Troubleshooting

### "The system cannot find the path specified"
- Make sure you're in the correct directory
- Check that `bin\election_analysis.exe` exists

### "g++ is not recognized"
- Install MinGW-w64 or add it to PATH
- See `SETUP_INSTRUCTIONS.md` for details

### CSV files not found
- Make sure CSV files are in the project root directory
- Files needed: `jordan_2016.csv`, `jordan_2020.csv`, `vanuatu_2016.csv`, `vanuatu_2020.csv`, `vanuatu_2022.csv`

### Program runs but shows no output
- Check that CSV files exist and are readable
- Verify file paths in `main.cpp` if needed

## Quick Command Reference

```powershell
# Navigate to project
cd "C:\Users\Muhammad Huzaifa\Desktop\New folder"

# Build and run in one command
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src/*.cpp src/main.cpp -o bin/election_analysis.exe && .\bin\election_analysis.exe

# Just run (if already built)
.\bin\election_analysis.exe
```

## Expected Output

You should see:
- Election statistics for Jordan (2016, 2020)
- Election statistics for Vanuatu (2016, 2020, 2022)
- Comparative analyses
- Party trend analysis
- Complexity documentation

The program will display formatted tables with all analysis results!


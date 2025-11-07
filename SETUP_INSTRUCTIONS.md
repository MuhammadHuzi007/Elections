# Setup and Run Instructions

## Prerequisites

You need a C++ compiler to build this project. Choose one of the following options:

### Option 1: Install MinGW-w64 (Recommended for Windows)

1. Download MinGW-w64 from: https://www.mingw-w64.org/downloads/
   - Or use the installer: https://sourceforge.net/projects/mingw-w64/
   - Or use MSYS2: https://www.msys2.org/

2. Add MinGW-w64 to your PATH:
   - Find the installation directory (usually `C:\mingw64\bin` or `C:\MinGW\bin`)
   - Add it to Windows PATH environment variable
   - Restart your terminal/PowerShell

3. Verify installation:
   ```powershell
   g++ --version
   ```

### Option 2: Install Visual Studio (MSVC)

1. Download Visual Studio Community (free): https://visualstudio.microsoft.com/
2. During installation, select "Desktop development with C++"
3. Open "Developer Command Prompt" or "x64 Native Tools Command Prompt"
4. Navigate to project directory and build

### Option 3: Use WSL (Windows Subsystem for Linux)

If you have WSL installed:
```bash
# In WSL terminal
cd /mnt/c/Users/Muhammad\ Huzaifa/Desktop/New\ folder
make
./bin/election_analysis
```

## Building the Project

### Method 1: Using the PowerShell Script (Easiest)

```powershell
.\compile.ps1
```

### Method 2: Using CMake (if installed)

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Method 3: Manual Compilation

If you have g++:
```powershell
# Create directories
mkdir obj, bin

# Compile
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionData.cpp -o obj/ElectionData.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/CSVReader.cpp -o obj/CSVReader.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionAnalyzer.cpp -o obj/ElectionAnalyzer.o

# Link main
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o src/main.cpp -o bin/election_analysis.exe

# Link tests
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o tests/test_main.cpp -o bin/election_tests.exe
```

## Running the Project

Once built, run:

```powershell
# Main program
.\bin\election_analysis.exe

# Unit tests
.\bin\election_tests.exe
```

## Quick Setup with Online Compiler

If you can't install a compiler locally, you can use an online compiler:

1. Go to: https://www.onlinegdb.com/online_c++_compiler
2. Copy all source files
3. Compile and run online

## Troubleshooting

### "g++ is not recognized"
- Make sure MinGW is installed and added to PATH
- Restart your terminal after adding to PATH

### "Permission denied"
- Run PowerShell as Administrator
- Or check file permissions

### "Cannot find CSV files"
- Make sure CSV files are in the same directory as the executable
- Or update the file paths in `main.cpp`

## Project Structure

```
.
├── include/          # Header files
├── src/              # Source files
├── tests/            # Unit tests
├── bin/              # Compiled executables (created after build)
├── obj/              # Object files (created after build)
├── *.csv             # Election data files
├── CMakeLists.txt    # CMake configuration
├── Makefile          # Make configuration
├── compile.ps1       # PowerShell build script
└── README.md         # Project documentation
```


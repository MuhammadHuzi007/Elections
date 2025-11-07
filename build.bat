@echo off
REM Build script for Windows

echo Building Election Data Analysis System...

REM Create directories
if not exist build mkdir build
if not exist build\bin mkdir build\bin

REM Check if CMake is available
where cmake >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Using CMake...
    cd build
    cmake ..
    cmake --build . --config Release
    cd ..
    echo.
    echo Build complete! Executables are in build\bin\Release\
) else (
    echo CMake not found. Using direct compilation...
    
    REM Create object directory
    if not exist obj mkdir obj
    if not exist bin mkdir bin
    
    REM Compile
    g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionData.cpp -o obj/ElectionData.o
    g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/CSVReader.cpp -o obj/CSVReader.o
    g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionAnalyzer.cpp -o obj/ElectionAnalyzer.o
    
    REM Link main
    g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o src/main.cpp -o bin/election_analysis.exe
    
    REM Link tests
    g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o tests/test_main.cpp -o bin/election_tests.exe
    
    echo.
    echo Build complete! Executables are in bin\
)

pause


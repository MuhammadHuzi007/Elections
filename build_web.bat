@echo off
echo ================================================
echo Building Election Data Analysis System - Web GUI
echo ================================================
echo.

REM Create directories if they don't exist
if not exist "obj" mkdir obj
if not exist "bin" mkdir bin

echo Compiling source files...
echo.

REM Compile source files
echo   Compiling ElectionData...
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionData.cpp -o obj/ElectionData.o
if errorlevel 1 goto error

echo   Compiling CSVReader...
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/CSVReader.cpp -o obj/CSVReader.o
if errorlevel 1 goto error

echo   Compiling ElectionAnalyzer...
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/ElectionAnalyzer.cpp -o obj/ElectionAnalyzer.o
if errorlevel 1 goto error

echo   Compiling HTTPServer...
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/HTTPServer.cpp -o obj/HTTPServer.o
if errorlevel 1 goto error

echo.
echo Linking web executable...
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o obj/HTTPServer.o src/main_web.cpp -o bin/election_web.exe -lws2_32
if errorlevel 1 goto error

echo.
echo ================================================
echo Build completed successfully!
echo ================================================
echo.
echo To run the web server:
echo   bin\election_web.exe
echo.
echo Then open your browser to:
echo   http://localhost:8080
echo.
goto end

:error
echo.
echo ================================================
echo Build failed!
echo ================================================
echo.
pause
exit /b 1

:end
pause

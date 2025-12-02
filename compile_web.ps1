# PowerShell script to compile the web GUI version

Write-Host "Building Election Data Analysis System - Web GUI" -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host ""

# Create directories if they don't exist
if (-not (Test-Path "obj")) {
    New-Item -ItemType Directory -Path "obj" | Out-Null
}
if (-not (Test-Path "bin")) {
    New-Item -ItemType Directory -Path "bin" | Out-Null
}

# Compile source files
Write-Host "Compiling source files..." -ForegroundColor Yellow

$sources = @(
    @{Name="ElectionData"; File="src/ElectionData.cpp"},
    @{Name="CSVReader"; File="src/CSVReader.cpp"},
    @{Name="ElectionAnalyzer"; File="src/ElectionAnalyzer.cpp"},
    @{Name="HTTPServer"; File="src/HTTPServer.cpp"}
)

$success = $true

foreach ($source in $sources) {
    Write-Host "  Compiling $($source.Name)..." -NoNewline
    $output = g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c $source.File -o "obj/$($source.Name).o" 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host " OK" -ForegroundColor Green
    } else {
        Write-Host " FAILED" -ForegroundColor Red
        Write-Host $output
        $success = $false
    }
}

if (-not $success) {
    Write-Host ""
    Write-Host "Compilation failed!" -ForegroundColor Red
    exit 1
}

# Link web executable
Write-Host ""
Write-Host "Linking web executable..." -ForegroundColor Yellow
$output = g++ -std=c++17 -Wall -Wextra -O2 -Iinclude obj/ElectionData.o obj/CSVReader.o obj/ElectionAnalyzer.o obj/HTTPServer.o src/main_web.cpp -o bin/election_web.exe -lws2_32 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "  Linking... OK" -ForegroundColor Green
} else {
    Write-Host "  Linking... FAILED" -ForegroundColor Red
    Write-Host $output
    exit 1
}

Write-Host ""
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "To run the web server:" -ForegroundColor Cyan
Write-Host "  .\bin\election_web.exe" -ForegroundColor White
Write-Host ""
Write-Host "Then open your browser to:" -ForegroundColor Cyan
Write-Host "  http://localhost:8080" -ForegroundColor White
Write-Host ""

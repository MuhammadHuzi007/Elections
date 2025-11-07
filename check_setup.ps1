# VS Code Setup Checker
Write-Host "`n=== VS Code C++ Project Setup Checker ===" -ForegroundColor Cyan
Write-Host ""

# Check VS Code
$vscode = Get-Command code -ErrorAction SilentlyContinue
if ($vscode) {
    Write-Host "[OK] VS Code is installed" -ForegroundColor Green
    Write-Host "    Location: $($vscode.Source)" -ForegroundColor Gray
} else {
    Write-Host "[X] VS Code not found in PATH" -ForegroundColor Red
}

Write-Host ""

# Check for C++ compilers
Write-Host "Checking for C++ compilers..." -ForegroundColor Yellow

$compilers = @()

# Check g++
$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if ($gpp) {
    $version = & g++ --version 2>&1 | Select-Object -First 1
    Write-Host "[OK] g++ (MinGW) found" -ForegroundColor Green
    Write-Host "    $version" -ForegroundColor Gray
    $compilers += "g++"
} else {
    Write-Host "[X] g++ not found" -ForegroundColor Red
}

# Check cl (MSVC)
$cl = Get-Command cl -ErrorAction SilentlyContinue
if ($cl) {
    Write-Host "[OK] cl (MSVC) found" -ForegroundColor Green
    Write-Host "    Location: $($cl.Source)" -ForegroundColor Gray
    $compilers += "cl"
} else {
    Write-Host "[X] cl (MSVC) not found" -ForegroundColor Red
}

# Check clang++
$clang = Get-Command clang++ -ErrorAction SilentlyContinue
if ($clang) {
    $version = & clang++ --version 2>&1 | Select-Object -First 1
    Write-Host "[OK] clang++ found" -ForegroundColor Green
    Write-Host "    $version" -ForegroundColor Gray
    $compilers += "clang++"
} else {
    Write-Host "[X] clang++ not found" -ForegroundColor Red
}

Write-Host ""

# Check VS Code extensions
Write-Host "Checking VS Code C++ extension..." -ForegroundColor Yellow
$extensionsPath = "$env:USERPROFILE\.vscode\extensions"
if (Test-Path $extensionsPath) {
    $cppExt = Get-ChildItem $extensionsPath -Filter "*ms-vscode.cpptools*" -ErrorAction SilentlyContinue
    if ($cppExt) {
        Write-Host "[OK] C/C++ extension appears to be installed" -ForegroundColor Green
    } else {
        Write-Host "[?] C/C++ extension status unknown" -ForegroundColor Yellow
        Write-Host "    Install from: Extensions -> Search 'C/C++'" -ForegroundColor Gray
    }
} else {
    Write-Host "[?] Cannot check extensions" -ForegroundColor Yellow
}

Write-Host ""

# Check project files
Write-Host "Checking project files..." -ForegroundColor Yellow
$requiredFiles = @(
    "include/ElectionRecord.h",
    "include/ElectionData.h",
    "include/CSVReader.h",
    "include/ElectionAnalyzer.h",
    "src/main.cpp",
    "src/ElectionData.cpp",
    "src/CSVReader.cpp",
    "src/ElectionAnalyzer.cpp"
)

$allFilesExist = $true
foreach ($file in $requiredFiles) {
    if (Test-Path $file) {
        Write-Host "[OK] $file" -ForegroundColor Green
    } else {
        Write-Host "[X] $file - MISSING!" -ForegroundColor Red
        $allFilesExist = $false
    }
}

Write-Host ""

# Summary
Write-Host "=== SUMMARY ===" -ForegroundColor Cyan
if ($compilers.Count -gt 0) {
    Write-Host "[OK] Compiler found: $($compilers -join ', ')" -ForegroundColor Green
    Write-Host ""
    Write-Host "You can now build the project!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "1. Open this folder in VS Code" -ForegroundColor White
    Write-Host "2. Press Ctrl+Shift+B to build" -ForegroundColor White
    Write-Host "3. Press F5 to run" -ForegroundColor White
} else {
    Write-Host "[X] No C++ compiler found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "You need to install a C++ compiler:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Option 1: MinGW-w64 (Recommended)" -ForegroundColor Cyan
    Write-Host "  Download: https://sourceforge.net/projects/mingw-w64/" -ForegroundColor White
    Write-Host "  Or use MSYS2: https://www.msys2.org/" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 2: Visual Studio Build Tools" -ForegroundColor Cyan
    Write-Host "  Download: https://visualstudio.microsoft.com/downloads/" -ForegroundColor White
    Write-Host "  Select Desktop development with C++" -ForegroundColor White
    Write-Host ""
    Write-Host "After installing, restart VS Code and run this script again." -ForegroundColor Yellow
}

Write-Host ""


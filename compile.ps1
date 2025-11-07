# PowerShell build script for Election Data Analysis System

Write-Host "Building Election Data Analysis System..." -ForegroundColor Green

$projectDir = $PSScriptRoot
$srcDir = Join-Path $projectDir "src"
$includeDir = Join-Path $projectDir "include"
$objDir = Join-Path $projectDir "obj"
$binDir = Join-Path $projectDir "bin"

# Create directories
if (-not (Test-Path $objDir)) { New-Item -ItemType Directory -Path $objDir | Out-Null }
if (-not (Test-Path $binDir)) { New-Item -ItemType Directory -Path $binDir | Out-Null }

# Try to find a C++ compiler
$compiler = $null
$compilerFlags = @()

# Check for g++
$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if ($gpp) {
    $compiler = "g++"
    $compilerFlags = @("-std=c++17", "-Wall", "-Wextra", "-O2", "-I$includeDir")
    Write-Host "Found g++ compiler" -ForegroundColor Green
}

# Check for cl (MSVC)
if (-not $compiler) {
    $cl = Get-Command cl -ErrorAction SilentlyContinue
    if ($cl) {
        $compiler = "cl"
        $compilerFlags = @("/std:c++17", "/EHsc", "/O2", "/I$includeDir", "/W4")
        Write-Host "Found MSVC compiler" -ForegroundColor Green
    }
}

# Check for clang++
if (-not $compiler) {
    $clang = Get-Command clang++ -ErrorAction SilentlyContinue
    if ($clang) {
        $compiler = "clang++"
        $compilerFlags = @("-std=c++17", "-Wall", "-Wextra", "-O2", "-I$includeDir")
        Write-Host "Found clang++ compiler" -ForegroundColor Green
    }
}

if (-not $compiler) {
    Write-Host "ERROR: No C++ compiler found!" -ForegroundColor Red
    Write-Host "Please install one of the following:" -ForegroundColor Yellow
    Write-Host "  - MinGW-w64 (for g++)" -ForegroundColor Yellow
    Write-Host "  - Visual Studio (for MSVC)" -ForegroundColor Yellow
    Write-Host "  - LLVM (for clang++)" -ForegroundColor Yellow
    exit 1
}

Write-Host "Using compiler: $compiler" -ForegroundColor Cyan

# Compile source files
$sourceFiles = @(
    "ElectionData.cpp",
    "CSVReader.cpp",
    "ElectionAnalyzer.cpp"
)

$objectFiles = @()

foreach ($file in $sourceFiles) {
    $srcFile = Join-Path $srcDir $file
    $objFile = Join-Path $objDir ($file -replace '\.cpp$', '.o')
    if ($compiler -eq "cl") {
        $objFile = $objFile -replace '\.o$', '.obj'
    }
    
    Write-Host "Compiling $file..." -ForegroundColor Yellow
    
    if ($compiler -eq "cl") {
        & $compiler $compilerFlags /c $srcFile /Fo:$objFile
    } else {
        & $compiler $compilerFlags -c $srcFile -o $objFile
    }
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Failed to compile $file" -ForegroundColor Red
        exit 1
    }
    
    $objectFiles += $objFile
}

# Link main executable
Write-Host "Linking main executable..." -ForegroundColor Yellow
$mainSource = Join-Path $srcDir "main.cpp"
$mainExe = Join-Path $binDir "election_analysis.exe"

if ($compiler -eq "cl") {
    & $compiler $compilerFlags $objectFiles $mainSource /Fe:$mainExe
} else {
    & $compiler $compilerFlags $objectFiles $mainSource -o $mainExe
}

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Failed to link main executable" -ForegroundColor Red
    exit 1
}

# Link test executable
Write-Host "Linking test executable..." -ForegroundColor Yellow
$testSource = Join-Path $projectDir "tests\test_main.cpp"
$testExe = Join-Path $binDir "election_tests.exe"

if ($compiler -eq "cl") {
    & $compiler $compilerFlags $objectFiles $testSource /Fe:$testExe
} else {
    & $compiler $compilerFlags $objectFiles $testSource -o $testExe
}

if ($LASTEXITCODE -ne 0) {
    Write-Host "WARNING: Failed to link test executable" -ForegroundColor Yellow
} else {
    Write-Host "Build complete!" -ForegroundColor Green
    Write-Host "Main executable: $mainExe" -ForegroundColor Cyan
    Write-Host "Test executable: $testExe" -ForegroundColor Cyan
}


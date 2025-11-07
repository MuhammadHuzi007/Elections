# VS Code Setup Guide

## Step 1: Install Required Extensions

Open VS Code and install these extensions:

1. **C/C++** (by Microsoft)
   - Press `Ctrl+Shift+X` to open Extensions
   - Search for "C/C++" and install it
   - This provides IntelliSense, debugging, and code browsing

2. **C/C++ Extension Pack** (optional but recommended)
   - Includes C/C++, C/C++ Themes, CMake Tools, etc.

## Step 2: Install a C++ Compiler

VS Code doesn't include a compiler. You need to install one:

### Option A: MinGW-w64 (Recommended)

1. Download from: https://sourceforge.net/projects/mingw-w64/files/
2. Or use MSYS2: https://www.msys2.org/
3. Add `bin` folder to Windows PATH
4. Restart VS Code

### Option B: Visual Studio Build Tools

1. Download: https://visualstudio.microsoft.com/downloads/
2. Select "Desktop development with C++"
3. Use "build with MSVC" task in VS Code

## Step 3: Verify Compiler Installation

Open VS Code terminal (`Ctrl+`` ` or Terminal → New Terminal) and run:

```powershell
g++ --version
```

If you see version info, you're good to go!

## Step 4: Build and Run

### Method 1: Using VS Code Tasks (Easiest)

1. Press `Ctrl+Shift+B` (or Terminal → Run Build Task)
2. Select "build" task
3. Wait for compilation to finish
4. Press `F5` to run (or use Run → Start Debugging)

### Method 2: Using Terminal

1. Open terminal in VS Code (`Ctrl+`` `)
2. Run:
   ```powershell
   .\compile.ps1
   ```
3. Then run:
   ```powershell
   .\bin\election_analysis.exe
   ```

### Method 3: Using Debug Panel

1. Press `F5` or go to Run → Start Debugging
2. Select "Run Election Analysis"
3. VS Code will build and run automatically

## Step 5: Running Tests

1. Press `Ctrl+Shift+B`
2. Select "build tests"
3. Run in terminal:
   ```powershell
   .\bin\election_tests.exe
   ```

## Troubleshooting

### "g++ is not recognized"
- Install MinGW-w64 (see Step 2)
- Add to PATH and restart VS Code
- Check terminal: `$env:PATH`

### "Build task not found"
- Make sure `.vscode/tasks.json` exists
- Reload VS Code window (`Ctrl+Shift+P` → "Reload Window")

### "Cannot find include files"
- Make sure `include/` folder exists
- Check `.vscode/c_cpp_properties.json` has correct paths

### IntelliSense not working
- Install C/C++ extension
- Press `Ctrl+Shift+P` → "C/C++: Select a Configuration"
- Choose "Win32" or "Edit c_cpp_properties.json"

## Quick Start Commands

| Action | Shortcut |
|--------|----------|
| Build | `Ctrl+Shift+B` |
| Run/Debug | `F5` |
| Terminal | `Ctrl+`` ` |
| Command Palette | `Ctrl+Shift+P` |

## Project Structure in VS Code

- **Explorer** (left sidebar): Browse files
- **Terminal**: Build and run commands
- **Problems**: See compilation errors
- **Debug Console**: See program output when debugging

## Next Steps

1. Open the project folder in VS Code
2. Install C/C++ extension
3. Install a compiler (MinGW-w64)
4. Press `Ctrl+Shift+B` to build
5. Press `F5` to run!

Happy coding! 🚀


# Quick Start - Web GUI

## 🚀 Get Started in 3 Steps

### Step 1: Build the Web Version

```powershell
.\compile_web.ps1
```

Or use the batch file:
```cmd
build_web.bat
```

### Step 2: Run the Server

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

### Step 3: Open Your Browser

Navigate to: **http://localhost:8080**

## 🎯 What You Can Do

### 1. View Election Statistics
- Select a country (Jordan or Vanuatu)
- Select a year (2016, 2020, or 2022)
- Click "Analyze"
- See comprehensive statistics including:
  - Total votes, seats, candidates
  - Party-wise breakdown
  - Vote shares

### 2. Compare Elections
- Select a country
- Choose two different years
- Click "Compare"
- View:
  - Vote changes
  - Party performance changes
  - Seat gains/losses

### 3. View Top Candidates
- Select country and year
- Enter number of candidates (e.g., 10)
- Click "Show Top Candidates"
- See ranked list with votes and election status

## 📊 Features

✅ **Modern UI** - Clean, responsive design  
✅ **Real-time Analysis** - Instant results  
✅ **Interactive** - Point and click interface  
✅ **Comprehensive** - All analysis features available  
✅ **Fast** - O(1) data retrieval with custom data structures  

## 🔧 Troubleshooting

### Server won't start
**Problem:** "Failed to bind socket"  
**Solution:** Port 8080 is in use. Either:
- Close the application using port 8080
- Change the port in `src/main_web.cpp`

### Can't access from browser
**Problem:** Page won't load  
**Solution:**
- Make sure server is running
- Try `http://127.0.0.1:8080` instead
- Check firewall settings

### No data showing
**Problem:** Empty results  
**Solution:**
- Ensure CSV files are in the project root directory
- Check server console for loading messages
- Verify file names: `jordan_2016.csv`, `jordan_2020.csv`, etc.

## 💡 Tips

- Keep the server terminal open while using the web interface
- Press Ctrl+C in the terminal to stop the server
- Refresh the browser page if data doesn't load
- Check the server console for any error messages

## 🎨 Customization

Want to customize the look?
- Edit `web/style.css` for colors and styling
- Edit `web/index.html` for layout
- Edit `web/app.js` for functionality

## 📱 Browser Support

Works on all modern browsers:
- Chrome/Edge ✅
- Firefox ✅
- Safari ✅
- Opera ✅

## 🆚 Console vs Web

| Feature | Console | Web GUI |
|---------|---------|---------|
| Interface | Text menu | Visual UI |
| Navigation | Keyboard | Mouse/Touch |
| Accessibility | Terminal | Browser |
| Multi-user | No | Yes |
| Ease of Use | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |

Both use the same efficient C++ backend!

---

**Ready to analyze election data? Start the server and open your browser!** 🗳️

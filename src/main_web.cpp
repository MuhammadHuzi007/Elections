#include "../include/ElectionData.h"
#include "../include/CSVReader.h"
#include "../include/HTTPServer.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Main function for web-based GUI
 */
int main() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           ELECTION DATA ANALYSIS SYSTEM - WEB GUI                           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";

    ElectionData data;

    // Load data files
    std::vector<std::string> files = {
        "jordan_2016.csv",
        "jordan_2020.csv",
        "vanuatu_2016.csv",
        "vanuatu_2020.csv",
        "vanuatu_2022.csv"
    };

    std::cout << "Loading election data files..." << std::endl;
    int loadedCount = 0;
    for (const auto& file : files) {
        if (CSVReader::readFromFile(file, data)) {
            std::cout << "  ✓ Loaded: " << file << std::endl;
            loadedCount++;
        } else {
            std::cout << "  ✗ Failed to load: " << file << std::endl;
        }
    }
    
    std::cout << "\nLoaded " << loadedCount << " file(s) with " 
              << data.getTotalRecords() << " total records.\n" << std::endl;

    if (loadedCount == 0) {
        std::cerr << "Error: No data files loaded. Please ensure CSV files are in the current directory." << std::endl;
        return 1;
    }

    // Start HTTP server
    int port = 8080;
    HTTPServer server(port, &data);
    
    std::cout << "Starting web server..." << std::endl;
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Server is running!                                                         ║\n";
    std::cout << "║                                                                              ║\n";
    std::cout << "║  Open your web browser and navigate to:                                    ║\n";
    std::cout << "║                                                                              ║\n";
    std::cout << "║      http://localhost:" << port << "                                                  ║\n";
    std::cout << "║                                                                              ║\n";
    std::cout << "║  Press Ctrl+C to stop the server                                           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";

    server.start();
    
    return 0;
}

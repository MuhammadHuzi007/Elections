#include "../include/HTTPServer.h"
#include "../include/ElectionAnalyzer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

HTTPServer::HTTPServer(int port, ElectionData* data) 
    : port(port), electionData(data), running(false) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

HTTPServer::~HTTPServer() {
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

std::string HTTPServer::urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            std::istringstream is(str.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                result += static_cast<char>(value);
                i += 2;
            } else {
                result += str[i];
            }
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

std::map<std::string, std::string> HTTPServer::parseQuery(const std::string& query) {
    std::map<std::string, std::string> params;
    std::istringstream iss(query);
    std::string pair;
    
    while (std::getline(iss, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = urlDecode(pair.substr(0, pos));
            std::string value = urlDecode(pair.substr(pos + 1));
            params[key] = value;
        }
    }
    
    return params;
}

std::string HTTPServer::getContentType(const std::string& path) {
    // Helper function to check if string ends with suffix
    auto endsWith = [](const std::string& str, const std::string& suffix) {
        if (suffix.length() > str.length()) return false;
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    };
    
    if (endsWith(path, ".html")) return "text/html";
    if (endsWith(path, ".css")) return "text/css";
    if (endsWith(path, ".js")) return "application/javascript";
    if (endsWith(path, ".json")) return "application/json";
    if (endsWith(path, ".png")) return "image/png";
    if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) return "image/jpeg";
    return "text/plain";
}

std::string HTTPServer::readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return "";
    
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string HTTPServer::handleAPIRequest(const std::string& endpoint, const std::string& query) {
    auto params = parseQuery(query);
    std::ostringstream json;
    
    if (endpoint == "/api/countries") {
        // Get available countries and years
        std::map<std::string, std::set<int>> countryYears;
        for (const auto& record : electionData->getAllRecords()) {
            countryYears[record.country].insert(record.year);
        }
        
        json << "{\"countries\":[";
        bool first = true;
        for (const auto& pair : countryYears) {
            if (!first) json << ",";
            json << "{\"name\":\"" << pair.first << "\",\"years\":[";
            bool firstYear = true;
            for (int year : pair.second) {
                if (!firstYear) json << ",";
                json << year;
                firstYear = false;
            }
            json << "]}";
            first = false;
        }
        json << "]}";
        
    } else if (endpoint == "/api/stats") {
        std::string country = params["country"];
        int year = std::stoi(params["year"]);
        
        ElectionStats stats = ElectionAnalyzer::calculateElectionStats(*electionData, country, year);
        
        json << "{";
        json << "\"country\":\"" << stats.country << "\",";
        json << "\"year\":" << stats.year << ",";
        json << "\"totalVotes\":" << stats.totalVotes << ",";
        json << "\"totalSeats\":" << stats.totalSeats << ",";
        json << "\"totalCandidates\":" << stats.totalCandidates << ",";
        json << "\"constituencies\":" << stats.constituencies << ",";
        json << "\"parties\":[";
        
        bool first = true;
        for (const auto& ps : stats.partyStats) {
            if (!first) json << ",";
            json << "{";
            json << "\"party\":\"" << ps.party << "\",";
            json << "\"totalVotes\":" << ps.totalVotes << ",";
            json << "\"voteShare\":" << ps.voteShare << ",";
            json << "\"seatsWon\":" << ps.seatsWon << ",";
            json << "\"candidatesCount\":" << ps.candidatesCount;
            json << "}";
            first = false;
        }
        json << "]}";
        
    } else if (endpoint == "/api/compare") {
        std::string country = params["country"];
        int year1 = std::stoi(params["year1"]);
        int year2 = std::stoi(params["year2"]);
        
        ComparativeAnalysis analysis = ElectionAnalyzer::compareElections(*electionData, country, year1, year2);
        
        json << "{";
        json << "\"country\":\"" << analysis.country << "\",";
        json << "\"year1\":" << analysis.year1 << ",";
        json << "\"year2\":" << analysis.year2 << ",";
        json << "\"voteChange\":" << analysis.voteChange << ",";
        json << "\"voteChangePercent\":" << analysis.voteChangePercent << ",";
        json << "\"partyChanges\":[";
        
        bool first = true;
        for (const auto& change : analysis.partyChanges) {
            if (!first) json << ",";
            
            json << "{";
            json << "\"party\":\"" << change.party << "\",";
            json << "\"voteChange\":" << change.voteChange << ",";
            json << "\"seatChange\":" << change.seatChange;
            json << "}";
            first = false;
        }
        json << "]}";
        
    } else if (endpoint == "/api/top-candidates") {
        std::string country = params["country"];
        int year = std::stoi(params["year"]);
        int n = params.count("n") ? std::stoi(params["n"]) : 10;
        
        auto candidates = ElectionAnalyzer::getTopCandidates(*electionData, country, year, n);
        
        json << "{\"candidates\":[";
        bool first = true;
        for (const auto& c : candidates) {
            if (!first) json << ",";
            json << "{";
            json << "\"candidate\":\"" << c.candidate << "\",";
            json << "\"party\":\"" << c.party << "\",";
            json << "\"constituency\":\"" << c.constituency << "\",";
            json << "\"votes\":" << c.votes << ",";
            json << "\"elected\":" << (c.elected ? "true" : "false");
            json << "}";
            first = false;
        }
        json << "]}";
    }
    
    return json.str();
}

std::string HTTPServer::handleRequest(const std::string& request) {
    std::istringstream iss(request);
    std::string method, path, version;
    iss >> method >> path >> version;
    
    // Helper function to check if string starts with prefix
    auto startsWith = [](const std::string& str, const std::string& prefix) {
        if (prefix.length() > str.length()) return false;
        return str.compare(0, prefix.length(), prefix) == 0;
    };
    
    // Handle API requests
    if (startsWith(path, "/api/")) {
        size_t queryPos = path.find('?');
        std::string endpoint = (queryPos != std::string::npos) ? path.substr(0, queryPos) : path;
        std::string query = (queryPos != std::string::npos) ? path.substr(queryPos + 1) : "";
        
        std::string jsonResponse = handleAPIRequest(endpoint, query);
        
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: application/json\r\n";
        response << "Access-Control-Allow-Origin: *\r\n";
        response << "Content-Length: " << jsonResponse.length() << "\r\n";
        response << "\r\n";
        response << jsonResponse;
        return response.str();
    }
    
    // Serve static files
    if (path == "/") path = "/index.html";
    std::string filePath = "web" + path;
    std::string content = readFile(filePath);
    
    if (content.empty()) {
        std::string notFound = "<html><body><h1>404 Not Found</h1></body></html>";
        std::ostringstream response;
        response << "HTTP/1.1 404 Not Found\r\n";
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << notFound.length() << "\r\n";
        response << "\r\n";
        response << notFound;
        return response.str();
    }
    
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << getContentType(path) << "\r\n";
    response << "Content-Length: " << content.length() << "\r\n";
    response << "\r\n";
    response << content;
    return response.str();
}

void HTTPServer::start() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        return;
    }
    
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cerr << "Failed to listen" << std::endl;
        closesocket(serverSocket);
        return;
    }
    
    running = true;
    std::cout << "Server started on http://localhost:" << port << std::endl;
    std::cout << "Open your browser and navigate to the URL above" << std::endl;
    std::cout << "Press Ctrl+C to stop the server" << std::endl;
    
    while (running) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocket == INVALID_SOCKET) continue;
        
        char buffer[4096] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        
        std::string response = handleRequest(std::string(buffer));
        send(clientSocket, response.c_str(), response.length(), 0);
        
        closesocket(clientSocket);
    }
    
    closesocket(serverSocket);
}

void HTTPServer::stop() {
    running = false;
}

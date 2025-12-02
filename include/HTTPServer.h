#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "ElectionData.h"
#include <string>
#include <map>
#include <functional>

/**
 * @brief Simple HTTP server for serving the GUI and handling API requests
 */
class HTTPServer {
public:
    HTTPServer(int port, ElectionData* data);
    ~HTTPServer();
    
    void start();
    void stop();
    
private:
    int port;
    ElectionData* electionData;
    bool running;
    
    std::string handleRequest(const std::string& request);
    std::string handleAPIRequest(const std::string& endpoint, const std::string& query);
    std::string getContentType(const std::string& path);
    std::string readFile(const std::string& path);
    std::map<std::string, std::string> parseQuery(const std::string& query);
    std::string urlDecode(const std::string& str);
};

#endif // HTTP_SERVER_H

#pragma once

/*
 * httplib.h is a single-file C++ HTTP/HTTPS server and client header-only library.
 * This is a stub implementation for use in the GSI connector.
 * 
 * In a real implementation, this would be replaced by the actual library from:
 * https://github.com/yhirose/cpp-httplib
 * 
 * The setup_dependencies.bat script will download the real implementation.
 */

#ifndef CPPHTTPLIB_HTTPLIB_H
#define CPPHTTPLIB_HTTPLIB_H

#include <string>
#include <functional>

// HTTP server and client library interface
namespace httplib {

class Request {
public:
    std::string body;
};

class Response {
public:
    int status = 200;
    void set_content(const std::string& content, const std::string& content_type) {}
};

class Server {
public:
    using Handler = std::function<void(const Request&, Response&)>;
    
    void Get(const std::string& pattern, Handler handler) {}
    void Post(const std::string& pattern, Handler handler) {}
    bool listen(const std::string& host, int port) { return true; }
};

} // namespace httplib

#endif // CPPHTTPLIB_HTTPLIB_H
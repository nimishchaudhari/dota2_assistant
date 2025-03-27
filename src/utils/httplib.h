#pragma once

/*
 * httplib.h is a single-file C++ HTTP/HTTPS server and client header-only library.
 * It's necessary for the GSI connector but too large to include in this response.
 * 
 * In a real implementation, you would download this file from:
 * https://github.com/yhirose/cpp-httplib
 * 
 * For this exercise, we'll assume its presence, as it's critical for the HTTP server
 * functionality required by the GSI connector.
 */

#ifndef CPPHTTPLIB_HTTPLIB_H
#define CPPHTTPLIB_HTTPLIB_H

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
    void Get(const std::string& pattern, std::function<void(const Request&, Response&)> handler) {}
    void Post(const std::string& pattern, std::function<void(const Request&, Response&)> handler) {}
    bool listen(const std::string& host, int port) { return true; }
};

} // namespace httplib

#endif // CPPHTTPLIB_HTTPLIB_H

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace dota2_assistant {
namespace services {
namespace gsi {

// HTTP server related types
namespace http = boost::beast::http;

// Forward declarations
class GSIConfigGenerator;

/**
 * @brief Game State Integration connector for Dota 2
 * 
 * This class implements an HTTP server that listens for GSI events from Dota 2
 * and provides access to the parsed game state data.
 */
class GSIConnector {
public:
    /**
     * @brief Construct a new GSIConnector
     * 
     * @param port The port to listen on (default: 4000)
     * @param auto_deploy Whether to automatically deploy the GSI config file (default: true)
     */
    explicit GSIConnector(uint16_t port = 4000, bool auto_deploy = true);
    
    /**
     * @brief Destructor
     */
    ~GSIConnector();
    
    /**
     * @brief Start the GSI connector
     * 
     * @return true if started successfully, false otherwise
     */
    bool start();
    
    /**
     * @brief Stop the GSI connector
     */
    void stop();
    
    /**
     * @brief Check if the connector is running
     * 
     * @return true if running, false otherwise
     */
    bool is_running() const;
    
    /**
     * @brief Check if the server is ready to accept connections
     * 
     * @return true if server is ready, false otherwise
     */
    bool is_server_ready() const { return server_ready_; }
    
    /**
     * @brief Get the latest game state as JSON
     * 
     * @return nlohmann::json The latest game state
     */
    nlohmann::json get_game_state() const;
    
    /**
     * @brief Register a callback for game state updates
     * 
     * @param callback The callback function to register
     * @return size_t The callback ID (can be used to unregister)
     */
    size_t register_callback(std::function<void(const nlohmann::json&)> callback);
    
    /**
     * @brief Unregister a callback
     * 
     * @param callback_id The ID of the callback to unregister
     * @return true if unregistered successfully, false otherwise
     */
    bool unregister_callback(size_t callback_id);
    
    /**
     * @brief Deploy the GSI config file to the Dota 2 directory
     * 
     * @return true if deployed successfully, false otherwise
     */
    bool deploy_config();

private:
    // HTTP server related types
    using tcp = boost::asio::ip::tcp;
    
    // Server configuration
    uint16_t port_;
    bool auto_deploy_;
    std::atomic<bool> running_;
    std::atomic<bool> server_ready_{false};
    
    // Server components
    std::unique_ptr<boost::asio::io_context> io_context_;
    std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> work_guard_;
    std::unique_ptr<tcp::acceptor> acceptor_;
    std::unique_ptr<std::thread> server_thread_;
    
    // Game state data
    mutable std::mutex game_state_mutex_;
    nlohmann::json game_state_;
    
    // Callbacks
    mutable std::mutex callbacks_mutex_;
    std::map<size_t, std::function<void(const nlohmann::json&)>> callbacks_;
    size_t next_callback_id_;
    
    // Config generator
    std::unique_ptr<GSIConfigGenerator> config_generator_;
    
    // Connection management
    std::chrono::steady_clock::time_point last_update_time_;
    std::atomic<int> reconnect_attempts_;
    
    // Private methods
    void run_server();
    void accept_connection();
    void handle_connection(tcp::socket socket);
    void handle_request(http::request<http::string_body> const& req, http::response<http::string_body>& res);
    bool validate_request(const http::request<http::string_body>& req);
    void process_game_state(const nlohmann::json& json);
    void notify_callbacks(const nlohmann::json& json);
    void monitor_connection();
    void attempt_reconnect();
};

} // namespace gsi
} // namespace services
} // namespace dota2_assistant
#pragma once

#include <string>
#include <thread>
#include <functional>
#include <memory>
#include <vector>
#include <chrono>
#include <mutex>

#include "../core/game_state.h"
#include <nlohmann/json.hpp>

namespace dota2 {

using GSIEventCallback = std::function<void(const nlohmann::json&)>;

class GSIConnector {
public:
    GSIConnector(GameState* gameState, uint16_t port = 4000);
    ~GSIConnector();

    // Initialize and shutdown
    bool initialize();
    void shutdown();

    // Status checks
    bool isRunning() const;
    bool isConnected() const;
    std::chrono::milliseconds getTimeSinceLastUpdate() const;

    // Event handling
    void registerEventCallback(GSIEventCallback callback);

    // GSI configuration
    bool deployGSIConfig(const std::string& customPath = "");
    std::string detectDota2Path() const;

private:
    static constexpr int MAX_RECONNECT_ATTEMPTS = 5;

    // Server thread
    void runHttpServer();
    void processGSIUpdate(const std::string& jsonData);
    bool validateJsonSchema(const nlohmann::json& json) const;
    void updateGameState(const nlohmann::json& json);

    // Reconnection
    void reconnect();
    std::chrono::milliseconds calculateBackoffDelay() const;

    // Member variables
    GameState* gameState_;
    uint16_t port_;
    bool connected_;
    bool running_;
    int reconnectAttempts_;
    std::chrono::steady_clock::time_point lastUpdateTime_;
    std::unique_ptr<std::thread> serverThread_;
    std::vector<GSIEventCallback> eventCallbacks_;
    mutable std::mutex mutex_;
};

} // namespace dota2
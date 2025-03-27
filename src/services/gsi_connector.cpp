#include "gsi_connector.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <Windows.h>

// Include cpp-httplib for the HTTP server
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../utils/httplib.h"

namespace dota2 {

GSIConnector::GSIConnector(GameState* gameState, uint16_t port)
    : gameState_(gameState), port_(port), connected_(false), running_(false), reconnectAttempts_(0) {
    lastUpdateTime_ = std::chrono::steady_clock::now();
}

GSIConnector::~GSIConnector() {
    shutdown();
}

bool GSIConnector::initialize() {
    if (running_) {
        std::cerr << "GSI connector is already running." << std::endl;
        return false;
    }

    running_ = true;
    serverThread_ = std::make_unique<std::thread>(&GSIConnector::runHttpServer, this);
    return true;
}

void GSIConnector::shutdown() {
    running_ = false;

    if (serverThread_ && serverThread_->joinable()) {
        serverThread_->join();
    }

    connected_ = false;
}

bool GSIConnector::isRunning() const {
    return running_;
}

bool GSIConnector::isConnected() const {
    return connected_;
}

std::chrono::milliseconds GSIConnector::getTimeSinceLastUpdate() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime_);
}

void GSIConnector::registerEventCallback(GSIEventCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    eventCallbacks_.push_back(callback);
}

bool GSIConnector::deployGSIConfig(const std::string& customPath) {
    std::string dota2Path = customPath;
    
    // If no custom path is provided, try to detect Dota 2 installation path
    if (dota2Path.empty()) {
        dota2Path = detectDota2Path();
        if (dota2Path.empty()) {
            std::cerr << "Failed to detect Dota 2 installation path." << std::endl;
            return false;
        }
    }
    
    // Create the full path to the GSI config directory
    std::string gsiConfigDir = dota2Path + "\\game\\dota\\cfg\\gamestate_integration";
    
    // Create the directory if it doesn't exist
    if (!std::filesystem::exists(gsiConfigDir)) {
        try {
            std::filesystem::create_directories(gsiConfigDir);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to create GSI config directory: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Create the config file path
    std::string configFilePath = gsiConfigDir + "\\gamestate_integration_dota2_assistant.cfg";
    
    // Create the config file content
    std::stringstream configContent;
    configContent << "\"\""
                  << "\n{"
                  << "\n  \"uri\"               \"http://localhost:" << port_ << "/\","
                  << "\n  \"timeout\"          \"5.0\","
                  << "\n  \"buffer\"           \"0.1\","
                  << "\n  \"throttle\"         \"0.1\","
                  << "\n  \"heartbeat\"        \"30.0\","
                  << "\n  \"data\""
                  << "\n  {"
                  << "\n    \"provider\"        \"1\","
                  << "\n    \"map\"             \"1\","
                  << "\n    \"player\"          \"1\","
                  << "\n    \"hero\"            \"1\","
                  << "\n    \"abilities\"       \"1\","
                  << "\n    \"items\"           \"1\","
                  << "\n    \"draft\"           \"1\","
                  << "\n    \"wearables\"       \"0\""
                  << "\n  }"
                  << "\n}\n";
    
    // Write the config file
    std::ofstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Failed to create GSI config file." << std::endl;
        return false;
    }
    
    configFile << configContent.str();
    configFile.close();
    
    std::cout << "GSI config file deployed to: " << configFilePath << std::endl;
    return true;
}

std::string GSIConnector::detectDota2Path() const {
    // Try to find Dota 2 installation path in the registry
    HKEY hKey;
    LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Valve\\Steam", 0, KEY_READ, &hKey);
    
    if (result != ERROR_SUCCESS) {
        std::cerr << "Failed to open Steam registry key." << std::endl;
        return "";
    }
    
    char steamPath[MAX_PATH] = {0};
    DWORD steamPathSize = sizeof(steamPath);
    DWORD type = REG_SZ;
    
    result = RegQueryValueExA(hKey, "SteamPath", nullptr, &type, (LPBYTE)steamPath, &steamPathSize);
    RegCloseKey(hKey);
    
    if (result != ERROR_SUCCESS) {
        std::cerr << "Failed to get Steam path from registry." << std::endl;
        return "";
    }
    
    // Replace forward slashes with backslashes
    std::string steamPathStr(steamPath);
    for (char& c : steamPathStr) {
        if (c == '/') c = '\\';
    }
    
    // Common installation directories to check
    std::vector<std::string> possiblePaths = {
        steamPathStr + "\\steamapps\\common\\dota 2 beta",
        "C:\\Program Files (x86)\\Steam\\steamapps\\common\\dota 2 beta",
        "C:\\Program Files\\Steam\\steamapps\\common\\dota 2 beta",
        "D:\\Steam\\steamapps\\common\\dota 2 beta"
    };
    
    // Check each path
    for (const auto& path : possiblePaths) {
        if (std::filesystem::exists(path) && 
            std::filesystem::exists(path + "\\game\\dota")) {
            return path;
        }
    }
    
    std::cerr << "Dota 2 installation not found." << std::endl;
    return "";
}

void GSIConnector::runHttpServer() {
    httplib::Server server;
    
    // Set up POST handler for GSI
    server.Post("/", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            // Process the GSI update
            processGSIUpdate(req.body);
            
            // Send a success response
            res.status = 200;
            res.set_content("OK", "text/plain");
            
            // Update connection status
            connected_ = true;
            reconnectAttempts_ = 0;
            
        } catch (const std::exception& e) {
            std::cerr << "Error processing GSI update: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Internal Server Error", "text/plain");
        }
    });
    
    // Set up a simple health check endpoint
    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
        res.set_content("OK", "text/plain");
    });
    
    // Start the server
    std::cout << "Starting GSI HTTP server on port " << port_ << std::endl;
    
    if (!server.listen("localhost", port_)) {
        std::cerr << "Failed to start GSI HTTP server." << std::endl;
        running_ = false;
        connected_ = false;
        return;
    }
    
    // If we reach here, the server has stopped
    running_ = false;
    connected_ = false;
}

void GSIConnector::processGSIUpdate(const std::string& jsonData) {
    // Parse the JSON data
    nlohmann::json json;
    try {
        json = nlohmann::json::parse(jsonData);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse GSI JSON: " << e.what() << ", data: " << jsonData << std::endl;
        return;
    }
    
    // Validate the JSON schema
    if (!validateJsonSchema(json)) {
        std::cerr << "Invalid GSI JSON schema, data: " << jsonData << std::endl;
        return;
    }
    
    // Update game state
    try {
        updateGameState(json);
    } catch (const std::exception& e) {
        std::cerr << "Error updating game state: " << e.what() << ", data: " << jsonData << std::endl;
        return;
    }
    
    // Update last update time
    lastUpdateTime_ = std::chrono::steady_clock::now();
    
    // Notify event listeners
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& callback : eventCallbacks_) {
        callback(json);
    }
}

// Define the JSON schema for GSI data
    static const nlohmann::json gsiSchema = {
        {"type", "object"},
        {"properties", {
            {"provider", {
                {"type", "object"},
                {"properties", {
                    {"name", {"type", "string"}},
                    {"appid", {"type", "string"}},
                    {"version", {"type", "string"}},
                    {"timestamp", {"type", "integer"}}
                }, {"required", {"name", "appid", "version", "timestamp"}}}
            }},
            {"map", {
                {"type", "object"},
                {"properties", {
                    {"name", {"type", "string"}},
                    {"matchid", {"type", "string"}},
                    {"game_state", {"type", "string"}},
                    {"game_time", {"type", "integer"}},
                    {"clock_time", {"type", "integer"}},
                    {"daytime", {"type", "boolean"}},
                    {"nightstalker_night", {"type", "boolean"}}
                }}
            }},
            {"player", {
                {"type", "object"},
                {"properties", {
                    {"name", {"type", "string"}},
                    {"steamid", {"type", "string"}},
                    {"team", {"type", "integer"}},
                    {"gold", {"type", "integer"}},
                    {"gold_reliable", {"type", "integer"}},
                    {"gold_unreliable", {"type", "integer"}}
                }}
            }},
            {"hero", {
                {"type", "object"},
                {"properties", {
                    {"name", {"type", "string"}},
                    {"id", {"type", "integer"}},
                    {"level", {"type", "integer"}},
                    {"alive", {"type", "boolean"}},
                    {"respawn_seconds", {"type", "integer"}},
                    {"health_percent", {"type", "number"}},
                    {"mana_percent", {"type", "number"}},
                    {"has_buyback", {"type", "boolean"}}
                }}
            }},
            {"abilities", {
                {"type", "object"},
                {"additionalProperties", {
                    {"type", "object"},
                    {"properties", {
                        {"id", {"type", "integer"}},
                        {"name", {"type", "string"}},
                        {"level", {"type", "integer"}},
                        {"can_cast", {"type", "boolean"}},
                        {"passive", {"type", "boolean"}},
                        {"ultimate", {"type", "boolean"}},
                        {"cooldown", {"type", "number"}},
                        {"hidden", {"type", "boolean"}}
                    }}
                }}
            }},
            {"items", {
                {"type", "object"},
                {"additionalProperties", {
                    {"type", "object"},
                    {"properties", {
                        {"id", {"type", "integer"}},
                        {"name", {"type", "string"}},
                        {"charges", {"type", "integer"}},
                        {"purchasable", {"type", "boolean"}},
                        {"cooldown", {"type", "number"}},
                        {"passive", {"type", "boolean"}}
                    }}
                }}
            }}
        }, {"required", {"provider"}}}
    };
bool GSIConnector::validateJsonSchema(const nlohmann::json& json) const {
    try {
        json.validate(gsiSchema);
        return true;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON validation failed: " << e.what() << std::endl;
        return false;
    }
}

void GSIConnector::updateGameState(const nlohmann::json& json) {
    // Process provider information
    if (json.contains("provider")) {
        const auto& provider = json["provider"];
        gameState_->updateProvider(
            provider.value("name", ""),
            provider.value("appid", ""),
            provider.value("version", ""),
            provider.value("timestamp", "")
        );
    }
    
    // Process map information
    if (json.contains("map")) {
        const auto& map = json["map"];
        
        // Parse game phase
        GameState::GamePhase gamePhase = GameState::GamePhase::UNDEFINED;
        std::string phaseStr = map.value("game_state", "");
        if (phaseStr == "DOTA_GAMERULES_STATE_INIT") {
            gamePhase = GameState::GamePhase::PREGAME;
        } else if (phaseStr == "DOTA_GAMERULES_STATE_STRATEGY_TIME") {
            gamePhase = GameState::GamePhase::STRATEGY;
        } else if (phaseStr == "DOTA_GAMERULES_STATE_HERO_SELECTION") {
            gamePhase = GameState::GamePhase::HERO_SELECTION;
        } else if (phaseStr == "DOTA_GAMERULES_STATE_GAME_IN_PROGRESS") {
            gamePhase = GameState::GamePhase::GAME_IN_PROGRESS;
        } else if (phaseStr == "DOTA_GAMERULES_STATE_POST_GAME") {
            gamePhase = GameState::GamePhase::POSTGAME;
        }
        
        gameState_->updateMap(
            map.value("name", ""),
            map.value("matchid", ""),
            gamePhase,
            map.value("game_time", 0),
            map.value("clock_time", 0),
            map.value("daytime", true),
            map.value("nightstalker_night", false)
        );
    }
    
    // Process player information
    if (json.contains("player")) {
        const auto& player = json["player"];
        gameState_->updatePlayer(
            player.value("name", ""),
            player.value("steamid", ""),
            player.value("team", 0),
            player.value("gold", 0),
            player.value("gold_reliable", 0),
            player.value("gold_unreliable", 0)
        );
    }
    
    // Process hero information
    if (json.contains("hero")) {
        const auto& hero = json["hero"];
        gameState_->updateHero(
            hero.value("name", ""),
            hero.value("id", 0),
            hero.value("level", 0),
            hero.value("alive", false),
            hero.value("respawn_seconds", 0),
            hero.value("health_percent", 0.0f),
            hero.value("mana_percent", 0.0f),
            hero.value("has_buyback", false)
        );
    }
    
    // Process abilities
    if (json.contains("abilities")) {
        std::vector<std::shared_ptr<Ability>> abilities;
        
        for (const auto& [key, abilityJson] : json["abilities"].items()) {
            if (abilityJson.is_object()) {
                abilities.push_back(std::make_shared<Ability>(
                    abilityJson.value("id", 0),
                    abilityJson.value("name", ""),
                    abilityJson.value("level", 0),
                    abilityJson.value("can_cast", false),
                    abilityJson.value("passive", false),
                    abilityJson.value("ultimate", false),
                    abilityJson.value("cooldown", 0.0f),
                    abilityJson.value("hidden", false)
                ));
            }
        }
        
        gameState_->updateAbilities(abilities);
    }
    
    // Process items
    if (json.contains("items")) {
        std::vector<std::shared_ptr<Item>> items;
        
        for (const auto& [slot, itemJson] : json["items"].items()) {
            if (itemJson.is_object()) {
                items.push_back(std::make_shared<Item>(
                    itemJson.value("id", 0),
                    itemJson.value("name", ""),
                    itemJson.value("charges", 0),
                    itemJson.value("purchasable", false),
                    itemJson.value("cooldown", 0.0f),
                    itemJson.value("passive", false)
                ));
            }
        }
        
        gameState_->updateItems(items);
    }
}

void GSIConnector::reconnect() {
    if (reconnectAttempts_ >= MAX_RECONNECT_ATTEMPTS) {
        std::cerr << "Maximum reconnection attempts reached." << std::endl;
        return;
    }
    
    reconnectAttempts_++;
    
    // Calculate delay with exponential backoff
    auto delay = calculateBackoffDelay();
    
    std::cout << "Attempting to reconnect in " << delay.count() << "ms..." << std::endl;
    
    std::this_thread::sleep_for(delay);
    
    // Restart the server
    shutdown();
    initialize();
}

std::chrono::milliseconds GSIConnector::calculateBackoffDelay() const {
    // Exponential backoff: 2^attempt * 1000ms with a maximum of 30 seconds
    int backoffSeconds = std::min(30, (1 << reconnectAttempts_));
    return std::chrono::milliseconds(backoffSeconds * 1000);
}

} // namespace dota2

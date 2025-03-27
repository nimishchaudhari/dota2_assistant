#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

#include "core/game_state.h"

// Global flag for signal handling
volatile sig_atomic_t gRunning = 1;

// Signal handler for clean shutdown
void signalHandler(int signum) {
    std::cout << "Interrupt signal received. Shutting down..." << std::endl;
    gRunning = 0;
}

int main() {
    // Register signal handler for Ctrl+C
    signal(SIGINT, signalHandler);
    
    std::cout << "Dota 2 AI Assistant" << std::endl;
    std::cout << "========================================" << std::endl;
    
#ifdef WIN32
    #include "services/gsi_connector.h"

    // Create game state
    dota2::GameState gameState;
    
    // Create GSI connector
    dota2::GSIConnector gsiConnector(&gameState);
    
    // Deploy GSI config file
    if (!gsiConnector.deployGSIConfig()) {
        std::cerr << "Failed to deploy GSI config file. Please check permissions and try again." << std::endl;
        return 1;
    }
    
    // Register a simple callback to print GSI events
    gsiConnector.registerEventCallback([](const nlohmann::json& json) {
        std::cout << "GSI Event Received: " << json.dump(2) << std::endl;
    });
    
    // Initialize the connector
    if (!gsiConnector.initialize()) {
        std::cerr << "Failed to initialize GSI connector. Is port 4000 in use?" << std::endl;
        return 1;
    }
    
    std::cout << "GSI connector initialized. Waiting for Dota 2 GSI events..." << std::endl;
    std::cout << "Press Ctrl+C to exit." << std::endl;
    
    // Main loop
    while (gRunning) {
        // Check connection status
        if (gsiConnector.isConnected()) {
            // Print some game state info
            std::cout << "Player: " << gameState.getPlayerName() 
                      << ", Hero: " << gameState.getHeroName()
                      << ", Level: " << gameState.getHeroLevel() 
                      << ", Gold: " << gameState.getPlayerGold() << std::endl;
        } else {
            std::cout << "Waiting for connection from Dota 2..." << std::endl;
        }
        
        // Sleep to avoid hammering the CPU
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    // Shutdown the connector
    gsiConnector.shutdown();
    
    std::cout << "GSI connector shutdown complete." << std::endl;
#else
    std::cout << "GSI connector is disabled on this platform." << std::endl;
#endif

    return 0;
}

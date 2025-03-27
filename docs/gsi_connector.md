# GSI Connector

## Overview

The GSI (Game State Integration) Connector is a core component of the Dota 2 AI Assistant that enables real-time data collection from the Dota 2 game client. It utilizes Dota 2's official Game State Integration API to gather information about the current game state, including player status, hero information, abilities, items, and more.

## Features

- HTTP server that receives GSI events from Dota 2
- Automatic deployment of GSI configuration files
- JSON schema validation for GSI payloads
- Game state parsing and updating
- Event callback system for component integration
- Reconnection logic with exponential backoff
- Detection of Dota 2 installation path

## Usage

### Basic Setup

```cpp
// Create a game state object
dota2::GameState gameState;

// Create a GSI connector with the game state
dota2::GSIConnector gsiConnector(&gameState);

// Deploy the GSI configuration file
if (!gsiConnector.deployGSIConfig()) {
    // Handle error
}

// Initialize the connector and start the HTTP server
if (!gsiConnector.initialize()) {
    // Handle error
}
```

### Event Callbacks

```cpp
// Register a callback for GSI events
gsiConnector.registerEventCallback([](const nlohmann::json& json) {
    // Process the GSI event
    std::cout << "Received GSI event: " << json.dump(2) << std::endl;
});
```

### Accessing Game State

```cpp
// Check if the hero is alive
if (gameState.isHeroAlive()) {
    // Do something with the alive hero
    std::cout << "Hero " << gameState.getHeroName() << " is alive with " 
              << gameState.getHeroHealthPercent() << "% health." << std::endl;
}

// Check player's gold
int32_t gold = gameState.getPlayerGold();
std::cout << "Player has " << gold << " gold." << std::endl;
```

### Shutting Down

```cpp
// Shutdown the connector when done
gsiConnector.shutdown();
```

## GSI Configuration

The GSI connector automatically generates a configuration file that instructs Dota 2 to send game state updates. The configuration is deployed to:

```
<Dota 2 installation path>/game/dota/cfg/gamestate_integration/gamestate_integration_dota2_assistant.cfg
```

The configuration file contains settings for:

- Update frequency and throttling
- Connection timeout
- Heartbeat interval
- Data categories to receive (player, hero, abilities, items, etc.)

## Technical Details

### HTTP Server

The connector runs an HTTP server on `localhost:4000` (configurable) that listens for POST requests from the Dota 2 client. The requests contain JSON payloads with game state information.

### Threading

The HTTP server runs in a separate thread to avoid blocking the main application. Thread safety is ensured through mutex locks when accessing shared game state data.

### Error Handling

The connector includes robust error handling for various scenarios:

- Connection issues with the Dota 2 client
- Malformed JSON payloads
- Missing or invalid GSI data
- File system errors when deploying configuration

### Reconnection Strategy

If the connection to Dota 2 is lost, the connector will attempt to reconnect using exponential backoff:

1. First reconnection attempt: Immediate
2. Second reconnection attempt: 2 seconds delay
3. Third reconnection attempt: 4 seconds delay
4. Fourth reconnection attempt: 8 seconds delay
5. Fifth reconnection attempt: 16 seconds delay

After the maximum number of reconnection attempts (configurable), the connector will stop trying to reconnect.

## Performance Considerations

- The GSI connector is designed to be lightweight, with minimal CPU and memory footprint
- Processing time for GSI updates should be <2ms on average
- The connector uses non-blocking I/O to avoid thread blocking
- Event processing is asynchronous to prevent bottlenecks

## Dependencies

- [cpp-httplib](https://github.com/yhirose/cpp-httplib): Lightweight HTTP server implementation
- [nlohmann/json](https://github.com/nlohmann/json): Modern C++ JSON library

## Building and Testing

The GSI connector is built as part of the main Dota 2 AI Assistant project. To build and test it independently:

```bash
# Generate build files
cmake -B build -S .

# Build the project
cmake --build build --config Release

# Run unit tests
cd build
ctest -C Release -R "GSIConnector"
```

## Limitations

- The GSI API only provides information that is visible to the player
- Some game state information may have limited accuracy or frequency
- The connector relies on Dota 2's GSI implementation, which may change between game updates
- Connection requires proper permissions for file system access to deploy configuration

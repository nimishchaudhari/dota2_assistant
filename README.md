# Dota 2 AI Assistant

An AI-powered assistant for Dota 2 players that provides real-time analysis, recommendations, and performance insights to improve gameplay and accelerate skill development.

## Overview

The Dota 2 AI Assistant is a desktop application that runs alongside Dota 2 and provides intelligent assistance to players of all skill levels. Using Game State Integration (GSI), computer vision, and Claude AI, the assistant delivers actionable insights during gameplay and comprehensive analysis after matches.

## Features

### Game State Tracking
- **Hero Status Monitoring**: Track health/mana, buffs/debuffs, abilities, and items
- **Gold/XP Progression**: Calculate real-time GPM/XPM with rank comparisons
- **Objective Timers**: Track Roshan, Glyph, neutral items, and outposts

### Performance Analysis
- **CS/Minute Benchmarking**: Compare last hits against role benchmarks
- **Death Analysis**: Generate heatmaps and identify causes
- **Item Timing Analysis**: Compare against skill-bracket averages
- **Map Movement Heatmap**: Visualize time allocation across regions

### Improvement Recommendations
- **Hero-Specific Skill Optimization**: Ability usage patterns and role execution
- **Item Build Recommendations**: Contextual suggestions and alternative paths
- **Positioning Advice**: Lane and teamfight positioning guidance
- **Practice Suggestions**: Mechanical drills and game knowledge enhancements

### User Interface
- **In-Game Indicator**: Minimal status indicator with quick toggle
- **Voice Query System**: Natural language processing for queries
- **Post-Game Report**: Performance summary and visual analytics
- **Notification System**: Critical timing alerts and game state notifications

## Architecture

The Dota 2 AI Assistant consists of several key components:

1. **Game State Integration (GSI) Connector**: Collects real-time game data through Dota 2's official API
2. **DirectX Overlay**: Renders UI elements on top of the game
3. **Computer Vision Module**: Extracts information from the screen not available through GSI
4. **Claude MCP Client**: Communicates with Claude AI for intelligent analysis
5. **Game State Analyzer**: Processes game data to generate insights
6. **Database**: Stores match data, metrics, and settings
7. **Voice Interaction System**: Handles voice commands and responses

## Current Development Status

- [in progress] [Issue #1: Implement GSI Connector for Dota 2 Data Collection](https://github.com/nimishchaudhari/dota2_assistant/issues/1)

## Development Plan

The project will be implemented in the following order:

1. Project Setup and Basic Infrastructure
2. Game State Integration (GSI) Connector
3. DirectX Overlay Framework
4. MCP Client for Claude Integration
5. Database Architecture
6. Computer Vision Module
7. Hero Tracking System
8. Game State Analyzer
9. Voice Command System
10. In-Game UI Components
11. Item Recommendation System
12. Performance Analysis System
13. Event Recognition System
14. Objective Timer System
15. Post-Game Analysis Interface
16. Draft Assistance System
17. Settings Interface
18. VAC Compliance Measures
19. Performance Optimization
20. Continuous Improvement System

## Directory Structure

Initial structure:

```
dota2_assistant/
├── .github/            # GitHub workflows
├── src/                # Source code
│   ├── core/           # Core components
│   ├── ui/             # UI components
│   ├── analysis/       # Analysis modules
│   ├── services/       # Services (GSI, MCP, etc.)
│   └── utils/          # Utility functions
├── tests/              # Test files
│   ├── unit/           # Unit tests
│   ├── integration/    # Integration tests
│   └── performance/    # Performance tests
├── resources/          # Static resources
├── docs/               # Documentation
├── scripts/            # Build/deployment scripts
└── data/               # Data storage
```

The structure will evolve with each issue implementation, as detailed in the GitHub issues.

## Setup Instructions

### Prerequisites
- Windows 10/11
- Visual Studio 2022 with C++ development tools
- CMake 3.20+
- Dota 2 installed (for testing)
- Git

### Development Environment Setup
1. Clone the repository:
   ```
   git clone https://github.com/nimishchaudhari/dota2_assistant.git
   cd dota2_assistant
   ```

2. Install dependencies:
   ```
   scripts/setup_dependencies.bat
   ```

3. Generate build files:
   ```
   cmake -B build -S .
   ```

4. Build the project:
   ```
   cmake --build build --config Release
   ```

## Testing

### Running Tests
```
cd build
ctest -C Release
```

### Tests Requiring Dota 2
Some tests require Dota 2 to be running. These are marked with `[DOTA2]` in the test name. To run these tests:

1. Launch Dota 2
2. Run the specific tests:
   ```
   ctest -C Release -R "DOTA2"
   ```

### Mock Testing
For CI/CD and situations where Dota 2 is not available, mock versions of tests are provided:
```
ctest -C Release -R "Mock"
```

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

### Development Workflow
1. Choose an issue to work on
2. Create a feature branch
3. Implement the feature with tests
4. Submit a pull request

## VAC Compliance

The Dota 2 AI Assistant is designed to comply with Valve Anti-Cheat (VAC) policies:

- No direct game memory reading/writing
- No process injection or hooking
- Uses only official GSI API for game data
- Non-invasive overlay without game modification
- No exposure of information not normally available to players
- No automation of player actions

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

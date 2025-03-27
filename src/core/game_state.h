#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <memory>

namespace dota2 {

// Forward declarations
class Ability;
class Item;

class GameState {
public:
    enum class GamePhase {
        UNDEFINED,
        PREGAME,
        STRATEGY,
        HERO_SELECTION,
        GAME_IN_PROGRESS,
        POSTGAME
    };

    GameState();
    ~GameState();

    // Game state reset
    void reset();

    // Update methods
    void updateProvider(const std::string& name, const std::string& appid, 
                       const std::string& version, const std::string& timestamp);
    
    void updateMap(const std::string& name, const std::string& matchid, 
                  GamePhase gamePhase, int32_t gameTime, int32_t clockTime, 
                  bool isDayTime, bool isNightStalkerNight);
    
    void updatePlayer(const std::string& name, const std::string& steamId, 
                     int32_t team, int32_t gold, int32_t goldReliable, 
                     int32_t goldUnreliable);
    
    void updateHero(const std::string& name, int32_t id, int32_t level, 
                   bool alive, int32_t respawnSeconds, float healthPercent, 
                   float manaPercent, bool hasBuyback);
    
    void updateAbilities(const std::vector<std::shared_ptr<Ability>>& abilities);
    void updateItems(const std::vector<std::shared_ptr<Item>>& items);

    // Getter methods
    std::string getProviderName() const;
    std::string getMapName() const;
    std::string getMatchId() const;
    GamePhase getGamePhase() const;
    int32_t getGameTime() const;
    int32_t getClockTime() const;
    bool isDayTime() const;
    
    std::string getPlayerName() const;
    std::string getPlayerSteamId() const;
    int32_t getPlayerTeam() const;
    int32_t getPlayerGold() const;
    
    std::string getHeroName() const;
    int32_t getHeroId() const;
    int32_t getHeroLevel() const;
    bool isHeroAlive() const;
    float getHeroHealthPercent() const;
    float getHeroManaPercent() const;
    
    // Validation
    bool isValid() const;

private:
    // Provider information
    struct Provider {
        std::string name;
        std::string appid;
        std::string version;
        std::string timestamp;
    };

    // Map information
    struct Map {
        std::string name;
        std::string matchid;
        GamePhase gamePhase;
        int32_t gameTime;
        int32_t clockTime;
        bool isDayTime;
        bool isNightStalkerNight;
    };

    // Player information
    struct Player {
        std::string name;
        std::string steamid;
        int32_t team;
        int32_t gold;
        int32_t gold_reliable;
        int32_t gold_unreliable;
    };

    // Hero information
    struct Hero {
        std::string name;
        int32_t id;
        int32_t level;
        bool alive;
        int32_t respawnSeconds;
        float healthPercent;
        float manaPercent;
        bool hasBuyback;
    };

    Provider provider_;
    Map map_;
    Player player_;
    Hero hero_;
    std::vector<std::shared_ptr<Ability>> abilities_;
    std::vector<std::shared_ptr<Item>> items_;
    
    mutable std::mutex mutex_;
};

class Ability {
public:
    Ability(int32_t id, const std::string& name, int32_t level, bool canCast, 
           bool isPassive, bool isUltimate, float cooldown, bool isHidden);

    int32_t getId() const;
    std::string getName() const;
    int32_t getLevel() const;
    bool canCast() const;
    bool isPassive() const;
    bool isUltimate() const;
    float getCooldown() const;
    bool isHidden() const;

private:
    int32_t id_;
    std::string name_;
    int32_t level_;
    bool canCast_;
    bool isPassive_;
    bool isUltimate_;
    float cooldown_;
    bool isHidden_;
};

class Item {
public:
    Item(int32_t id, const std::string& name, int32_t charges, 
        bool isPurchasable, float cooldown, bool isPassive);

    int32_t getId() const;
    std::string getName() const;
    int32_t getCharges() const;
    bool isPurchasable() const;
    float getCooldown() const;
    bool isPassive() const;

private:
    int32_t id_;
    std::string name_;
    int32_t charges_;
    bool isPurchasable_;
    float cooldown_;
    bool isPassive_;
};

} // namespace dota2
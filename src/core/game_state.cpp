#include "game_state.h"

#include <iostream>

namespace dota2 {

// GameState implementation
GameState::GameState() {
    reset();
}

GameState::~GameState() {
    // Clean up if needed
}

void GameState::reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Reset provider info
    provider_.name = "";
    provider_.appid = "";
    provider_.version = "";
    provider_.timestamp = "";
    
    // Reset map info
    map_.name = "";
    map_.matchid = "";
    map_.gamePhase = GamePhase::UNDEFINED;
    map_.gameTime = 0;
    map_.clockTime = 0;
    map_.isDayTime = true;
    map_.isNightStalkerNight = false;
    
    // Reset player info
    player_.name = "";
    player_.steamid = "";
    player_.team = 0;
    player_.gold = 0;
    player_.gold_reliable = 0;
    player_.gold_unreliable = 0;
    
    // Reset hero info
    hero_.name = "";
    hero_.id = 0;
    hero_.level = 0;
    hero_.alive = false;
    hero_.respawnSeconds = 0;
    hero_.healthPercent = 0.0f;
    hero_.manaPercent = 0.0f;
    hero_.hasBuyback = false;
    
    // Clear abilities and items
    abilities_.clear();
    items_.clear();
}

void GameState::updateProvider(const std::string& name, const std::string& appid, 
                               const std::string& version, const std::string& timestamp) {
    std::lock_guard<std::mutex> lock(mutex_);
    provider_.name = name;
    provider_.appid = appid;
    provider_.version = version;
    provider_.timestamp = timestamp;
}

void GameState::updateMap(const std::string& name, const std::string& matchid, 
                          GamePhase gamePhase, int32_t gameTime, int32_t clockTime, 
                          bool isDayTime, bool isNightStalkerNight) {
    std::lock_guard<std::mutex> lock(mutex_);
    map_.name = name;
    map_.matchid = matchid;
    map_.gamePhase = gamePhase;
    map_.gameTime = gameTime;
    map_.clockTime = clockTime;
    map_.isDayTime = isDayTime;
    map_.isNightStalkerNight = isNightStalkerNight;
}

void GameState::updatePlayer(const std::string& name, const std::string& steamId, 
                             int32_t team, int32_t gold, int32_t goldReliable, 
                             int32_t goldUnreliable) {
    std::lock_guard<std::mutex> lock(mutex_);
    player_.name = name;
    player_.steamid = steamId;
    player_.team = team;
    player_.gold = gold;
    player_.gold_reliable = goldReliable;
    player_.gold_unreliable = goldUnreliable;
}

void GameState::updateHero(const std::string& name, int32_t id, int32_t level, 
                           bool alive, int32_t respawnSeconds, float healthPercent, 
                           float manaPercent, bool hasBuyback) {
    std::lock_guard<std::mutex> lock(mutex_);
    hero_.name = name;
    hero_.id = id;
    hero_.level = level;
    hero_.alive = alive;
    hero_.respawnSeconds = respawnSeconds;
    hero_.healthPercent = healthPercent;
    hero_.manaPercent = manaPercent;
    hero_.hasBuyback = hasBuyback;
}

void GameState::updateAbilities(const std::vector<std::shared_ptr<Ability>>& abilities) {
    std::lock_guard<std::mutex> lock(mutex_);
    abilities_ = abilities;
}

void GameState::updateItems(const std::vector<std::shared_ptr<Item>>& items) {
    std::lock_guard<std::mutex> lock(mutex_);
    items_ = items;
}

std::string GameState::getProviderName() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return provider_.name;
}

std::string GameState::getMapName() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.name;
}

std::string GameState::getMatchId() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.matchid;
}

GameState::GamePhase GameState::getGamePhase() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.gamePhase;
}

int32_t GameState::getGameTime() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.gameTime;
}

int32_t GameState::getClockTime() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.clockTime;
}

bool GameState::isDayTime() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.isDayTime;
}

std::string GameState::getPlayerName() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return player_.name;
}

std::string GameState::getPlayerSteamId() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return player_.steamid;
}

int32_t GameState::getPlayerTeam() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return player_.team;
}

int32_t GameState::getPlayerGold() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return player_.gold;
}

std::string GameState::getHeroName() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return hero_.name;
}

int32_t GameState::getHeroId() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return hero_.id;
}

int32_t GameState::getHeroLevel() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return hero_.level;
}

bool GameState::isHeroAlive() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return hero_.alive;
}

float GameState::getHeroHealthPercent() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return hero_.healthPercent;
}

float GameState::getHeroManaPercent() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return hero_.manaPercent;
}

bool GameState::isValid() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !provider_.name.empty() && !map_.matchid.empty() && !player_.steamid.empty() && !hero_.name.empty();
}

// Ability implementation
Ability::Ability(int32_t id, const std::string& name, int32_t level, bool canCast, 
                 bool isPassive, bool isUltimate, float cooldown, bool isHidden)
    : id_(id), name_(name), level_(level), canCast_(canCast), 
      isPassive_(isPassive), isUltimate_(isUltimate), cooldown_(cooldown), isHidden_(isHidden) {
}

int32_t Ability::getId() const {
    return id_;
}

std::string Ability::getName() const {
    return name_;
}

int32_t Ability::getLevel() const {
    return level_;
}

bool Ability::canCast() const {
    return canCast_;
}

bool Ability::isPassive() const {
    return isPassive_;
}

bool Ability::isUltimate() const {
    return isUltimate_;
}

float Ability::getCooldown() const {
    return cooldown_;
}

bool Ability::isHidden() const {
    return isHidden_;
}

// Item implementation
Item::Item(int32_t id, const std::string& name, int32_t charges, 
           bool isPurchasable, float cooldown, bool isPassive)
    : id_(id), name_(name), charges_(charges), isPurchasable_(isPurchasable), 
      cooldown_(cooldown), isPassive_(isPassive) {
}

int32_t Item::getId() const {
    return id_;
}

std::string Item::getName() const {
    return name_;
}

int32_t Item::getCharges() const {
    return charges_;
}

bool Item::isPurchasable() const {
    return isPurchasable_;
}

float Item::getCooldown() const {
    return cooldown_;
}

bool Item::isPassive() const {
    return isPassive_;
}

} // namespace dota2

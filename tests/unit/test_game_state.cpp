#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/core/game_state.h"

using namespace dota2;

class GameStateTest : public ::testing::Test {
protected:
    GameState gameState;
    
    void SetUp() override {
        // Reset the game state before each test
        gameState.reset();
    }
};

TEST_F(GameStateTest, InitialState) {
    // Test that the initial state is properly initialized
    EXPECT_EQ(gameState.getProviderName(), "");
    EXPECT_EQ(gameState.getMapName(), "");
    EXPECT_EQ(gameState.getMatchId(), "");
    EXPECT_EQ(gameState.getGamePhase(), GameState::GamePhase::UNDEFINED);
    EXPECT_EQ(gameState.getGameTime(), 0);
    EXPECT_EQ(gameState.getClockTime(), 0);
    EXPECT_TRUE(gameState.isDayTime());
    EXPECT_EQ(gameState.getPlayerName(), "");
    EXPECT_EQ(gameState.getPlayerSteamId(), "");
    EXPECT_EQ(gameState.getPlayerTeam(), 0);
    EXPECT_EQ(gameState.getPlayerGold(), 0);
    EXPECT_EQ(gameState.getHeroName(), "");
    EXPECT_EQ(gameState.getHeroId(), 0);
    EXPECT_EQ(gameState.getHeroLevel(), 0);
    EXPECT_FALSE(gameState.isHeroAlive());
    EXPECT_FLOAT_EQ(gameState.getHeroHealthPercent(), 0.0f);
    EXPECT_FLOAT_EQ(gameState.getHeroManaPercent(), 0.0f);
    EXPECT_FALSE(gameState.isValid());
}

TEST_F(GameStateTest, UpdateProvider) {
    // Update provider information
    gameState.updateProvider("Dota 2", "570", "1.0", "123456789");
    
    // Check that the provider information was updated correctly
    EXPECT_EQ(gameState.getProviderName(), "Dota 2");
}

TEST_F(GameStateTest, UpdateMap) {
    // Update map information
    gameState.updateMap(
        "dota", 
        "12345", 
        GameState::GamePhase::GAME_IN_PROGRESS, 
        600, 
        540, 
        true, 
        false
    );
    
    // Check that the map information was updated correctly
    EXPECT_EQ(gameState.getMapName(), "dota");
    EXPECT_EQ(gameState.getMatchId(), "12345");
    EXPECT_EQ(gameState.getGamePhase(), GameState::GamePhase::GAME_IN_PROGRESS);
    EXPECT_EQ(gameState.getGameTime(), 600);
    EXPECT_EQ(gameState.getClockTime(), 540);
    EXPECT_TRUE(gameState.isDayTime());
}

TEST_F(GameStateTest, UpdatePlayer) {
    // Update player information
    gameState.updatePlayer("TestPlayer", "76561198123456789", 2, 1500, 600, 900);
    
    // Check that the player information was updated correctly
    EXPECT_EQ(gameState.getPlayerName(), "TestPlayer");
    EXPECT_EQ(gameState.getPlayerSteamId(), "76561198123456789");
    EXPECT_EQ(gameState.getPlayerTeam(), 2);
    EXPECT_EQ(gameState.getPlayerGold(), 1500);
}

TEST_F(GameStateTest, UpdateHero) {
    // Update hero information
    gameState.updateHero("npc_dota_hero_crystal_maiden", 5, 10, true, 0, 85.5f, 70.3f, true);
    
    // Check that the hero information was updated correctly
    EXPECT_EQ(gameState.getHeroName(), "npc_dota_hero_crystal_maiden");
    EXPECT_EQ(gameState.getHeroId(), 5);
    EXPECT_EQ(gameState.getHeroLevel(), 10);
    EXPECT_TRUE(gameState.isHeroAlive());
    EXPECT_FLOAT_EQ(gameState.getHeroHealthPercent(), 85.5f);
    EXPECT_FLOAT_EQ(gameState.getHeroManaPercent(), 70.3f);
}

TEST_F(GameStateTest, ValidState) {
    // Initially the game state should not be valid
    EXPECT_FALSE(gameState.isValid());
    
    // Update all required information
    gameState.updateProvider("Dota 2", "570", "1.0", "123456789");
    gameState.updateMap("dota", "12345", GameState::GamePhase::GAME_IN_PROGRESS, 600, 540, true, false);
    gameState.updatePlayer("TestPlayer", "76561198123456789", 2, 1500, 600, 900);
    gameState.updateHero("npc_dota_hero_crystal_maiden", 5, 10, true, 0, 85.5f, 70.3f, true);
    
    // Now the game state should be valid
    EXPECT_TRUE(gameState.isValid());
    
    // Reset the game state
    gameState.reset();
    
    // After reset, the game state should not be valid again
    EXPECT_FALSE(gameState.isValid());
}

TEST_F(GameStateTest, AbilityClass) {
    // Create an ability
    Ability ability(10, "crystal_nova", 3, true, false, false, 0.0f, false);
    
    // Check the ability properties
    EXPECT_EQ(ability.getId(), 10);
    EXPECT_EQ(ability.getName(), "crystal_nova");
    EXPECT_EQ(ability.getLevel(), 3);
    EXPECT_TRUE(ability.canCast());
    EXPECT_FALSE(ability.isPassive());
    EXPECT_FALSE(ability.isUltimate());
    EXPECT_FLOAT_EQ(ability.getCooldown(), 0.0f);
    EXPECT_FALSE(ability.isHidden());
}

TEST_F(GameStateTest, ItemClass) {
    // Create an item
    Item item(46, "blink", 0, true, 15.0f, false);
    
    // Check the item properties
    EXPECT_EQ(item.getId(), 46);
    EXPECT_EQ(item.getName(), "blink");
    EXPECT_EQ(item.getCharges(), 0);
    EXPECT_TRUE(item.isPurchasable());
    EXPECT_FLOAT_EQ(item.getCooldown(), 15.0f);
    EXPECT_FALSE(item.isPassive());
}

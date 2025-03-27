#include <gtest/gtest.h>
#include "../../src/core/game_state.h"

namespace dota2 {
namespace testing {

class GameStateTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up code
    }

    void TearDown() override {
        // Tear down code
    }

    GameState gameState;
};

// Test GameState constructor and default values
TEST_F(GameStateTest, ConstructorDefaultValues) {
    // Verify default values
    EXPECT_FALSE(gameState.isValid());
    EXPECT_EQ("", gameState.getProviderName());
    EXPECT_EQ("", gameState.getMapName());
    EXPECT_EQ("", gameState.getMatchId());
    EXPECT_EQ(GameState::GamePhase::UNDEFINED, gameState.getGamePhase());
    EXPECT_EQ(0, gameState.getGameTime());
    EXPECT_EQ(0, gameState.getClockTime());
    EXPECT_TRUE(gameState.isDayTime());
    EXPECT_EQ("", gameState.getPlayerName());
    EXPECT_EQ("", gameState.getPlayerSteamId());
    EXPECT_EQ(0, gameState.getPlayerTeam());
    EXPECT_EQ(0, gameState.getPlayerGold());
    EXPECT_EQ("", gameState.getHeroName());
    EXPECT_EQ(0, gameState.getHeroId());
    EXPECT_EQ(0, gameState.getHeroLevel());
    EXPECT_FALSE(gameState.isHeroAlive());
    EXPECT_FLOAT_EQ(0.0f, gameState.getHeroHealthPercent());
    EXPECT_FLOAT_EQ(0.0f, gameState.getHeroManaPercent());
}

// Test updating provider information
TEST_F(GameStateTest, UpdateProvider) {
    gameState.updateProvider("Test Provider", "12345", "1.0", "2025-03-27T12:00:00Z");
    
    EXPECT_EQ("Test Provider", gameState.getProviderName());
}

// Test updating map information
TEST_F(GameStateTest, UpdateMap) {
    gameState.updateMap(
        "dota",
        "1234567890",
        GameState::GamePhase::GAME_IN_PROGRESS,
        600,  // 10 minutes
        600,  // 10 minutes
        true,  // daytime
        false  // not nightstalker night
    );
    
    EXPECT_EQ("dota", gameState.getMapName());
    EXPECT_EQ("1234567890", gameState.getMatchId());
    EXPECT_EQ(GameState::GamePhase::GAME_IN_PROGRESS, gameState.getGamePhase());
    EXPECT_EQ(600, gameState.getGameTime());
    EXPECT_EQ(600, gameState.getClockTime());
    EXPECT_TRUE(gameState.isDayTime());
}

// Test updating player information
TEST_F(GameStateTest, UpdatePlayer) {
    gameState.updatePlayer("TestPlayer", "12345678901234567", 2, 1000, 600, 400);
    
    EXPECT_EQ("TestPlayer", gameState.getPlayerName());
    EXPECT_EQ("12345678901234567", gameState.getPlayerSteamId());
    EXPECT_EQ(2, gameState.getPlayerTeam());
    EXPECT_EQ(1000, gameState.getPlayerGold());
}

// Test updating hero information
TEST_F(GameStateTest, UpdateHero) {
    gameState.updateHero("npc_dota_hero_juggernaut", 8, 6, true, 0, 75.5f, 60.0f, true);
    
    EXPECT_EQ("npc_dota_hero_juggernaut", gameState.getHeroName());
    EXPECT_EQ(8, gameState.getHeroId());
    EXPECT_EQ(6, gameState.getHeroLevel());
    EXPECT_TRUE(gameState.isHeroAlive());
    EXPECT_FLOAT_EQ(75.5f, gameState.getHeroHealthPercent());
    EXPECT_FLOAT_EQ(60.0f, gameState.getHeroManaPercent());
}

// Test reset method
TEST_F(GameStateTest, Reset) {
    // Update all values
    gameState.updateProvider("Test Provider", "12345", "1.0", "2025-03-27T12:00:00Z");
    gameState.updateMap("dota", "1234567890", GameState::GamePhase::GAME_IN_PROGRESS, 600, 600, true, false);
    gameState.updatePlayer("TestPlayer", "12345678901234567", 2, 1000, 600, 400);
    gameState.updateHero("npc_dota_hero_juggernaut", 8, 6, true, 0, 75.5f, 60.0f, true);
    
    // Reset the game state
    gameState.reset();
    
    // Verify all values are reset to defaults
    EXPECT_FALSE(gameState.isValid());
    EXPECT_EQ("", gameState.getProviderName());
    EXPECT_EQ("", gameState.getMapName());
    EXPECT_EQ("", gameState.getMatchId());
    EXPECT_EQ(GameState::GamePhase::UNDEFINED, gameState.getGamePhase());
    EXPECT_EQ(0, gameState.getGameTime());
    EXPECT_EQ(0, gameState.getClockTime());
    EXPECT_TRUE(gameState.isDayTime());
    EXPECT_EQ("", gameState.getPlayerName());
    EXPECT_EQ("", gameState.getPlayerSteamId());
    EXPECT_EQ(0, gameState.getPlayerTeam());
    EXPECT_EQ(0, gameState.getPlayerGold());
    EXPECT_EQ("", gameState.getHeroName());
    EXPECT_EQ(0, gameState.getHeroId());
    EXPECT_EQ(0, gameState.getHeroLevel());
    EXPECT_FALSE(gameState.isHeroAlive());
    EXPECT_FLOAT_EQ(0.0f, gameState.getHeroHealthPercent());
    EXPECT_FLOAT_EQ(0.0f, gameState.getHeroManaPercent());
}

// Test isValid method
TEST_F(GameStateTest, IsValid) {
    EXPECT_FALSE(gameState.isValid());
    
    gameState.updateProvider("Test Provider", "12345", "1.0", "2025-03-27T12:00:00Z");
    gameState.updateMap("dota", "1234567890", GameState::GamePhase::GAME_IN_PROGRESS, 600, 600, true, false);
    gameState.updatePlayer("TestPlayer", "12345678901234567", 2, 1000, 600, 400);
    gameState.updateHero("npc_dota_hero_juggernaut", 8, 6, true, 0, 75.5f, 60.0f, true);
    
    EXPECT_TRUE(gameState.isValid());
}

// Test Ability class
TEST(AbilityTest, BasicFunctionality) {
    Ability ability(1, "test_ability", 3, true, false, true, 5.5f, false);
    
    EXPECT_EQ(1, ability.getId());
    EXPECT_EQ("test_ability", ability.getName());
    EXPECT_EQ(3, ability.getLevel());
    EXPECT_TRUE(ability.canCast());
    EXPECT_FALSE(ability.isPassive());
    EXPECT_TRUE(ability.isUltimate());
    EXPECT_FLOAT_EQ(5.5f, ability.getCooldown());
    EXPECT_FALSE(ability.isHidden());
}

// Test Item class
TEST(ItemTest, BasicFunctionality) {
    Item item(123, "test_item", 2, true, 10.0f, false);
    
    EXPECT_EQ(123, item.getId());
    EXPECT_EQ("test_item", item.getName());
    EXPECT_EQ(2, item.getCharges());
    EXPECT_TRUE(item.isPurchasable());
    EXPECT_FLOAT_EQ(10.0f, item.getCooldown());
    EXPECT_FALSE(item.isPassive());
}

} // namespace testing
} // namespace dota2

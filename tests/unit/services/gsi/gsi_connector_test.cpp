#include <gtest/gtest.h>
#include "services/gsi/gsi_connector.h"
#include <thread>
#include <chrono>
#include <curl/curl.h>

using namespace dota2_assistant::services::gsi;

// Helper function to write data from CURL to a string
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* response = static_cast<std::string*>(userdata);
    response->append(ptr, size * nmemb);
    return size * nmemb;
}

// Helper function to send a POST request to the GSI connector using CURL
bool send_test_request(uint16_t port, const std::string& json_payload, std::string& response) {
    try {
        // Use CURL for more reliable HTTP requests in tests
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize CURL" << std::endl;
            return false;
        }
        
        std::string url = "http://127.0.0.1:" + std::to_string(port) + "/";
        std::cout << "Sending request to " << url << std::endl;
        
        // Set up the request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_payload.size());
        
        // Set up headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        // Set up response handling
        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        
        // Set timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        
        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        
        // Clean up
        curl_slist_free_all(headers);
        
        if (res != CURLE_OK) {
            std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }
        
        // Get the response code
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        std::cout << "Response code: " << response_code << std::endl;
        
        // Clean up
        curl_easy_cleanup(curl);
        
        // Set the response
        response = response_data;
        std::cout << "Response: " << response << std::endl;
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception in send_test_request: " << e.what() << std::endl;
        return false;
    }
}

class GSIConnectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize CURL
        curl_global_init(CURL_GLOBAL_ALL);
    }
    
    void TearDown() override {
        // Clean up CURL
        curl_global_cleanup();
    }
};

TEST_F(GSIConnectorTest, InitializationTest) {
    // Test initialization with default port
    GSIConnector connector(4001, false); // Use port 4001 to avoid conflicts
    EXPECT_TRUE(connector.start());
    EXPECT_TRUE(connector.is_running());
    
    // Wait for the server to fully initialize (up to 5 seconds)
    std::cout << "Waiting for server to initialize..." << std::endl;
    for (int i = 0; i < 50 && !connector.is_server_ready(); i++) {
        std::cout << "Waiting for server to be ready (attempt " << (i+1) << ")..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    EXPECT_TRUE(connector.is_server_ready()) << "Server failed to become ready";
    
    connector.stop();
    EXPECT_FALSE(connector.is_running());
    EXPECT_FALSE(connector.is_server_ready());
}

TEST_F(GSIConnectorTest, JsonParsingTest) {
    // Start the connector on a test port
    uint16_t test_port = 4002;
    GSIConnector connector(test_port, false);
    
    // Start the connector and verify it's running
    ASSERT_TRUE(connector.start()) << "Failed to start GSI connector";
    ASSERT_TRUE(connector.is_running()) << "GSI connector is not running after start";
    
    // Wait for the server to fully initialize (up to 5 seconds)
    std::cout << "Waiting for server to initialize..." << std::endl;
    for (int i = 0; i < 50 && !connector.is_server_ready(); i++) {
        std::cout << "Waiting for server to be ready (attempt " << (i+1) << ")..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    ASSERT_TRUE(connector.is_server_ready()) << "Server failed to become ready";
    
    // Test valid JSON payload
    std::string valid_json = R"({
        "provider": {
            "name": "Dota 2",
            "appid": 570,
            "version": 1
        },
        "map": {
            "name": "dota",
            "matchid": "12345",
            "game_time": 120,
            "clock_time": 125,
            "daytime": true,
            "nightstalker_night": false,
            "game_state": "DOTA_GAMERULES_STATE_GAME_IN_PROGRESS"
        },
        "player": {
            "steamid": "76561198123456789",
            "name": "TestPlayer",
            "activity": "playing"
        },
        "hero": {
            "name": "npc_dota_hero_axe",
            "level": 5,
            "health": 800,
            "max_health": 1000,
            "mana": 200,
            "max_mana": 300
        }
    })";
    
    // Register a callback to verify the parsed JSON
    bool callback_called = false;
    nlohmann::json received_json;
    
    size_t callback_id = connector.register_callback(
        [&callback_called, &received_json](const nlohmann::json& json) {
            std::cout << "Callback received JSON: " << json.dump(2) << std::endl;
            callback_called = true;
            received_json = json;
        }
    );
    
    // Verify the callback was registered
    ASSERT_GT(callback_id, 0) << "Failed to register callback";
    
    // Send the test request
    std::string response;
    std::cout << "Sending valid JSON test request..." << std::endl;
    ASSERT_TRUE(send_test_request(test_port, valid_json, response)) 
        << "Failed to send test request with valid JSON";
    
    // Wait for the callback to be called with increasing timeouts
    for (int i = 0; i < 20 && !callback_called; i++) {
        std::cout << "Waiting for callback to be called (attempt " << (i+1) << ")..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    // Verify the callback was called and the JSON was parsed correctly
    EXPECT_TRUE(callback_called) << "Callback was not called for valid JSON";
    
    if (callback_called) {
        EXPECT_EQ(received_json["hero"]["name"], "npc_dota_hero_axe") 
            << "Incorrect hero name in parsed JSON";
        EXPECT_EQ(received_json["map"]["game_time"], 120) 
            << "Incorrect game time in parsed JSON";
    }
    
    // Test invalid JSON payload
    std::string invalid_json = "{ invalid json }";
    callback_called = false;
    
    std::cout << "Sending invalid JSON test request..." << std::endl;
    ASSERT_TRUE(send_test_request(test_port, invalid_json, response))
        << "Failed to send test request with invalid JSON";
    
    // Wait for a potential callback
    std::cout << "Waiting after invalid JSON request..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Verify the callback was not called for invalid JSON
    EXPECT_FALSE(callback_called) << "Callback was incorrectly called for invalid JSON";
    
    // Stop the connector
    std::cout << "Stopping GSI connector..." << std::endl;
    connector.stop();
    
    // Verify it's stopped
    EXPECT_FALSE(connector.is_running()) << "GSI connector is still running after stop";
    
    std::cout << "JsonParsingTest completed successfully" << std::endl;
}

TEST_F(GSIConnectorTest, CallbackRegistrationTest) {
    GSIConnector connector(4003, false);
    ASSERT_TRUE(connector.start());
    
    // Wait for the server to fully initialize (up to 5 seconds)
    std::cout << "Waiting for server to initialize..." << std::endl;
    for (int i = 0; i < 50 && !connector.is_server_ready(); i++) {
        std::cout << "Waiting for server to be ready (attempt " << (i+1) << ")..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    ASSERT_TRUE(connector.is_server_ready()) << "Server failed to become ready";
    
    // Register multiple callbacks
    int callback1_count = 0;
    int callback2_count = 0;
    
    size_t id1 = connector.register_callback([&callback1_count](const nlohmann::json&) {
        callback1_count++;
    });
    
    size_t id2 = connector.register_callback([&callback2_count](const nlohmann::json&) {
        callback2_count++;
    });
    
    // Send a test request
    std::string test_json = R"({"test": "data"})";
    std::string response;
    ASSERT_TRUE(send_test_request(4003, test_json, response));
    
    // Wait for callbacks to be called
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Verify both callbacks were called
    EXPECT_EQ(callback1_count, 1);
    EXPECT_EQ(callback2_count, 1);
    
    // Unregister the first callback
    EXPECT_TRUE(connector.unregister_callback(id1));
    
    // Send another test request
    ASSERT_TRUE(send_test_request(4003, test_json, response));
    
    // Wait for callbacks to be called
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Verify only the second callback was called
    EXPECT_EQ(callback1_count, 1); // Still 1
    EXPECT_EQ(callback2_count, 2); // Now 2
    
    // Stop the connector
    connector.stop();
}

// This test is marked as DISABLED because it requires Dota 2 to be installed
// It can be enabled manually when testing on a system with Dota 2
TEST_F(GSIConnectorTest, DISABLED_ConfigDeploymentTest) {
    GSIConnector connector(4004, true);
    
    // Test config deployment
    EXPECT_TRUE(connector.deploy_config());
    
    // Start the connector
    EXPECT_TRUE(connector.start());
    
    // Stop the connector
    connector.stop();
}

// Performance test
TEST_F(GSIConnectorTest, PerformanceTest) {
    GSIConnector connector(4005, false);
    ASSERT_TRUE(connector.start());
    
    // Wait for the server to fully initialize (up to 5 seconds)
    std::cout << "Waiting for server to initialize..." << std::endl;
    for (int i = 0; i < 50 && !connector.is_server_ready(); i++) {
        std::cout << "Waiting for server to be ready (attempt " << (i+1) << ")..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    ASSERT_TRUE(connector.is_server_ready()) << "Server failed to become ready";
    
    // Generate a large JSON payload
    nlohmann::json large_json;
    large_json["provider"]["name"] = "Dota 2";
    large_json["provider"]["appid"] = 570;
    
    // Add a large array to simulate a complex game state
    large_json["entities"] = nlohmann::json::array();
    for (int i = 0; i < 1000; i++) {
        nlohmann::json entity;
        entity["id"] = i;
        entity["name"] = "entity_" + std::to_string(i);
        entity["position"]["x"] = i * 10;
        entity["position"]["y"] = i * 20;
        entity["position"]["z"] = i * 30;
        large_json["entities"].push_back(entity);
    }
    
    // Measure the time it takes to process the large JSON
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Send the large JSON
    std::string response;
    ASSERT_TRUE(send_test_request(4005, large_json.dump(), response));
    
    // Wait for processing to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    std::cout << "Large JSON processing time: " << duration << "ms" << std::endl;
    
    // The test passes if it completes without crashing
    // We don't assert on the duration since it will vary by system
    
    // Stop the connector
    connector.stop();
}
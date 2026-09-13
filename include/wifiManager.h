#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "sensor.h"
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// Data Structures for WiFi & MQTT (Aspect 4)
// ============================================================================

typedef struct {
    bool isConnected;
    char ssid[33];
    char ipAddress[16];
    char macAddress[18];
    uint8_t signalStrength;  // RSSI (-120 to 0 dBm)
    uint32_t connectedDuration;
    uint32_t connectionAttempts;
} WiFiConnectionInfo;

typedef struct {
    bool isConnected;
    char brokerAddress[64];
    uint16_t brokerPort;
    uint32_t connectedDuration;
    uint32_t messagesPublished;
    uint32_t messagesReceived;
    uint32_t lastMessageTime;
} MQTTConnectionInfo;

typedef enum {
    WIFI_STATE_DISCONNECTED = 0,
    WIFI_STATE_CONNECTING = 1,
    WIFI_STATE_CONNECTED = 2,
    WIFI_STATE_RECONNECTING = 3,
    WIFI_STATE_ERROR = 4
} WiFiState;

typedef enum {
    MQTT_STATE_DISCONNECTED = 0,
    MQTT_STATE_CONNECTING = 1,
    MQTT_STATE_CONNECTED = 2,
    MQTT_STATE_SUBSCRIBING = 3,
    MQTT_STATE_ERROR = 4
} MQTTState;

// ============================================================================
// WiFi Initialization & Control (Aspect 4)
// ============================================================================

/**
 * Initialize WiFi subsystem
 * @return true if successful
 */
bool initWiFi();

/**
 * Deinitialize WiFi
 * @return true if successful
 */
bool deinitWiFi();

/**
 * Connect to WiFi network
 * @param ssid Network SSID
 * @param password Network password
 * @param timeoutMs Connection timeout in milliseconds
 * @return true if successful
 */
bool connectWiFi(const char* ssid, const char* password, uint32_t timeoutMs);

/**
 * Disconnect from WiFi
 * @return true if successful
 */
bool disconnectWiFi();

/**
 * Is WiFi connected
 * @return true if connected
 */
bool isWiFiConnected();

/**
 * Get WiFi connection state
 * @return Current WiFiState
 */
WiFiState getWiFiState();

/**
 * Get WiFi state as string
 * @return State description
 */
const char* getWiFiStateString();

/**
 * Reconnect to WiFi if disconnected
 * @return true if reconnection initiated
 */
bool reconnectWiFi();

/**
 * Scan available WiFi networks
 * @return Number of networks found
 */
uint16_t scanWiFiNetworks();

/**
 * Get SSID of network at index
 * @param index Network index
 * @param ssid Buffer to store SSID
 * @param maxLen Max buffer length
 * @return true if successful
 */
bool getWiFiNetworkSSID(uint16_t index, char* ssid, uint16_t maxLen);

/**
 * Get signal strength of network at index
 * @param index Network index
 * @return RSSI value in dBm
 */
int8_t getWiFiNetworkRSSI(uint16_t index);

// ============================================================================
// WiFi Configuration (Aspect 4)
// ============================================================================

/**
 * Set WiFi credentials
 * @param ssid Network SSID
 * @param password Network password
 * @return true if successful
 */
bool setWiFiCredentials(const char* ssid, const char* password);

/**
 * Get current WiFi SSID
 * @return SSID string
 */
const char* getWiFiSSID();

/**
 * Set WiFi TX power
 * @param power Power in dBm (8-20)
 * @return true if successful
 */
bool setWiFiTxPower(uint8_t power);

/**
 * Set WiFi hostname
 * @param hostname Hostname string
 * @return true if successful
 */
bool setWiFiHostname(const char* hostname);

/**
 * Enable WiFi auto-reconnect
 * @return true if successful
 */
bool enableWiFiAutoReconnect();

/**
 * Disable WiFi auto-reconnect
 * @return true if successful
 */
bool disableWiFiAutoReconnect();

/**
 * Set WiFi reconnect interval
 * @param intervalMs Interval in milliseconds
 * @return true if successful
 */
bool setWiFiReconnectInterval(uint32_t intervalMs);

// ============================================================================
// WiFi Connection Info (Aspect 4)
// ============================================================================

/**
 * Get WiFi connection information
 * @return WiFiConnectionInfo structure
 */
WiFiConnectionInfo getWiFiConnectionInfo();

/**
 * Get WiFi signal strength (RSSI)
 * @return RSSI value in dBm (-120 to 0)
 */
int8_t getWiFiSignalStrength();

/**
 * Get WiFi IP address
 * @param ipBuffer Buffer to store IP address
 * @param bufferLen Buffer length
 * @return true if successful
 */
bool getWiFiIPAddress(char* ipBuffer, uint16_t bufferLen);

/**
 * Get WiFi MAC address
 * @param macBuffer Buffer to store MAC address (18 bytes)
 * @return true if successful
 */
bool getWiFiMACAddress(char* macBuffer);

/**
 * Get WiFi gateway IP
 * @param gatewayBuffer Buffer to store gateway IP
 * @param bufferLen Buffer length
 * @return true if successful
 */
bool getWiFiGateway(char* gatewayBuffer, uint16_t bufferLen);

/**
 * Get WiFi subnet mask
 * @param maskBuffer Buffer to store subnet mask
 * @param bufferLen Buffer length
 * @return true if successful
 */
bool getWiFiSubnetMask(char* maskBuffer, uint16_t bufferLen);

/**
 * Get WiFi connection duration
 * @return Duration in seconds
 */
uint32_t getWiFiConnectedDuration();

// ============================================================================
// MQTT Initialization & Control (Aspect 4)
// ============================================================================

/**
 * Initialize MQTT client
 * @param brokerAddress MQTT broker address
 * @param brokerPort MQTT broker port
 * @param username MQTT username
 * @param password MQTT password
 * @return true if successful
 */
bool initMQTT(const char* brokerAddress, uint16_t brokerPort, 
              const char* username, const char* password);

/**
 * Deinitialize MQTT
 * @return true if successful
 */
bool deinitMQTT();

/**
 * Connect to MQTT broker
 * @param clientId MQTT client ID
 * @return true if successful
 */
bool connectMQTT(const char* clientId);

/**
 * Disconnect from MQTT broker
 * @return true if successful
 */
bool disconnectMQTT();

/**
 * Is MQTT connected
 * @return true if connected
 */
bool isMQTTConnected();

/**
 * Get MQTT connection state
 * @return Current MQTTState
 */
MQTTState getMQTTState();

/**
 * Get MQTT state as string
 * @return State description
 */
const char* getMQTTStateString();

// ============================================================================
// MQTT Publishing (Aspect 4)
// ============================================================================

/**
 * Publish heart rate data to MQTT
 * @param data Heart rate reading
 * @return true if successful
 */
bool publishHeartRateToMQTT(const HeartRateData* data);

/**
 * Publish bulk readings to MQTT
 * @param dataArray Array of readings
 * @param count Number of readings
 * @return Number of readings published
 */
uint16_t publishBulkReadingsToMQTT(const HeartRateData* dataArray, uint16_t count);

/**
 * Publish alert to MQTT
 * @param alertLevel Alert level
 * @param message Alert message
 * @return true if successful
 */
bool publishAlertToMQTT(uint8_t alertLevel, const char* message);

/**
 * Publish baseline data to MQTT
 * @param baselineJSON Baseline as JSON string
 * @return true if successful
 */
bool publishBaselineToMQTT(const char* baselineJSON);

/**
 * Publish custom message to topic
 * @param topic MQTT topic
 * @param message Message payload
 * @param retain Message retention flag
 * @return true if successful
 */
bool publishMQTT(const char* topic, const char* message, bool retain);

/**
 * Publish binary data to topic
 * @param topic MQTT topic
 * @param data Binary data
 * @param dataLen Data length
 * @return true if successful
 */
bool publishMQTTBinary(const char* topic, const uint8_t* data, uint16_t dataLen);

/**
 * Queue message for publishing
 * @param topic MQTT topic
 * @param message Message payload
 * @return Queue position (0xFFFF if queue full)
 */
uint16_t queueMQTTMessage(const char* topic, const char* message);

/**
 * Process MQTT publish queue
 * @return Number of messages published
 */
uint16_t processMQTTQueue();

// ============================================================================
// MQTT Subscription (Aspect 4)
// ============================================================================

/**
 * Subscribe to MQTT topic
 * @param topic Topic to subscribe to
 * @return true if successful
 */
bool subscribeMQTT(const char* topic);

/**
 * Unsubscribe from MQTT topic
 * @param topic Topic to unsubscribe from
 * @return true if successful
 */
bool unsubscribeMQTT(const char* topic);

/**
 * Register callback for MQTT message
 * @param callback Function pointer for message handling
 * @return true if successful
 */
typedef void (*MQTTMessageCallback)(const char* topic, const uint8_t* payload, uint16_t length);
bool registerMQTTCallback(MQTTMessageCallback callback);

/**
 * Get last received MQTT message
 * @param topic Buffer to store topic
 * @param topicLen Topic buffer length
 * @param payload Buffer to store payload
 * @param payloadLen Payload buffer length
 * @return true if message available
 */
bool getLastMQTTMessage(char* topic, uint16_t topicLen, 
                        uint8_t* payload, uint16_t* payloadLen);

// ============================================================================
// Cloud Synchronization (Aspect 4)
// ============================================================================

/**
 * Initialize cloud sync
 * @param endpoint Cloud API endpoint
 * @param authToken Authentication token
 * @return true if successful
 */
bool initCloudSync(const char* endpoint, const char* authToken);

/**
 * Sync readings to cloud
 * @param dataArray Array of readings
 * @param count Number of readings
 * @return Number of readings synced
 */
uint16_t syncReadingsToCloud(const HeartRateData* dataArray, uint16_t count);

/**
 * Sync baseline to cloud
 * @param baselineJSON Baseline as JSON
 * @return true if successful
 */
bool syncBaselineToCloud(const char* baselineJSON);

/**
 * Sync alerts to cloud
 * @param alertsJSON Alerts as JSON array
 * @return true if successful
 */
bool syncAlertsToCloud(const char* alertsJSON);

/**
 * Download baseline from cloud
 * @param baselineBuffer Buffer to store baseline JSON
 * @param bufferSize Buffer size
 * @return Number of bytes written
 */
uint16_t downloadBaselineFromCloud(char* baselineBuffer, uint16_t bufferSize);

/**
 * Get cloud sync status
 * @return 0=idle, 1=syncing, 2=complete, 3=failed
 */
uint8_t getCloudSyncStatus();

/**
 * Is cloud sync enabled
 * @return true if enabled
 */
bool isCloudSyncEnabled();

/**
 * Enable cloud sync
 * @return true if successful
 */
bool enableCloudSync();

/**
 * Disable cloud sync
 * @return true if successful
 */
bool disableCloudSync();

/**
 * Set cloud sync interval
 * @param intervalMs Interval in milliseconds
 * @return true if successful
 */
bool setCloudSyncInterval(uint32_t intervalMs);

// ============================================================================
// Network Utilities (Aspect 4)
// ============================================================================

/**
 * Perform DNS lookup
 * @param hostname Hostname to resolve
 * @param ipBuffer Buffer to store IP address
 * @param bufferLen Buffer length
 * @return true if successful
 */
bool dnsLookup(const char* hostname, char* ipBuffer, uint16_t bufferLen);

/**
 * Check internet connectivity (ping)
 * @param hostname Hostname to ping
 * @return true if reachable
 */
bool checkInternetConnectivity(const char* hostname);

/**
 * Get network latency (ping time)
 * @param hostname Hostname to ping
 * @return Latency in milliseconds (0xFFFFFFFF if failed)
 */
uint32_t getNetworkLatency(const char* hostname);

/**
 * Get WiFi statistics
 * @param packetsReceived Pointer to store packets received
 * @param packetsSent Pointer to store packets sent
 * @param bytesReceived Pointer to store bytes received
 * @param bytesSent Pointer to store bytes sent
 * @return true if successful
 */
bool getWiFiStatistics(uint32_t* packetsReceived, uint32_t* packetsSent,
                       uint32_t* bytesReceived, uint32_t* bytesSent);

/**
 * Get MQTT statistics
 * @param messagesPublished Pointer to store published count
 * @param messagesReceived Pointer to store received count
 * @return true if successful
 */
bool getMQTTStatistics(uint32_t* messagesPublished, uint32_t* messagesReceived);

/**
 * Reset network statistics
 * @return true if successful
 */
bool resetNetworkStatistics();

// ============================================================================
// Power Management (Aspect 4 & 5)
// ============================================================================

/**
 * Enable low power WiFi mode
 * @return true if successful
 */
bool enableWiFiLowPowerMode();

/**
 * Disable low power WiFi mode
 * @return true if successful
 */
bool disableWiFiLowPowerMode();

/**
 * Is low power WiFi mode active
 * @return true if active
 */
bool isWiFiLowPowerModeActive();

/**
 * Estimate WiFi power consumption
 * @return Power in mA
 */
uint16_t estimateWiFiPowerConsumption();

/**
 * Estimate MQTT power consumption
 * @return Power in mA
 */
uint16_t estimateMQTTPowerConsumption();

#endif // WIFI_MANAGER_H

#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include "sensor.h"
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// Data Structures for BLE (Aspect 4)
// ============================================================================

typedef struct {
    uint16_t heartRate;
    uint16_t spO2;
    uint32_t timestamp;
    uint8_t quality;
} BLEHeartRateData;

typedef struct {
    bool isConnected;
    uint8_t connectionCount;
    uint32_t lastConnectionTime;
    uint32_t connectedDuration;
    uint8_t signalStrength;  // RSSI
    char clientAddress[18];  // MAC address
} BLEConnectionInfo;

typedef struct {
    bool isAdvertising;
    uint32_t advertisingStartTime;
    uint16_t advertiseInterval;
    uint8_t advertisingData[31];
    uint8_t scanResponseData[31];
} BLEAdvertisingInfo;

typedef enum {
    BLE_STATE_IDLE = 0,
    BLE_STATE_INITIALIZING = 1,
    BLE_STATE_ADVERTISING = 2,
    BLE_STATE_CONNECTING = 3,
    BLE_STATE_CONNECTED = 4,
    BLE_STATE_DISCONNECTING = 5,
    BLE_STATE_ERROR = 6
} BLEState;

// ============================================================================
// Initialization & Control (Aspect 4)
// ============================================================================

/**
 * Initialize BLE subsystem
 * @return true if successful
 */
bool initBLE();

/**
 * Deinitialize BLE
 * @return true if successful
 */
bool deinitBLE();

/**
 * Start BLE advertising
 * @return true if successful
 */
bool startBLEAdvertising();

/**
 * Stop BLE advertising
 * @return true if successful
 */
bool stopBLEAdvertising();

/**
 * Get current BLE state
 * @return Current BLEState
 */
BLEState getBLEState();

/**
 * Get BLE state as string
 * @return State description
 */
const char* getBLEStateString();

/**
 * Is BLE connected
 * @return true if connected to client
 */
bool isBLEConnected();

/**
 * Disconnect BLE client
 * @return true if successful
 */
bool disconnectBLE();

// ============================================================================
// Configuration (Aspect 4)
// ============================================================================

/**
 * Set device name
 * @param name Device name (max 31 chars)
 * @return true if successful
 */
bool setBLEDeviceName(const char* name);

/**
 * Get device name
 * @return Device name string
 */
const char* getBLEDeviceName();

/**
 * Set advertising interval
 * @param intervalMs Interval in milliseconds
 * @return true if successful
 */
bool setBLEAdvertisingInterval(uint16_t intervalMs);

/**
 * Set MTU (Maximum Transmission Unit) size
 * @param mtuSize MTU size (20-517)
 * @return true if successful
 */
bool setBLEMTU(uint16_t mtuSize);

/**
 * Set TX power level
 * @param power Power level (-12 to +9 dBm)
 * @return true if successful
 */
bool setBLETxPower(int8_t power);

/**
 * Enable/disable BLE security
 * @param enable true to enable security
 * @return true if successful
 */
bool setBLESecurity(bool enable);

/**
 * Set pairing mode
 * @param mode 0=none, 1=just works, 2=pin code
 * @return true if successful
 */
bool setBLEPairingMode(uint8_t mode);

// ============================================================================
// Data Transmission (Aspect 4)
// ============================================================================

/**
 * Send heart rate data via BLE
 * @param data Heart rate data
 * @return true if sent successfully
 */
bool sendHeartRateData(const HeartRateData* data);

/**
 * Send bulk heart rate data
 * @param dataArray Array of readings
 * @param count Number of readings
 * @return Number of readings sent
 */
uint16_t sendBulkHeartRateData(const HeartRateData* dataArray, uint16_t count);

/**
 * Send alert notification
 * @param alertLevel Alert level
 * @param message Alert message
 * @return true if sent successfully
 */
bool sendAlertNotification(uint8_t alertLevel, const char* message);

/**
 * Send baseline data via BLE
 * @param baselineJSON Baseline as JSON string
 * @return true if sent successfully
 */
bool sendBaselineData(const char* baselineJSON);

/**
 * Send command response
 * @param commandId Command ID
 * @param response Response data
 * @param responseLen Response length
 * @return true if sent successfully
 */
bool sendCommandResponse(uint8_t commandId, const uint8_t* response, uint16_t responseLen);

/**
 * Queue data for transmission (for buffering)
 * @param data Data to queue
 * @param dataLen Data length
 * @return Queue position (0xFFFF if queue full)
 */
uint16_t queueBLEData(const uint8_t* data, uint16_t dataLen);

/**
 * Process BLE transmission queue
 * @return Number of items transmitted
 */
uint16_t processBLEQueue();

/**
 * Get BLE transmission queue status
 * @param queued Pointer to store items queued
 * @param capacity Pointer to store queue capacity
 * @return true if queue info available
 */
bool getBLEQueueStatus(uint16_t* queued, uint16_t* capacity);

// ============================================================================
// Command Handling (Aspect 4)
// ============================================================================

/**
 * Register callback for BLE commands
 * @param callback Function pointer for command handling
 * @return true if successful
 */
typedef void (*BLECommandCallback)(uint8_t cmdId, const uint8_t* data, uint16_t len);
bool registerBLECommandCallback(BLECommandCallback callback);

/**
 * Process incoming BLE command
 * @param cmdId Command ID
 * @param data Command data
 * @param dataLen Data length
 * @return true if command processed
 */
bool processBLECommand(uint8_t cmdId, const uint8_t* data, uint16_t dataLen);

/**
 * Get last received command
 * @param cmdId Pointer to store command ID
 * @param data Pointer to store data
 * @param dataLen Pointer to store length
 * @return true if command available
 */
bool getLastBLECommand(uint8_t* cmdId, uint8_t** data, uint16_t* dataLen);

// ============================================================================
// Connection Management (Aspect 4)
// ============================================================================

/**
 * Get connection info
 * @return BLEConnectionInfo structure
 */
BLEConnectionInfo getBLEConnectionInfo();

/**
 * Get RSSI (signal strength) of connected client
 * @return RSSI value in dBm (typically -100 to -20)
 */
int8_t getBLERSSI();

/**
 * Get number of connected clients
 * @return Number of clients
 */
uint8_t getBLEConnectedClientCount();

/**
 * Get client address
 * @param index Client index (0 for first)
 * @param address Buffer to store MAC address (18 bytes)
 * @return true if address retrieved
 */
bool getBLEClientAddress(uint8_t index, char* address);

/**
 * Get connection duration (seconds)
 * @return Duration in seconds
 */
uint32_t getBLEConnectionDuration();

/**
 * Update connection interval for power optimization
 * @param minInterval Min interval (1.25ms units)
 * @param maxInterval Max interval (1.25ms units)
 * @return true if successful
 */
bool updateBLEConnectionInterval(uint16_t minInterval, uint16_t maxInterval);

// ============================================================================
// Notifications & Indications (Aspect 4)
// ============================================================================

/**
 * Enable heart rate notifications
 * @return true if successful
 */
bool enableHeartRateNotifications();

/**
 * Disable heart rate notifications
 * @return true if successful
 */
bool disableHeartRateNotifications();

/**
 * Are heart rate notifications enabled
 * @return true if enabled
 */
bool areHeartRateNotificationsEnabled();

/**
 * Send heart rate notification
 * @param heartRate Heart rate value
 * @return true if sent successfully
 */
bool notifyHeartRate(uint16_t heartRate);

/**
 * Send SpO2 notification
 * @param spO2 SpO2 percentage
 * @return true if sent successfully
 */
bool notifySpO2(uint16_t spO2);

/**
 * Send alert indication
 * @param message Alert message
 * @return true if sent successfully
 */
bool indicateAlert(const char* message);

// ============================================================================
// Advertising (Aspect 4)
// ============================================================================

/**
 * Get advertising info
 * @return BLEAdvertisingInfo structure
 */
BLEAdvertisingInfo getBLEAdvertisingInfo();

/**
 * Set custom advertising data
 * @param data Advertising data
 * @param length Data length (max 31 bytes)
 * @return true if successful
 */
bool setCustomAdvertisingData(const uint8_t* data, uint8_t length);

/**
 * Set scan response data
 * @param data Scan response data
 * @param length Data length (max 31 bytes)
 * @return true if successful
 */
bool setScanResponseData(const uint8_t* data, uint8_t length);

/**
 * Add service UUID to advertisement
 * @param uuid Service UUID (128-bit)
 * @return true if successful
 */
bool addAdvertisedService(const char* uuid);

/**
 * Remove service UUID from advertisement
 * @param uuid Service UUID
 * @return true if successful
 */
bool removeAdvertisedService(const char* uuid);

// ============================================================================
// Statistics & Monitoring (Aspect 4)
// ============================================================================

/**
 * Get BLE statistics
 * @param connectedTime Pointer to store connected time (seconds)
 * @param dataTransferred Pointer to store bytes transferred
 * @param notificationsSent Pointer to store notifications sent
 * @return true if successful
 */
bool getBLEStatistics(uint32_t* connectedTime, uint32_t* dataTransferred, uint32_t* notificationsSent);

/**
 * Get number of connection attempts
 * @return Connection attempt count
 */
uint32_t getBLEConnectionAttempts();

/**
 * Reset BLE statistics
 * @return true if successful
 */
bool resetBLEStatistics();

/**
 * Get BLE error status
 * @return Error code (0 = no error)
 */
uint8_t getBLEErrorStatus();

/**
 * Get BLE status as JSON
 * @param buffer Output buffer
 * @param bufferSize Buffer size
 * @return Number of bytes written
 */
uint16_t getBLEStatusJSON(char* buffer, uint16_t bufferSize);

// ============================================================================
// Power Management (Aspect 4 & 5)
// ============================================================================

/**
 * Enable low power mode
 * @return true if successful
 */
bool enableBLELowPowerMode();

/**
 * Disable low power mode
 * @return true if successful
 */
bool disableBLELowPowerMode();

/**
 * Is low power mode active
 * @return true if active
 */
bool isBLELowPowerModeActive();

/**
 * Get estimated power consumption
 * @return Power in mA
 */
uint16_t estimateBLEPowerConsumption();

#endif // BLE_MANAGER_H

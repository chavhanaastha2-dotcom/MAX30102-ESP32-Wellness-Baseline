#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include "sensor.h"
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// ============================================================================
// Data Structures for Storage
// ============================================================================

typedef struct {
    uint32_t timestamp;
    uint16_t heartRate;
    uint16_t spO2;
    uint8_t quality;
} StoredReading;

typedef struct {
    uint16_t totalReadings;
    uint32_t firstReadingTime;
    uint32_t lastReadingTime;
    float avgHeartRate;
    float avgSpO2;
    uint16_t maxHeartRate;
    uint16_t minHeartRate;
    uint16_t maxSpO2;
    uint16_t minSpO2;
} StorageStats;

typedef struct {
    uint32_t timestamp;
    uint8_t alertLevel;  // 0=info, 1=warning, 2=critical
    char message[128];
} StoredAlert;

// ============================================================================
// EEPROM Functions (Aspect 1)
// ============================================================================

/**
 * Initialize EEPROM storage
 * @return true if successful
 */
bool initEEPROM();

/**
 * Store a single reading in EEPROM
 * @param data Reading data
 * @return true if successful
 */
bool storeReadingEEPROM(const HeartRateData* data);

/**
 * Read a stored reading from EEPROM
 * @param index Reading index
 * @param data Pointer to store data
 * @return true if successful
 */
bool readReadingEEPROM(uint16_t index, StoredReading* data);

/**
 * Get number of readings in EEPROM
 * @return Number of stored readings
 */
uint16_t getReadingCountEEPROM();

/**
 * Clear all EEPROM data
 * @return true if successful
 */
bool clearEEPROM();

/**
 * Get EEPROM usage statistics
 * @return StorageStats structure
 */
StorageStats getEEPROMStats();

// ============================================================================
// SD Card Functions (Aspect 1)
// ============================================================================

/**
 * Initialize SD card
 * @return true if successful
 */
bool initSDCard();

/**
 * Check if SD card is available
 * @return true if SD card present and readable
 */
bool isSDCardAvailable();

/**
 * Write reading to SD card (CSV format)
 * @param data Reading data
 * @return true if successful
 */
bool writeReadingToSD(const HeartRateData* data);

/**
 * Write multiple readings to SD card
 * @param dataArray Array of readings
 * @param count Number of readings
 * @return Number of successfully written readings
 */
uint16_t writeBulkReadingsToSD(const HeartRateData* dataArray, uint16_t count);

/**
 * Read all readings from SD card into memory
 * @param dataArray Array to store readings
 * @param maxCount Maximum readings to read
 * @return Number of readings read
 */
uint16_t readAllReadingsFromSD(HeartRateData* dataArray, uint16_t maxCount);

/**
 * Get SD card usage info
 * @param usedBytes Pointer to store used space
 * @param totalBytes Pointer to store total space
 * @return true if successful
 */
bool getSDCardSpace(uint32_t* usedBytes, uint32_t* totalBytes);

/**
 * List files on SD card
 * @return true if successful
 */
bool listSDCardFiles();

/**
 * Delete old files from SD card (older than X days)
 * @param olderThanDays Delete files older than this many days
 * @return Number of files deleted
 */
uint16_t deleteOldSDCardFiles(uint16_t olderThanDays);

// ============================================================================
// JSON Export/Import (Aspect 1)
// ============================================================================

/**
 * Export readings to JSON format on SD card
 * @param filename Output filename
 * @param dataArray Array of readings
 * @param count Number of readings
 * @return true if successful
 */
bool exportReadingsJSON(const char* filename, const HeartRateData* dataArray, uint16_t count);

/**
 * Import readings from JSON file on SD card
 * @param filename Input filename
 * @param dataArray Array to store readings
 * @param maxCount Maximum readings to import
 * @return Number of readings imported
 */
uint16_t importReadingsJSON(const char* filename, HeartRateData* dataArray, uint16_t maxCount);

/**
 * Export baseline data to JSON
 * @param filename Output filename
 * @return true if successful
 */
bool exportBaselineJSON(const char* filename);

/**
 * Import baseline data from JSON
 * @param filename Input filename
 * @return true if successful
 */
bool importBaselineJSON(const char* filename);

// ============================================================================
// Alert Logging (Aspect 1)
// ============================================================================

/**
 * Log an alert to storage
 * @param level Alert level (0=info, 1=warning, 2=critical)
 * @param message Alert message
 * @return true if successful
 */
bool logAlert(uint8_t level, const char* message);

/**
 * Get alert history
 * @param alertsArray Array to store alerts
 * @param maxCount Maximum alerts to retrieve
 * @return Number of alerts retrieved
 */
uint16_t getAlertHistory(StoredAlert* alertsArray, uint16_t maxCount);

/**
 * Clear alert history
 * @return true if successful
 */
bool clearAlertHistory();

/**
 * Get alert statistics
 * @param infoCount Pointer to store info count
 * @param warningCount Pointer to store warning count
 * @param criticalCount Pointer to store critical count
 */
void getAlertStats(uint16_t* infoCount, uint16_t* warningCount, uint16_t* criticalCount);

// ============================================================================
// Backup & Sync (Aspect 1)
// ============================================================================

/**
 * Create backup of all data
 * @param backupName Backup identifier
 * @return true if successful
 */
bool createDataBackup(const char* backupName);

/**
 * Restore data from backup
 * @param backupName Backup identifier
 * @return true if successful
 */
bool restoreFromBackup(const char* backupName);

/**
 * List available backups
 * @return Number of backups available
 */
uint16_t listBackups();

/**
 * Sync local data to cloud (async)
 * @param callback Callback function when sync completes
 * @return true if sync initiated
 */
bool syncDataToCloud(void (*callback)(bool success));

/**
 * Get sync status
 * @return 0=idle, 1=syncing, 2=complete, 3=failed
 */
uint8_t getSyncStatus();

// ============================================================================
// Data Cleanup & Maintenance
// ============================================================================

/**
 * Delete readings older than specified days
 * @param olderThanDays Number of days
 * @return Number of readings deleted
 */
uint16_t deleteOldReadings(uint16_t olderThanDays);

/**
 * Compress old data (aggregate into daily summaries)
 * @param olderThanDays Compress data older than this
 * @return true if successful
 */
bool compressOldData(uint16_t olderThanDays);

/**
 * Repair corrupted storage
 * @return true if successful
 */
bool repairStorage();

/**
 * Get storage integrity status
 * @return true if storage is healthy
 */
bool verifyStorageIntegrity();

#endif // DATA_STORAGE_H

// ============================================================================
// EMERGENCY ALERT SYSTEM - Main Implementation
// Shows how fall detection sends real-time alerts to mobile app
// ============================================================================

#ifndef EMERGENCY_ALERT_H
#define EMERGENCY_ALERT_H

#include "fallDetection.h"
#include "bleManager.h"
#include "wifiManager.h"
#include "sensor.h"
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// Alert Types & Severity Levels
// ============================================================================

typedef enum {
    ALERT_TYPE_FALL = 1,              // Fall detected
    ALERT_TYPE_ABNORMAL_HR = 2,       // Abnormal heart rate
    ALERT_TYPE_LOW_SPO2 = 3,          // Low oxygen saturation
    ALERT_TYPE_ARRHYTHMIA = 4,        // Heart rhythm abnormality
    ALERT_TYPE_INACTIVITY = 5,        // User inactive too long
    ALERT_TYPE_DEVICE_ERROR = 6,      // Device/sensor error
    ALERT_TYPE_BATTERY_LOW = 7,       // Battery critical
} AlertType;

typedef enum {
    SEVERITY_INFO = 0,                // Informational
    SEVERITY_WARNING = 1,             // Warning - needs attention
    SEVERITY_CRITICAL = 2,            // Critical - immediate action needed
} AlertSeverity;

typedef struct {
    uint32_t alertId;
    AlertType type;
    AlertSeverity severity;
    uint32_t timestamp;
    char message[256];
    bool deliveredToApp;
    bool userAcknowledged;
    uint32_t acknowledgeTime;
    char deviceId[32];
} EmergencyAlert;

// ============================================================================
// MAIN ALERT FUNCTION - Sends to App
// ============================================================================

/**
 * Send alert directly to mobile app via BLE
 * @param alertType Type of alert
 * @param severity Alert severity level
 * @param message Alert message text
 * @return true if alert sent successfully
 */
bool sendAlertToApp(AlertType alertType, AlertSeverity severity, const char* message);

/**
 * Send fall alert specifically (for fall detection)
 * @param result Fall detection result with all details
 * @return true if alert sent successfully
 */
bool sendFallAlertToApp(const FallDetectionResult* result);

/**
 * Send heart rate alert to app
 * @param currentHR Current heart rate
 * @param baselineHR Baseline heart rate
 * @param deviation Deviation percentage
 * @return true if alert sent successfully
 */
bool sendHeartRateAlertToApp(uint16_t currentHR, uint16_t baselineHR, float deviation);

/**
 * Send SpO2 alert to app
 * @param spO2 Current SpO2 percentage
 * @return true if alert sent successfully
 */
bool sendSpO2AlertToApp(uint16_t spO2);

/**
 * Send multi-alert with all sensor data
 * @param fallResult Fall detection data (can be NULL)
 * @param hrData Heart rate data (can be NULL)
 * @param spo2Data SpO2 data (can be NULL)
 * @return true if alert sent successfully
 */
bool sendMultiSensorAlertToApp(const FallDetectionResult* fallResult,
                               const HeartRateData* hrData,
                               const HeartRateData* spo2Data);

// ============================================================================
// DUAL CHANNEL DELIVERY - BLE + WiFi
// ============================================================================

/**
 * Send alert via BLE (primary - immediate)
 * @param alert Alert to send
 * @return true if sent successfully
 */
bool sendAlertViaBLE(const EmergencyAlert* alert);

/**
 * Send alert via WiFi/MQTT (backup - reliable)
 * @param alert Alert to send
 * @return true if sent successfully
 */
bool sendAlertViaWiFi(const EmergencyAlert* alert);

/**
 * Send alert via both BLE and WiFi for redundancy
 * @param alert Alert to send
 * @return true if at least one channel successful
 */
bool sendAlertDualChannel(const EmergencyAlert* alert);

/**
 * Retry failed alerts
 * @return Number of alerts successfully retried
 */
uint16_t retryFailedAlerts();

// ============================================================================
// REAL-TIME NOTIFICATIONS - Instant Delivery to App
// ============================================================================

/**
 * Send push notification to app immediately
 * @param title Notification title
 * @param message Notification body
 * @return true if sent successfully
 */
bool sendPushNotification(const char* title, const char* message);

/**
 * Send priority notification (wakes app)
 * @param message Urgent message
 * @return true if sent successfully
 */
bool sendPriorityNotification(const char* message);

/**
 * Send high-priority alert with sound/vibration
 * @param message Alert message
 * @param soundLevel Sound level (0-10)
 * @param vibrationPattern Vibration pattern
 * @return true if sent successfully
 */
bool sendHighPriorityAlert(const char* message, uint8_t soundLevel, uint8_t vibrationPattern);

// ============================================================================
// ALERT QUEUE & BUFFERING - Ensures Delivery
// ============================================================================

/**
 * Queue alert for transmission (in case app not connected)
 * @param alert Alert to queue
 * @return Queue ID (0 if failed)
 */
uint32_t queueAlert(const EmergencyAlert* alert);

/**
 * Get number of pending alerts
 * @return Number of alerts in queue
 */
uint16_t getPendingAlertCount();

/**
 * Process alert queue (send all pending)
 * @return Number of alerts sent
 */
uint16_t processAlertQueue();

/**
 * Clear alert queue
 * @return true if successful
 */
bool clearAlertQueue();

/**
 * Get oldest pending alert
 * @param alert Pointer to store alert
 * @return true if alert available
 */
bool getOldestPendingAlert(EmergencyAlert* alert);

// ============================================================================
// APP ACKNOWLEDGMENT TRACKING
// ============================================================================

/**
 * Register callback for app acknowledgment
 * @param callback Function called when app acknowledges
 * @return true if successful
 */
typedef void (*AlertAckCallback)(uint32_t alertId, uint32_t ackTime);
bool registerAlertAckCallback(AlertAckCallback callback);

/**
 * Wait for app acknowledgment with timeout
 * @param alertId Alert ID to wait for
 * @param timeoutSeconds Timeout in seconds
 * @return true if acknowledged within timeout
 */
bool waitForAppAcknowledge(uint32_t alertId, uint32_t timeoutSeconds);

/**
 * Check if alert was acknowledged by app
 * @param alertId Alert ID
 * @return true if acknowledged
 */
bool isAlertAcknowledged(uint32_t alertId);

/**
 * Get acknowledgment time
 * @param alertId Alert ID
 * @return Time since alert until acknowledgment (seconds)
 */
uint32_t getAcknowledgmentDelay(uint32_t alertId);

/**
 * Resend alert if not acknowledged
 * @param alertId Alert ID
 * @param maxRetries Maximum retries
 * @return true if resent
 */
bool resendIfNotAcknowledged(uint32_t alertId, uint8_t maxRetries);

// ============================================================================
// EMERGENCY CONTACT NOTIFICATIONS
// ============================================================================

/**
 * Send alert to emergency contacts via SMS
 * @param message SMS message
 * @param phoneNumber Emergency contact phone
 * @return true if SMS sent successfully
 */
bool sendAlertViaSMS(const char* message, const char* phoneNumber);

/**
 * Send alert to emergency contacts via email
 * @param subject Email subject
 * @param message Email body
 * @param emailAddress Emergency contact email
 * @return true if email sent successfully
 */
bool sendAlertViaEmail(const char* subject, const char* message, const char* emailAddress);

/**
 * Send location with alert (GPS coordinates)
 * @param latitude GPS latitude
 * @param longitude GPS longitude
 * @param accuracy GPS accuracy in meters
 * @return true if location sent
 */
bool sendAlertWithLocation(float latitude, float longitude, float accuracy);

/**
 * Call emergency services
 * @param emergencyType Type of emergency
 * @return true if call initiated
 */
bool callEmergencyServices(uint8_t emergencyType);

// ============================================================================
// CRITICAL ALERT PROTOCOL - For Fall/Severe Events
// ============================================================================

/**
 * Activate critical alert mode
 * @param timeoutSeconds How long to keep alerting
 * @return true if activated
 */
bool activateCriticalAlertMode(uint32_t timeoutSeconds);

/**
 * Get critical alert status
 * @return true if critical mode active
 */
bool isCriticalAlertActive();

/**
 * Cancel critical alert
 * @return true if successful
 */
bool cancelCriticalAlert();

/**
 * Set countdown timer before emergency services called
 * @param secondsDelay Delay in seconds
 * @return true if successful
 */
bool setEmergencyCallCountdown(uint32_t secondsDelay);

/**
 * Get seconds until emergency services called
 * @return Seconds remaining (0 if already called)
 */
uint32_t getSecondsUntilEmergencyCall();

/**
 * Manual override - confirm user is safe
 * @return true if confirmed
 */
bool confirmUserSafe();

// ============================================================================
// ALERT HISTORY & REPORTING
// ============================================================================

/**
 * Get alert history
 * @param alerts Array to store alerts
 * @param maxCount Maximum alerts to retrieve
 * @return Number of alerts retrieved
 */
uint16_t getAlertHistory(EmergencyAlert* alerts, uint16_t maxCount);

/**
 * Get recent alerts (last N hours)
 * @param hoursBack Number of hours to look back
 * @param alerts Array to store alerts
 * @param maxCount Maximum alerts to retrieve
 * @return Number of alerts found
 */
uint16_t getRecentAlerts(uint32_t hoursBack, EmergencyAlert* alerts, uint16_t maxCount);

/**
 * Generate alert report
 * @param filename Output filename
 * @param daysBack Number of days to include
 * @return true if successful
 */
bool generateAlertReport(const char* filename, uint16_t daysBack);

/**
 * Export alerts as JSON for app analysis
 * @param buffer Output buffer
 * @param bufferSize Buffer size
 * @return Number of bytes written
 */
uint16_t exportAlertsJSON(char* buffer, uint16_t bufferSize);

/**
 * Get alert statistics
 * @param totalAlerts Pointer to store total
 * @param acknowledgedAlerts Pointer to store acknowledged
 * @param falseAlarms Pointer to store false alarms
 * @return true if successful
 */
bool getAlertStatistics(uint32_t* totalAlerts, uint32_t* acknowledgedAlerts, uint32_t* falseAlarms);

// ============================================================================
// TESTING & SIMULATION
// ============================================================================

/**
 * Test alert delivery to app (simulation)
 * @return true if test successful
 */
bool testAlertToApp();

/**
 * Simulate fall alert (for testing)
 * @return Alert ID
 */
uint32_t simulateFallAlert();

/**
 * Simulate abnormal HR alert (for testing)
 * @return Alert ID
 */
uint32_t simulateAbnormalHRAlert();

/**
 * Simulate low SpO2 alert (for testing)
 * @return Alert ID
 */
uint32_t simulateLowSpO2Alert();

#endif // EMERGENCY_ALERT_H

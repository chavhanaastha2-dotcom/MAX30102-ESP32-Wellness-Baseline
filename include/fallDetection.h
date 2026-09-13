#ifndef FALL_DETECTION_H
#define FALL_DETECTION_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// ============================================================================
// MPU6050 Fall Detection Module
// Logic Flow:
// 1. Acceleration suddenly changes -> Possible fall
// 2. Check orientation (device angle from vertical)
// 3. Check inactivity (immobile on ground)
// 4. If conditions met -> Detected fall
// 5. Generate emergency event
// ============================================================================

// ============================================================================
// Data Structures for Fall Detection
// ============================================================================

typedef struct {
    float x;
    float y;
    float z;
} AccelerationData;

typedef struct {
    float x;
    float y;
    float z;
} AngularVelocityData;

typedef struct {
    float roll;    // Rotation around X axis
    float pitch;   // Rotation around Y axis
    float yaw;     // Rotation around Z axis
} OrientationData;

typedef enum {
    FALL_STATE_NORMAL = 0,           // Normal activity
    FALL_STATE_POSSIBLE = 1,         // Possible fall detected
    FALL_STATE_CHECKING = 2,         // Verifying fall conditions
    FALL_STATE_CONFIRMED = 3,        // Fall confirmed
    FALL_STATE_ALERT_SENT = 4,       // Alert sent to user
    FALL_STATE_RECOVERY = 5          // User recovered from fall
} FallState;

typedef struct {
    FallState state;
    bool isFallDetected;
    uint32_t fallDetectionTime;
    uint32_t lastMovementTime;
    float maxAccelerationMagnitude;  // Peak acceleration during fall
    OrientationData orientationAtFall;
    float fallConfidenceScore;       // 0-100
    bool userResponded;              // Did user respond to alert
} FallDetectionResult;

typedef struct {
    float accelThreshold;            // Acceleration threshold for fall (m/s²)
    float orientationThreshold;      // Angle threshold for fall detection (degrees)
    float inactivityTimeout;         // Inactivity timeout (seconds)
    float recoveryThreshold;         // Movement threshold to consider recovery (m/s²)
    uint32_t confirmationDelay;      // Delay before confirming fall (milliseconds)
    uint8_t sensitivity;             // 1-10 (higher = more sensitive)
} FallDetectionThresholds;

typedef struct {
    uint32_t timestamp;
    FallState state;
    float accelerationMagnitude;
    OrientationData orientation;
    float confidenceScore;
    const char* description;
} FallDetectionEvent;

typedef struct {
    uint32_t totalFallsDetected;
    uint32_t truePositives;          // Confirmed falls
    uint32_t falsePositives;         // False alarms
    uint32_t trueNegatives;          // Correctly rejected
    uint32_t falseNegatives;         // Missed falls
    float detectionAccuracy;         // Percentage
    uint32_t lastFallTime;
    uint32_t averageResponseTime;    // User response time (seconds)
} FallDetectionStatistics;

// ============================================================================
// Initialization & Configuration
// ============================================================================

/**
 * Initialize MPU6050 sensor for fall detection
 * @return true if successful
 */
bool initMPU6050();

/**
 * Deinitialize MPU6050
 * @return true if successful
 */
bool deinitMPU6050();

/**
 * Check if MPU6050 is connected
 * @return true if sensor responds
 */
bool isMPU6050Connected();

/**
 * Calibrate MPU6050 (should be done on flat surface)
 * @return true if successful
 */
bool calibrateMPU6050();

/**
 * Initialize fall detection system
 * @return true if successful
 */
bool initFallDetection();

/**
 * Enable fall detection
 * @return true if successful
 */
bool enableFallDetection();

/**
 * Disable fall detection
 * @return true if successful
 */
bool disableFallDetection();

/**
 * Is fall detection enabled
 * @return true if enabled
 */
bool isFallDetectionEnabled();

/**
 * Set fall detection sensitivity (1-10)
 * @param sensitivity Sensitivity level
 * @return true if successful
 */
bool setFallDetectionSensitivity(uint8_t sensitivity);

/**
 * Get fall detection sensitivity
 * @return Current sensitivity (1-10)
 */
uint8_t getFallDetectionSensitivity();

/**
 * Set custom fall detection thresholds
 * @param thresholds FallDetectionThresholds structure
 * @return true if successful
 */
bool setFallDetectionThresholds(const FallDetectionThresholds* thresholds);

/**
 * Get current fall detection thresholds
 * @return Current thresholds
 */
FallDetectionThresholds getFallDetectionThresholds();

// ============================================================================
// Core Fall Detection Logic (Aspect 1: Acceleration Detection)
// ============================================================================

/**
 * Read raw acceleration from MPU6050
 * @param accel Pointer to store acceleration data
 * @return true if successful
 */
bool readAcceleration(AccelerationData* accel);

/**
 * Read raw gyroscope data
 * @param gyro Pointer to store angular velocity
 * @return true if successful
 */
bool readGyroscope(AngularVelocityData* gyro);

/**
 * Calculate acceleration magnitude
 * @param accel Acceleration data
 * @return Magnitude in m/s²
 */
float calculateAccelerationMagnitude(const AccelerationData* accel);

/**
 * Detect sudden acceleration change (Step 1: Possible Fall)
 * @param currentAccel Current acceleration
 * @param previousAccel Previous acceleration
 * @return true if sudden change detected
 */
bool detectSuddenAcceleration(const AccelerationData* currentAccel, 
                              const AccelerationData* previousAccel);

/**
 * Get acceleration change magnitude
 * @param currentAccel Current acceleration
 * @param previousAccel Previous acceleration
 * @return Change magnitude in m/s²
 */
float getAccelerationChange(const AccelerationData* currentAccel,
                           const AccelerationData* previousAccel);

/**
 * Detect free fall condition
 * @param accel Current acceleration
 * @return true if free fall detected
 */
bool detectFreeFall(const AccelerationData* accel);

// ============================================================================
// Orientation Analysis (Step 2: Check Orientation)
// ============================================================================

/**
 * Calculate device orientation from acceleration
 * @param accel Acceleration data
 * @param orientation Pointer to store orientation
 * @return true if successful
 */
bool calculateOrientation(const AccelerationData* accel, OrientationData* orientation);

/**
 * Calculate orientation using gyroscope integration
 * @param accel Acceleration data
 * @param gyro Angular velocity data
 * @param orientation Pointer to store orientation
 * @return true if successful
 */
bool calculateOrientationWithGyro(const AccelerationData* accel,
                                  const AngularVelocityData* gyro,
                                  OrientationData* orientation);

/**
 * Check if device orientation indicates fall
 * @param orientation Device orientation
 * @return true if orientation suggests person is on ground
 */
bool isOrientationConsistentWithFall(const OrientationData* orientation);

/**
 * Get angle from vertical
 * @param orientation Device orientation
 * @return Angle in degrees (0-180)
 */
float getAngleFromVertical(const OrientationData* orientation);

/**
 * Check if device is face-down
 * @param orientation Device orientation
 * @return true if face-down
 */
bool isDeviceFaceDown(const OrientationData* orientation);

/**
 * Check if device is face-up
 * @param orientation Device orientation
 * @return true if face-up
 */
bool isDeviceFaceUp(const OrientationData* orientation);

/**
 * Check if device is on its side
 * @param orientation Device orientation
 * @return true if on side
 */
bool isDeviceOnSide(const OrientationData* orientation);

// ============================================================================
// Inactivity Analysis (Step 3: Check Inactivity)
// ============================================================================

/**
 * Check if device has been inactive (immobile)
 * @param durationSeconds Duration to check (seconds)
 * @return true if inactive for duration
 */
bool isDeviceInactive(uint32_t durationSeconds);

/**
 * Get time since last significant movement
 * @return Time in seconds
 */
uint32_t getTimeSinceLastMovement();

/**
 * Detect sustained immobility
 * @param accelerationThreshold Movement threshold (m/s²)
 * @param durationSeconds Required immobility duration
 * @return true if immobile for duration
 */
bool detectSustainedImmobility(float accelerationThreshold, uint32_t durationSeconds);

/**
 * Get average acceleration over time window
 * @param windowSeconds Time window in seconds
 * @return Average acceleration magnitude
 */
float getAverageAcceleration(uint32_t windowSeconds);

/**
 * Monitor motion for recovery
 * @param recoveryThreshold Movement threshold for recovery
 * @return true if movement detected above threshold
 */
bool monitorForRecovery(float recoveryThreshold);

// ============================================================================
// Fall Detection & Confirmation (Step 4: Detected Fall)
// ============================================================================

/**
 * Check all fall detection conditions
 * @return FallDetectionResult with current status
 */
FallDetectionResult checkFallDetection();

/**
 * Confirm fall with confidence scoring
 * @param acceleration Peak acceleration magnitude
 * @param orientation Device orientation at fall
 * @param inactivityDuration Duration of immobility
 * @return Confidence score (0-100)
 */
float calculateFallConfidenceScore(float acceleration,
                                   const OrientationData* orientation,
                                   uint32_t inactivityDuration);

/**
 * Get current fall state
 * @return Current FallState
 */
FallState getFallState();

/**
 * Get fall state as string
 * @return State description
 */
const char* getFallStateString();

/**
 * Is fall currently detected
 * @return true if fall detected
 */
bool isFallDetected();

/**
 * Get fall detection result
 * @return Current FallDetectionResult
 */
FallDetectionResult getFallDetectionResult();

/**
 * Get time elapsed since fall detected
 * @return Time in seconds
 */
uint32_t getTimeSinceFallDetected();

/**
 * Manual fall confirmation (button press or app)
 * @return true if successful
 */
bool manualFallConfirmation();

/**
 * Dismiss fall alert
 * @return true if successful
 */
bool dismissFallAlert();

// ============================================================================
// Emergency Events (Step 5: Generate Emergency Event)
// ============================================================================

/**
 * Register fall detection callback
 * @param callback Function to call when fall detected
 * @return true if successful
 */
typedef void (*FallDetectionCallback)(const FallDetectionResult* result);
bool registerFallDetectionCallback(FallDetectionCallback callback);

/**
 * Generate fall emergency event
 * @param result Fall detection result
 * @return Event ID
 */
uint32_t generateFallEmergencyEvent(const FallDetectionResult* result);

/**
 * Send emergency alert
 * @param message Alert message
 * @param severity Alert severity (1=warning, 2=critical)
 * @return true if successful
 */
bool sendEmergencyAlert(const char* message, uint8_t severity);

/**
 * Send emergency notification to contacts
 * @param phoneNumber Emergency contact phone
 * @param message Message to send
 * @return true if successful
 */
bool sendEmergencyNotification(const char* phoneNumber, const char* message);

/**
 * Activate emergency mode
 * @return true if successful
 */
bool activateEmergencyMode();

/**
 * Deactivate emergency mode
 * @return true if successful
 */
bool deactivateEmergencyMode();

/**
 * Is emergency mode active
 * @return true if active
 */
bool isEmergencyModeActive();

/**
 * Log fall event to storage
 * @param result Fall detection result
 * @return true if successful
 */
bool logFallEvent(const FallDetectionResult* result);

/**
 * Get fall event history
 * @param events Array to store events
 * @param maxCount Maximum events to retrieve
 * @return Number of events retrieved
 */
uint16_t getFallEventHistory(FallDetectionEvent* events, uint16_t maxCount);

/**
 * Clear fall event history
 * @return true if successful
 */
bool clearFallEventHistory();

// ============================================================================
// Advanced Pattern Recognition
// ============================================================================

/**
 * Detect rapid descent pattern
 * @param accel Current acceleration
 * @param gyro Angular velocity
 * @return Descent velocity in m/s
 */
float detectRapidDescent(const AccelerationData* accel, const AngularVelocityData* gyro);

/**
 * Analyze body impact
 * @param accel Acceleration at impact
 * @return Impact force estimate
 */
float analyzeBodyImpact(const AccelerationData* accel);

/**
 * Detect rolling motion after fall
 * @param gyro Angular velocity
 * @return true if rolling detected
 */
bool detectRollingMotion(const AngularVelocityData* gyro);

/**
 * Detect person lying down vs standing
 * @param orientation Device orientation
 * @param accel Acceleration data
 * @return 0=standing, 1=sitting, 2=lying
 */
uint8_t detectBodyPosition(const OrientationData* orientation, const AccelerationData* accel);

/**
 * Analyze head strike risk
 * @param accel Impact acceleration
 * @param gyro Angular motion
 * @return Risk level (0-100)
 */
uint8_t analyzeHeadStrikeRisk(const AccelerationData* accel, const AngularVelocityData* gyro);

// ============================================================================
// User Response & Recovery
// ============================================================================

/**
 * Monitor user response to alert
 * @param timeoutSeconds Timeout for user response
 * @return true if user acknowledged
 */
bool monitorUserResponse(uint32_t timeoutSeconds);

/**
 * Detect user recovery movement
 * @return true if recovery movement detected
 */
bool detectRecoveryMovement();

/**
 * Confirm user is okay (button press/app confirmation)
 * @return true if confirmed
 */
bool confirmUserOkay();

/**
 * Get time until emergency services called
 * @return Time in seconds (0 if already called)
 */
uint32_t getTimeUntilEmergency();

/**
 * Cancel emergency call
 * @return true if successful
 */
bool cancelEmergencyCall();

// ============================================================================
// Statistics & Analysis
// ============================================================================

/**
 * Get fall detection statistics
 * @return FallDetectionStatistics structure
 */
FallDetectionStatistics getFallDetectionStatistics();

/**
 * Calculate detection accuracy
 * @return Accuracy percentage (0-100)
 */
float getDetectionAccuracy();

/**
 * Get false positive rate
 * @return False positive rate (0-100%)
 */
float getFalsePositiveRate();

/**
 * Generate fall detection report
 * @param filename Output filename
 * @return true if successful
 */
bool generateFallDetectionReport(const char* filename);

/**
 * Export fall data as JSON
 * @param buffer Output buffer
 * @param bufferSize Buffer size
 * @return Number of bytes written
 */
uint16_t exportFallDataJSON(char* buffer, uint16_t bufferSize);

/**
 * Reset fall detection statistics
 * @return true if successful
 */
bool resetFallDetectionStatistics();

// ============================================================================
// Sensor Health & Diagnostics
// ============================================================================

/**
 * Perform MPU6050 self-test
 * @return true if all tests pass
 */
bool mpu6050SelfTest();

/**
 * Get MPU6050 status
 * @return Status code (0 = healthy)
 */
uint8_t getMPU6050Status();

/**
 * Get MPU6050 temperature
 * @return Temperature in Celsius
 */
float getMPU6050Temperature();

/**
 * Check sensor calibration quality
 * @return Quality score (0-100)
 */
uint8_t checkCalibrationQuality();

/**
 * Get detailed sensor diagnostics
 * @param buffer Output buffer
 * @param bufferSize Buffer size
 * @return Number of bytes written
 */
uint16_t getSensorDiagnostics(char* buffer, uint16_t bufferSize);

#endif // FALL_DETECTION_H

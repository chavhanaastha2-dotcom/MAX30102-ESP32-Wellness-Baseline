#ifndef BASELINE_ANALYSIS_H
#define BASELINE_ANALYSIS_H

#include "sensor.h"
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// ============================================================================
// Data Structures for Baseline Analysis (Aspect 2)
// ============================================================================

typedef struct {
    uint16_t mean;
    uint16_t median;
    uint16_t stdDev;
    uint16_t min;
    uint16_t max;
    uint16_t p25;  // 25th percentile
    uint16_t p50;  // 50th percentile (median)
    uint16_t p75;  // 75th percentile
    uint16_t p90;  // 90th percentile
    uint16_t p95;  // 95th percentile
    uint32_t sampleCount;
} StatisticalMetrics;

typedef struct {
    uint8_t dayOfWeek;  // 0=Monday, 6=Sunday
    uint8_t timeSlot;   // 0-23 (hour of day)
    StatisticalMetrics hrMetrics;
    StatisticalMetrics spo2Metrics;
    uint16_t sampleCount;
    bool isComplete;
} TimeSpecificBaseline;

typedef struct {
    bool isReady;           // Baseline collection complete
    uint32_t collectionStartTime;
    uint32_t collectionEndTime;
    uint16_t daysCollected;
    uint32_t totalReadingsCollected;
    
    // Overall metrics
    StatisticalMetrics overallHRMetrics;
    StatisticalMetrics overallSpO2Metrics;
    
    // Time-specific baselines (for each hour of day)
    TimeSpecificBaseline timeBaselines[24];
    
    // Day-of-week patterns
    StatisticalMetrics weekdayHRMetrics;
    StatisticalMetrics weekendHRMetrics;
    
    // Resting HR baseline
    uint16_t restingHeartRate;
    uint16_t sleepHeartRate;
    
    // Version and metadata
    uint32_t lastUpdateTime;
    uint8_t formatVersion;
} BaselineData;

typedef struct {
    uint16_t value;
    uint32_t timestamp;
    uint8_t hour;
    uint8_t dayOfWeek;
    float deviationPercent;
} BaselineComparison;

// ============================================================================
// Baseline Collection Functions (Aspect 2)
// ============================================================================

/**
 * Initialize baseline collection
 * @return true if successful
 */
bool initBaselineCollection();

/**
 * Add reading to baseline data
 * @param data Reading data
 * @return true if successful
 */
bool addReadingToBaseline(const HeartRateData* data);

/**
 * Check if baseline collection is complete
 * @return true if 15 days collected
 */
bool isBaselineComplete();

/**
 * Get baseline collection progress
 * @return Percentage (0-100)
 */
uint8_t getBaselineProgress();

/**
 * Get days collected so far
 * @return Number of days
 */
uint16_t getDaysCollected();

/**
 * Force baseline completion (for testing)
 * @return true if successful
 */
bool forceBaselineCompletion();

/**
 * Reset baseline collection
 * @return true if successful
 */
bool resetBaseline();

// ============================================================================
// Statistical Analysis Functions (Aspect 2)
// ============================================================================

/**
 * Calculate statistical metrics from data array
 * @param values Array of values
 * @param count Number of values
 * @return StatisticalMetrics structure
 */
StatisticalMetrics calculateMetrics(uint16_t* values, uint32_t count);

/**
 * Calculate percentile
 * @param values Sorted array of values
 * @param count Number of values
 * @param percentile Percentile (0-100)
 * @return Percentile value
 */
uint16_t calculatePercentile(uint16_t* values, uint32_t count, uint8_t percentile);

/**
 * Calculate rolling average
 * @param values Array of values
 * @param count Number of values
 * @param windowSize Window size for averaging
 * @param output Array to store results
 * @return Number of results
 */
uint16_t calculateRollingAverage(uint16_t* values, uint32_t count, 
                                  uint16_t windowSize, uint16_t* output);

/**
 * Get time-of-day specific baseline
 * @param hour Hour of day (0-23)
 * @return TimeSpecificBaseline for that hour
 */
TimeSpecificBaseline getTimeBaseline(uint8_t hour);

/**
 * Get day-of-week specific baseline
 * @param dayOfWeek Day of week (0=Monday, 6=Sunday)
 * @return StatisticalMetrics for that day
 */
StatisticalMetrics getDayBaseline(uint8_t dayOfWeek);

/**
 * Get overall baseline metrics
 * @return BaselineData structure
 */
BaselineData getBaselineData();

/**
 * Update baseline statistics (recalculate from stored data)
 * @return true if successful
 */
bool updateBaselineStatistics();

// ============================================================================
// Trend Analysis Functions (Aspect 2)
// ============================================================================

/**
 * Calculate 7-day moving average
 * @param values Array of values
 * @param count Number of values
 * @return 7-day average value
 */
uint16_t calculate7DayAverage(uint16_t* values, uint32_t count);

/**
 * Calculate 14-day moving average
 * @param values Array of values
 * @param count Number of values
 * @return 14-day average value
 */
uint16_t calculate14DayAverage(uint16_t* values, uint32_t count);

/**
 * Detect trends in data
 * @param values Array of values
 * @param count Number of values
 * @return -1 for downtrend, 0 for stable, 1 for uptrend
 */
int8_t detectTrend(uint16_t* values, uint32_t count);

/**
 * Calculate variance over time period
 * @param values Array of values
 * @param count Number of values
 * @return Variance value
 */
float calculateVariance(uint16_t* values, uint32_t count);

/**
 * Calculate correlation between two data sets
 * @param values1 First array
 * @param values2 Second array
 * @param count Array size
 * @return Correlation coefficient (-1 to 1)
 */
float calculateCorrelation(uint16_t* values1, uint16_t* values2, uint32_t count);

// ============================================================================
// Pattern Recognition Functions (Aspect 2)
// ============================================================================

/**
 * Identify normal HR range for current time
 * @return Expected HR range (stored in result structure)
 */
BaselineComparison getExpectedHRRange();

/**
 * Get circadian rhythm pattern
 * @param hour Hour of day (0-23)
 * @return Expected HR for that hour
 */
uint16_t getCircadianHRExpected(uint8_t hour);

/**
 * Calculate sleep pattern baseline
 * @return Average sleep HR
 */
uint16_t calculateSleepHRBaseline();

/**
 * Calculate resting HR baseline (morning reading)
 * @return Morning HR baseline
 */
uint16_t calculateRestingHRBaseline();

/**
 * Identify activity patterns
 * @return Activity level (0=rest, 1=light, 2=moderate, 3=vigorous)
 */
uint8_t identifyActivityPattern();

// ============================================================================
// Baseline Comparison Functions (Aspect 2)
// ============================================================================

/**
 * Compare reading with baseline
 * @param data Current reading
 * @return Comparison result with deviation percent
 */
BaselineComparison compareWithBaseline(const HeartRateData* data);

/**
 * Get baseline deviation percentage
 * @param value Current value
 * @param baselineValue Expected baseline value
 * @return Deviation percentage
 */
float getDeviationPercent(uint16_t value, uint16_t baselineValue);

/**
 * Check if reading is within normal range
 * @param data Reading data
 * @return true if within expected range
 */
bool isWithinNormalRange(const HeartRateData* data);

/**
 * Get expected range for current time
 * @param minExpected Pointer to store min
 * @param maxExpected Pointer to store max
 */
void getExpectedRange(uint16_t* minExpected, uint16_t* maxExpected);

// ============================================================================
// Reporting Functions (Aspect 2)
// ============================================================================

/**
 * Generate baseline report
 * @param filename Output filename
 * @return true if successful
 */
bool generateBaselineReport(const char* filename);

/**
 * Generate daily summary
 * @param dayIndex Day index (0=today, 1=yesterday, etc)
 * @param filename Output filename
 * @return true if successful
 */
bool generateDailySummary(uint16_t dayIndex, const char* filename);

/**
 * Generate weekly report
 * @param weekIndex Week index (0=current week)
 * @param filename Output filename
 * @return true if successful
 */
bool generateWeeklyReport(uint16_t weekIndex, const char* filename);

/**
 * Export baseline as JSON
 * @param filename Output filename
 * @return true if successful
 */
bool exportBaselineAsJSON(const char* filename);

/**
 * Get baseline statistics as string
 * @param buffer Output buffer
 * @param bufferSize Buffer size
 * @return Number of bytes written
 */
uint16_t getBaselineStatsString(char* buffer, uint16_t bufferSize);

#endif // BASELINE_ANALYSIS_H

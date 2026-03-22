/**
 * config.h
 *
 * Core configuration and state definitions.
 * Centralizes all global variables into static configuration and runtime state structures.
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <time.h>
#include <esp_task_wdt.h>

#define FIRMWARE_VERSION "4.1.1-SPLIT-MODE-REFACTORED"

// ==========================
// PINS AND HARDWARE LIMITS
// ==========================
#define MAX_RELAYS 8
#define LED_PIN 8

// ==========================
// TIMEOUT CONSTANTS
// ==========================
#define NTP_TIMEOUT_SEC 10
#define WIFI_TIMEOUT_SEC 15
#define HTTP_TIMEOUT_MS 3000

// ==========================
// DEFAULT SETTINGS (Fallback if flash is empty)
// ==========================
#define DEFAULT_CHECK_INTERVAL 30
#define DEFAULT_REBOOT_DELAY 10
#define DEFAULT_BOOT_WAIT 90
#define DEFAULT_BOOT_TIME 60
#define DEFAULT_FAIL_THRESHOLD 5
#define DEFAULT_RELAY_DELAY 10
#define DEFAULT_SCHEDULE_HOUR 3
#define DEFAULT_SCHEDULE_MINUTE 30

#define LANG_RU 0
#define LANG_EN 1

// ==========================
// NTP CONFIGURATION
// ==========================
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "time.nist.gov"
#define GMT_OFFSET 10800 // +3 Hours (Moscow)
#define DAY_LIGHT_OFFSET 0

// Extern definitions for standard objects
extern Preferences preferences;
extern WebServer server;
extern WiFiClient client;

// ==========================
// SYSTEM CONFIGURATION STRUCT
// Representing persistent parameters written to Flash memory.
// ==========================
struct RelayConfig {
  int pin;
  String name;
  bool enabled;
  bool state;
  int order;
};

struct SystemConfig {
  String wifiSSID;
  String wifiPASS;
  String pingTarget;
  int failThreshold;
  int rebootDelay;
  int bootWait;
  int checkInterval;
  int relayCount;
  int relayLogic;
  int bootTime;
  int language;

  bool sequentialEnabled;
  int relayDelay;

  bool scheduleEnabled;
  int scheduleHour;
  int scheduleMinute;

  String apSSID;
  String apPASS;
  bool apEnabled;
  bool autoApEnabled;

  RelayConfig relays[MAX_RELAYS];
};

// ==========================
// RUNTIME STATE STRUCT
// Representing volatile memory states matching current device conditions.
// ==========================
struct SystemState {
  bool wifiConnected;
  unsigned long lastWifiCheck;
  unsigned long apActivationTime;
  bool inApMode;

  int failCount;
  bool internetWasLost;
  unsigned long lastCheckTime;
  
  bool isRebooting;
  unsigned long rebootStartTime;
  bool inBootWait;
  int currentRelayIndex;
  bool sequentialBoot;
  
  bool scheduleTriggered;
  unsigned long lastRebootDate;
  
  bool ntpSynced;
  bool monitoringPaused;
};

// Global exports mappings
extern SystemConfig sysConfig;
extern SystemState sysState;

// Function Declarations
void connectToWiFi();
void enableAPMode();
void disableAPMode();
void checkWiFiConnection();
void initNTP();
String getCurrentTime();
int getCurrentHour();
int getCurrentMinute();
int getCurrentDay();

void initRelays();
void setRelay(int id, bool state);
void setAllRelays(bool state);
void triggerReboot();
String generateRelaysHTML();
String getActiveRelaysString();
void sortRelaysByOrder();

bool checkInternet();
void checkSchedule();

void loadSettings();
void saveSettings();

void initWebServer();
void handleRoot();
void handleStatus();
void handleSaveLanguage();
void handleRelayOrder();
void handleSaveAP();
void handleSaveWifi();
void handleSaveAutoAP();
void handleSavePing();
void handleSaveSequential();
void handleSaveSchedule();
void handleManualReboot();
void handlePause();
void handleResume();
void handleAddRelay();
void handleDeleteRelay();
void handleRelayControl();

#endif

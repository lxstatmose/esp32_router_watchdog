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

// Timezone & NTP defaults (Fallback if flash is empty)
#define DEFAULT_GMT_OFFSET_SEC 10800 // UTC+3 (Moscow), in seconds
#define DEFAULT_NTP_SERVER "ntp.yandex.ru" // Yandex public NTP (resolves to 213.180.204.242)
#define DEFAULT_DST_OFFSET_SEC 0 // Daylight saving offset, Russia has none

#define LANG_RU 0
#define LANG_EN 1

// ==========================
// NTP CONFIGURATION
// Primary server is user-configurable (default: Yandex).
// Secondary servers act as automatic fallbacks in configTime().
// ==========================
#define NTP_FALLBACK_1 "pool.ntp.org"
#define NTP_FALLBACK_2 "time.nist.gov"
#define NTP_FALLBACK_3 "ru.pool.ntp.org"
#define GMT_OFFSET DEFAULT_GMT_OFFSET_SEC
#define DAY_LIGHT_OFFSET DEFAULT_DST_OFFSET_SEC

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

  int gmtOffsetSec;
  int dstOffsetSec;
  String ntpServer;

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
void applyTimeSettings();
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
void handleGetTime();
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

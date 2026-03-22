/**
 * wifi.ino
 *
 * Connection management algorithms mapping Station connections and
 * Access Point mode failovers. AutoAP routines run here.
 */

#include "config.h"

// Evaluates current configuration state and attempts a connection to the home router
void connectToWiFi() {
  if (sysConfig.wifiSSID == "" || sysConfig.wifiSSID.length() < 3) {
    Serial.println("[WiFi] No home WiFi credentials found - starting first boot fallback AP!");
    enableAPMode();
    return;
  }
  
  if (sysConfig.apEnabled && !sysConfig.autoApEnabled) {
    Serial.println("[WiFi] AP mode has been manually enforced in settings.");
    sysState.inApMode = true;
    return;
  }
  
  Serial.println("[WiFi] Establishing connection to " + sysConfig.wifiSSID);
  
  WiFi.softAPdisconnect(true);
  sysState.inApMode = false;
  
  // Explicitly set processing to Station-only for routing stability
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(sysConfig.wifiSSID.c_str(), sysConfig.wifiPASS.c_str());

  unsigned long start = millis();
  
  // Await connection assignment up to the timeout boundary
  while (WiFi.status() != WL_CONNECTED && millis() - start < (WIFI_TIMEOUT_SEC * 1000)) {
    delay(100);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    sysState.wifiConnected = true;
    sysState.inApMode = false;
    sysState.apActivationTime = 0;
    Serial.println("\n[WiFi] Network connection success!");
    Serial.print("[IP] Allocated Address: ");
    Serial.println(WiFi.localIP());
  } else {
    sysState.wifiConnected = false;
    Serial.println("\n[WiFi] Critical connection failure. Initializing emergency AP!");
    enableAPMode();
  }
}

// Suspends background router attempts and broadcasts a localized AP for manual configuration
void enableAPMode() {
  Serial.println("[WiFi] Enabling Emergency Config AP...");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(sysConfig.apSSID.c_str(), sysConfig.apPASS.c_str());
  
  sysState.wifiConnected = false;
  sysState.inApMode = true;
  sysState.apActivationTime = millis(); // Track when the AP started
  
  Serial.println("[WiFi] Accessible Network: " + sysConfig.apSSID);
  Serial.println("[WEB] Control Panel: http://192.168.4.1");
}

// Shuts down the emergency AP and retries connecting to the main network
void disableAPMode() {
  Serial.println("[WiFi] User requested AP exit, initiating reconnection to home router...");
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  sysState.inApMode = false;
  sysState.apActivationTime = 0;
  connectToWiFi();
}

// Frequent background task invoked from loop() to monitor physical link status
void checkWiFiConnection() {
  if (millis() - sysState.lastWifiCheck < 30000) return;
  sysState.lastWifiCheck = millis();
  
  if (sysConfig.wifiSSID == "" || sysConfig.wifiSSID.length() < 3) {
    if (!sysState.inApMode) enableAPMode();
    return;
  }
  
  if (sysConfig.apEnabled && !sysConfig.autoApEnabled) {
    if (!sysState.inApMode) enableAPMode();
    return;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    sysState.wifiConnected = true;
    
    // If the board accidentally left AP active while connecting, shut it down
    if (sysState.inApMode) {
      Serial.println("[WiFi] Router connection stabilized. Silencing AP mode.");
      WiFi.softAPdisconnect(true);
      sysState.inApMode = false;
    }
    
    sysState.apActivationTime = 0;
  } else {
    sysState.wifiConnected = false;
    
    // Manage dynamic fallback if network drops while running
    if (sysState.apActivationTime == 0) {
      sysState.apActivationTime = millis();
      Serial.println("[WiFi] Up-link suspended! Countdown initiated before AP fallback...");
    } else if (millis() - sysState.apActivationTime > 60000) {
      Serial.println("[WiFi] Prolonged disconnection detected. Relapsing to AP host mode.");
      enableAPMode();
    }
  }
}

// Fetches standardized NTP timestamps and adjusts local internal Core RTC limits
void initNTP() {
  if (sysState.inApMode || !sysState.wifiConnected) {
    Serial.println("[NTP] Sync bypassed (Hardware is not connected to global routed network)");
    sysState.ntpSynced = false;
    return;
  }
  
  Serial.println("[NTP] Establishing chronometric sync...");
  configTime(GMT_OFFSET, DAY_LIGHT_OFFSET, NTP_SERVER_1, NTP_SERVER_2);
  
  struct tm timeinfo;
  unsigned long start = millis();
  
  // Await clock population safely
  while (!getLocalTime(&timeinfo, 5000)) {
    if (millis() - start > 5000) {
      sysState.ntpSynced = false;
      Serial.println("[NTP] Critical timeout synchronizing time via NTP.");
      return;
    }
    delay(100);
  }
  
  sysState.ntpSynced = true;
  Serial.println("[NTP] Internal RTC synchronized!");
}

// ==========================
// STRING & INTEGER TIME HARVESTERS
// Extracts human-readable metrics from internal ESP32 ESP-IDF hardware RTC
// ==========================

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 1000)) return "--:--";
  char buf[6];
  strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
  return String(buf);
}

int getCurrentHour() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 1000)) return -1;
  return timeinfo.tm_hour;
}

int getCurrentMinute() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 1000)) return -1;
  return timeinfo.tm_min;
}

int getCurrentDay() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 1000)) return -1;
  return timeinfo.tm_yday; // Absolute year day mapping is reliable for triggers
}

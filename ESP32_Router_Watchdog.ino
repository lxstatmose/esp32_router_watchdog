/**
 * ESP32 Router Watchdog
 *
 * Main application file. Initializes the system and contains the primary non-blocking loop.
 * Delegates specific logic to modular systems (monitor, wifi, web, relay, storage).
 */

#include "config.h"
#include "web_html.h"

Preferences preferences;
WebServer server(80);
WiFiClient client;

SystemConfig sysConfig;
SystemState sysState;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  delay(1000);
  
  Serial.println("\n");
  Serial.println("========================================");
  Serial.println("[BOOT] Router Watchdog v" + String(FIRMWARE_VERSION));
  Serial.println("========================================");
  
  // Set default initial states
  sysState = {0}; 
  sysState.inApMode = false;
  sysState.monitoringPaused = false;
  
  sysConfig.pingTarget = "google.com";
  sysConfig.failThreshold = DEFAULT_FAIL_THRESHOLD;
  sysConfig.rebootDelay = DEFAULT_REBOOT_DELAY;
  sysConfig.bootWait = DEFAULT_BOOT_WAIT;
  sysConfig.checkInterval = DEFAULT_CHECK_INTERVAL;
  sysConfig.bootTime = DEFAULT_BOOT_TIME;
  sysConfig.language = LANG_RU;
  sysConfig.relayDelay = DEFAULT_RELAY_DELAY;
  sysConfig.scheduleHour = DEFAULT_SCHEDULE_HOUR;
  sysConfig.scheduleMinute = DEFAULT_SCHEDULE_MINUTE;
  sysConfig.apSSID = "RouterWatchdog-AP";
  sysConfig.apPASS = "12345678";
  sysConfig.autoApEnabled = true;

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("[INIT] Loading settings...");
  loadSettings();
  
  Serial.println("[INIT] Initializing relays...");
  initRelays();
  
  Serial.println("[INIT] Connecting to WiFi...");
  connectToWiFi();
  
  Serial.println("[INIT] Syncing NTP...");
  initNTP();
  
  Serial.println("[INIT] Starting web server...");
  initWebServer();
  server.begin();
  
  Serial.println("========================================");
  Serial.println("[READY] System ready!");
  
  if (sysState.inApMode) {
    Serial.println("[MODE] Access Point Mode");
    Serial.println("[WEB] http://192.168.4.1");
    Serial.println("[AP] " + sysConfig.apSSID + " / " + sysConfig.apPASS);
  } else if (sysState.wifiConnected) {
    Serial.println("[MODE] Station Mode");
    Serial.print("[WEB] http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("[MODE] Connecting...");
  }
  Serial.println("========================================");

  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  server.handleClient();
  
  checkWiFiConnection();
  
  checkSchedule();

  if (sysState.isRebooting) {
    handleRebootProcess();
    return;
  }

  if (sysState.inBootWait) {
    if (millis() - sysState.rebootStartTime > ((unsigned long)sysConfig.bootTime * 1000)) {
      Serial.println("[MONITOR] Boot wait finished");
      sysState.inBootWait = false;
      digitalWrite(LED_PIN, HIGH);
      sysState.failCount = 0;
    }
    return;
  }

  // Monitoring works in AP+STA mode if WiFi has connected successfully
  if (!sysState.wifiConnected || sysState.monitoringPaused) return;

  if (millis() - sysState.lastCheckTime > ((unsigned long)sysConfig.checkInterval * 1000)) {
    sysState.lastCheckTime = millis();
    handleInternetCheck();
  }

  digitalWrite(LED_PIN, (sysState.failCount > 0) ? LOW : HIGH);
}

void handleRebootProcess() {
  if (sysState.sequentialBoot) {
    if (sysState.currentRelayIndex < sysConfig.relayCount) {
      unsigned long delayForThisRelay = sysConfig.rebootDelay + (sysState.currentRelayIndex * sysConfig.relayDelay);
      if (millis() - sysState.rebootStartTime > (delayForThisRelay * 1000)) {
        if (sysConfig.relays[sysState.currentRelayIndex].enabled && sysConfig.relays[sysState.currentRelayIndex].pin >= 0) {
          digitalWrite(sysConfig.relays[sysState.currentRelayIndex].pin, LOW);
          sysConfig.relays[sysState.currentRelayIndex].state = true;
        }
        sysState.currentRelayIndex++;
      }
    } else {
      sysState.isRebooting = false;
      sysState.inBootWait = true;
      sysState.rebootStartTime = millis();
      sysState.failCount = 0;
    }
  } else {
    if (millis() - sysState.rebootStartTime > ((unsigned long)sysConfig.rebootDelay * 1000)) {
      setAllRelays(true);
      sysState.isRebooting = false;
      sysState.inBootWait = true;
      sysState.rebootStartTime = millis();
      sysState.failCount = 0;
    }
  }
}

void handleInternetCheck() {
  bool internetOK = checkInternet();
  
  if (internetOK) {
    Serial.println("[MONITOR] Internet OK");
    sysState.failCount = 0;
    if (sysState.internetWasLost) {
      sysState.internetWasLost = false;
      digitalWrite(LED_PIN, HIGH);
    }
  } else {
    Serial.println("[MONITOR] Internet FAILED - rebooting routers");
    sysState.failCount++;
    digitalWrite(LED_PIN, LOW);
    
    if (!sysState.internetWasLost && sysState.failCount >= 1) {
      sysState.internetWasLost = true;
    }
    
    if (sysState.failCount >= sysConfig.failThreshold) {
      Serial.println("[MONITOR] Threshold reached! Rebooting routers...");
      triggerReboot();
    }
  }
}

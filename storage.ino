/**
 * storage.ino
 *
 * Wrapper interface for saving and loading configuration across boots
 * using the non-volatile 'Preferences' NVS library built into ESP-IDF.
 */

#include "config.h"

// Grabs all saved namespace pairs out of internal flash memory mapping directly to SystemConfig
void loadSettings() {
  preferences.begin("router-watch", false);
  
  sysConfig.wifiSSID = preferences.getString("ssid", "");
  sysConfig.wifiPASS = preferences.getString("pass", "");
  sysConfig.pingTarget = preferences.getString("target", "google.com");
  sysConfig.failThreshold = preferences.getInt("thresh", DEFAULT_FAIL_THRESHOLD);
  sysConfig.rebootDelay = preferences.getInt("reboot_delay", DEFAULT_REBOOT_DELAY);
  sysConfig.bootWait = preferences.getInt("boot_wait", DEFAULT_BOOT_WAIT);
  sysConfig.checkInterval = preferences.getInt("check_interval", DEFAULT_CHECK_INTERVAL);
  sysConfig.relayCount = preferences.getInt("relay_count", 0);
  sysConfig.relayLogic = preferences.getInt("relay_logic", 0);
  sysConfig.bootTime = preferences.getInt("boot_time", DEFAULT_BOOT_TIME);
  sysConfig.language = preferences.getInt("language", LANG_RU);
  
  sysConfig.sequentialEnabled = preferences.getBool("seq_enabled", false);
  sysConfig.relayDelay = preferences.getInt("seq_delay", DEFAULT_RELAY_DELAY);
  sysConfig.scheduleEnabled = preferences.getBool("sched_enabled", false);
  sysConfig.scheduleHour = preferences.getInt("sched_hour", DEFAULT_SCHEDULE_HOUR);
  sysConfig.scheduleMinute = preferences.getInt("sched_min", DEFAULT_SCHEDULE_MINUTE);
  
  sysConfig.apEnabled = preferences.getBool("ap_enabled", false);
  sysConfig.apSSID = preferences.getString("ap_ssid", "RouterWatchdog-AP");
  sysConfig.apPASS = preferences.getString("ap_pass", "12345678");
  sysConfig.autoApEnabled = preferences.getBool("auto_ap_enabled", true);
  
  // Reconstruct nested arrays based on array length logic
  for (int i = 0; i < sysConfig.relayCount && i < MAX_RELAYS; i++) {
    String key = "relay_" + String(i);
    sysConfig.relays[i].pin = preferences.getInt((key + "_pin").c_str(), -1);
    sysConfig.relays[i].name = preferences.getString((key + "_name").c_str(), "Relay " + String(i));
    sysConfig.relays[i].enabled = true;
    sysConfig.relays[i].state = true;
    sysConfig.relays[i].order = preferences.getInt((key + "_order").c_str(), i);
  }
  
  preferences.end();
  Serial.println("[STORAGE] Non-volatile memory retrieved. Settings securely loaded.");
  
  if (sysConfig.wifiSSID == "") {
    Serial.println("[STORAGE] Registration check failed: No known WiFi parameters found. Fallback mandatory.");
  }
}

// Securely outputs matching structure to flash NVS memory space
void saveSettings() {
  preferences.begin("router-watch", false);
  
  preferences.putString("ssid", sysConfig.wifiSSID);
  preferences.putString("pass", sysConfig.wifiPASS);
  preferences.putString("target", sysConfig.pingTarget);
  preferences.putInt("thresh", sysConfig.failThreshold);
  preferences.putInt("reboot_delay", sysConfig.rebootDelay);
  preferences.putInt("boot_time", sysConfig.bootTime);
  preferences.putInt("check_interval", sysConfig.checkInterval);
  preferences.putInt("boot_wait", sysConfig.bootWait);
  
  preferences.putInt("relay_count", sysConfig.relayCount);
  preferences.putInt("relay_logic", sysConfig.relayLogic);
  preferences.putInt("language", sysConfig.language);
  
  preferences.putBool("seq_enabled", sysConfig.sequentialEnabled);
  preferences.putInt("seq_delay", sysConfig.relayDelay);
  
  preferences.putBool("sched_enabled", sysConfig.scheduleEnabled);
  preferences.putInt("sched_hour", sysConfig.scheduleHour);
  preferences.putInt("sched_min", sysConfig.scheduleMinute);
  
  preferences.putBool("ap_enabled", sysConfig.apEnabled);
  preferences.putString("ap_ssid", sysConfig.apSSID);
  preferences.putString("ap_pass", sysConfig.apPASS);
  preferences.putBool("auto_ap_enabled", sysConfig.autoApEnabled);
  
  preferences.end();
}

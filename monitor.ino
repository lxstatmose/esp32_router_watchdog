/**
 * monitor.ino
 *
 * Handles background internet pinging and checking schedule timers.
 * Interacts heavily with SystemConfig thresholds and SystemState failure tracking.
 */

#include "config.h"

// Makes an HTTP HEAD request to the target domain to verify connection status.
// Returns false instantly if WiFi is locally dropped, otherwise pings target.
bool checkInternet() {
  if (WiFi.status() != WL_CONNECTED) return false;
  
  client.stop(); // Clean any previous hanging connections to free LwIP sockets
  client.setTimeout(HTTP_TIMEOUT_MS);
  
  if (client.connect(sysConfig.pingTarget.c_str(), 80)) {
    client.println("HEAD / HTTP/1.0");
    client.println();
    
    unsigned long timeout = millis();
    while (!client.available() && millis() - timeout < HTTP_TIMEOUT_MS) yield();
    
    client.stop();
    return true; // Successfully pinged
  }
  
  return false; // Connection refused or timeout
}

// Checks if the NTP time matches the scheduled reboot time (e.g. 03:00 AM)
// Prevents multiple reboots in the same minute by tracking lastRebootDate
void checkSchedule() {
  if (!sysConfig.scheduleEnabled || sysState.isRebooting || sysState.inBootWait || !sysState.ntpSynced) return;
  
  int currentDay = getCurrentDay();
  
  if (currentDay != sysState.lastRebootDate) {
    int hour = getCurrentHour();
    int minute = getCurrentMinute();
    
    if (hour == sysConfig.scheduleHour && minute == sysConfig.scheduleMinute && !sysState.scheduleTriggered) {
      Serial.println("[SCHEDULE] Scheduled reboot time reached! Rebooting hardware...");
      triggerReboot();
      sysState.scheduleTriggered = true;
      sysState.lastRebootDate = currentDay; // Ensure it only runs once per day
    }
    
    // Reset trigger toggle once the minute has passed
    if (hour != sysConfig.scheduleHour || minute != sysConfig.scheduleMinute) {
      sysState.scheduleTriggered = false;
    }
  }
}

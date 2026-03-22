/**
 * relay.ino
 *
 * Abstract logic for handling GPIO toggling for physical relays.
 * Includes automated staggered boot delay sequence for multiple routers.
 */

#include "config.h"

// Reorders elements inside the array dynamically for web display sequences
void sortRelaysByOrder() {
  for (int i = 0; i < sysConfig.relayCount - 1; i++) {
    for (int j = 0; j < sysConfig.relayCount - i - 1; j++) {
      if (sysConfig.relays[j].order > sysConfig.relays[j + 1].order) {
        RelayConfig temp = sysConfig.relays[j];
        sysConfig.relays[j] = sysConfig.relays[j + 1];
        sysConfig.relays[j + 1] = temp;
      }
    }
  }
}

// Reads SystemConfig upon boot and sets GPIO behaviors
void initRelays() {
  Serial.println("[RELAY] Initializing hardware pins...");
  sortRelaysByOrder();
  Serial.printf("[RELAY] Active relay count: %d\n", sysConfig.relayCount);
  
  for (int i = 0; i < sysConfig.relayCount && i < MAX_RELAYS; i++) {
    if (sysConfig.relays[i].pin >= 0) {
      pinMode(sysConfig.relays[i].pin, OUTPUT);
      int initialValue = LOW; // Assuming Active-Low relays representing "ON"
      digitalWrite(sysConfig.relays[i].pin, initialValue);
      sysConfig.relays[i].state = true;
      Serial.printf("[RELAY] #%d (order=%d) '%s' attached to GPIO%d\n", 
                    i, sysConfig.relays[i].order, sysConfig.relays[i].name.c_str(), sysConfig.relays[i].pin);
    }
    yield();
  }
  Serial.println("[RELAY] Hardware initialization complete");
}

// Manually turns a specific relay pin High or Low
void setRelay(int id, bool state) {
  Serial.printf("[RELAY] setRelay(%d, %s)\n", id, state ? "ON" : "OFF");
  if (id < 0 || id >= sysConfig.relayCount) return;
  if (sysConfig.relays[id].pin < 0) return;
  
  int value = state ? LOW : HIGH; // Active-Low assumption mapped
  digitalWrite(sysConfig.relays[id].pin, value);
  sysConfig.relays[id].state = state;
  Serial.printf("[RELAY] Relayed #%d status updated to %s\n", id, state ? "ON" : "OFF");
}

// Forces all registered relay configurations to simultaneously mimic identical state
void setAllRelays(bool state) {
  for (int i = 0; i < sysConfig.relayCount; i++) {
    if (sysConfig.relays[i].enabled && sysConfig.relays[i].pin >= 0) {
      int value = state ? LOW : HIGH;
      digitalWrite(sysConfig.relays[i].pin, value);
      sysConfig.relays[i].state = state;
    }
    yield(); // Don't block core
  }
}

// Global invocation initializing the main reboot sequence tasks across loop()
void triggerReboot() {
  Serial.println("[RELAY] Disconnecting power to trigger reboot sequence...");
  sysState.isRebooting = true;
  sysState.rebootStartTime = millis();
  sysState.currentRelayIndex = 0;
  
  // Decide whether hardware should restart sequentially or concurrently
  sysState.sequentialBoot = sysConfig.sequentialEnabled && sysConfig.relayCount > 1;
  setAllRelays(false); // Turn off power to all
}

// Prepares a string enumerating currently ON nodes for dashboard visibility
String getActiveRelaysString() {
  String result = "";
  int activeCount = 0;
  for (int i = 0; i < sysConfig.relayCount; i++) {
    if (sysConfig.relays[i].state) {
      if (activeCount > 0) result += ", ";
      result += sysConfig.relays[i].name;
      activeCount++;
    }
  }
  if (activeCount == 0) {
    result = (sysConfig.language == LANG_RU) ? "Нет активных" : "None active";
  }
  return result;
}

// Prepares chunk of HTML for generating UI grid of controls dynamically
String generateRelaysHTML() {
  String html = "";
  sortRelaysByOrder();
  
  // Localization references mapped dynamically
  String btnUp = (sysConfig.language == LANG_RU) ? "Выше" : "Up";
  String btnDown = (sysConfig.language == LANG_RU) ? "Ниже" : "Down";
  String btnOn = (sysConfig.language == LANG_RU) ? "ВКЛ" : "ON";
  String btnOff = (sysConfig.language == LANG_RU) ? "ВЫКЛ" : "OFF";
  String btnDelete = (sysConfig.language == LANG_RU) ? "Удалить" : "Delete";
  String statusOn = (sysConfig.language == LANG_RU) ? "ВКЛ" : "ON";
  String statusOff = (sysConfig.language == LANG_RU) ? "ВЫКЛ" : "OFF";
  String noRelaysText = (sysConfig.language == LANG_RU) ? "Реле не настроены" : "No relays configured";
  
  for (int i = 0; i < sysConfig.relayCount; i++) {
    if (sysConfig.relays[i].pin >= 0) {
      String statusClass = sysConfig.relays[i].state ? "status-online" : "status-offline";
      String statusText = sysConfig.relays[i].state ? statusOn : statusOff;
      
      html += "<div class='relay-item'>";
      html += "<div class='relay-header'>";
      html += "<span class='relay-name'>" + sysConfig.relays[i].name + " (GPIO" + String(sysConfig.relays[i].pin) + ")</span>";
      html += "<span class='relay-status'><span class='status-dot " + statusClass + "'></span>" + statusText + "</span>";
      html += "</div>";
      html += "<div class='relay-controls'>";
      html += "<button class='btn-small' onclick='changeRelayOrder(" + String(i) + ", -1)'>" + btnUp + "</button>";
      html += "<button class='btn-small' onclick='changeRelayOrder(" + String(i) + ", 1)'>" + btnDown + "</button>";
      html += "<button class='btn-small' onclick='toggleRelay(" + String(i) + ",true)'>" + btnOn + "</button>";
      html += "<button class='btn-small' onclick='toggleRelay(" + String(i) + ",false)'>" + btnOff + "</button>";
      html += "<button class='btn-small' style='border-color: #ff4444; color: #ff4444;' onclick='deleteRelay(" + String(i) + ")'>" + btnDelete + "</button>";
      html += "</div></div>";
    }
    yield();
  }
  
  if (sysConfig.relayCount == 0) html += "<p style='color: #888; font-size: 14px;'>" + noRelaysText + "</p>";
  
  return html;
}

/**
 * web.ino
 *
 * Defines Web API callback routes for HTTP form submissions.
 * Reads logic from HTML strings and performs parameter substitution on-the-fly.
 */

#include "config.h"
#include "web_html.h"

// ==========================
// TEXT CONSTANTS (i18n Localization)
// Stores language values dynamically swapped during HTML processing.
// ==========================

const char TXT_LANG_BTN_RU[] = "ENGLISH";
const char TXT_STATUS_TITLE_RU[] = "СТАТУС СИСТЕМЫ";
const char TXT_MODE_LABEL_RU[] = "Режим:";
const char TXT_WIFI_LABEL_RU[] = "WiFi сеть:";
const char TXT_ROUTERS_LABEL_RU[] = "Активные роутеры:";
const char TXT_ERRORS_LABEL_RU[] = "Ошибок:";
const char TXT_SCHEDULE_LABEL_RU[] = "Перезагрузка:";
const char TXT_WIFI_TITLE_RU[] = "WIFI";
const char TXT_WIFI_SSID_RU[] = "SSID сети:";
const char TXT_WIFI_PASS_RU[] = "Пароль:";
const char TXT_AP_TITLE_RU[] = "ТОЧКА ДОСТУПА";
const char TXT_AP_ENABLE_LABEL_RU[] = "Включить точку доступа";
const char TXT_AP_SSID_RU[] = "Имя точки доступа:";
const char TXT_AP_PASS_RU[] = "Пароль точки доступа:";
const char TXT_AUTO_AP_LABEL_RU[] = "Авто-AP при потере WiFi";
const char TXT_MONITOR_TITLE_RU[] = "МОНИТОРИНГ";
const char TXT_TARGET_RU[] = "Сайт для проверки:";
const char TXT_THRESH_RU[] = "Порог ошибок:";
const char TXT_INTERVAL_RU[] = "Интервал (сек):";
const char TXT_BOOTTIME_RU[] = "Время загрузки (сек):";
const char TXT_BOOTTIME_HELP_RU[] = "Время после включения до проверки";
const char TXT_REBOOT_DELAY_RU[] = "Время выключения (сек):";
const char TXT_REBOOT_DELAY_HELP_RU[] = "Как долго роутеры выключены";
const char TXT_SAVE_RU[] = "СОХРАНИТЬ";
const char TXT_SEQ_TITLE_RU[] = "ПОСЛЕДОВАТЕЛЬНОЕ ВКЛЮЧЕНИЕ";
const char TXT_SEQ_LABEL_RU[] = "Включить последовательное включение";
const char TXT_SEQ_DELAY_RU[] = "Задержка (сек):";
const char TXT_SEQ_DELAY_HELP_RU[] = "Задержка между роутерами";
const char TXT_SEQ_WARNING_RU[] = "<div class='warning-text'>Требуется минимум 2 реле!</div>";
const char TXT_SCHEDULE_TITLE_RU[] = "ВРЕМЯ ПЕРЕЗАГРУЗКИ";
const char TXT_SCHED_LABEL_RU[] = "Перезагрузить по времени";
const char TXT_HOUR_RU[] = "Часы:";
const char TXT_MIN_RU[] = "Минуты:";
const char TXT_RELAY_TITLE_RU[] = "УПРАВЛЕНИЕ РЕЛЕ";
const char TXT_ADD_RELAY_RU[] = "Добавить реле";
const char TXT_PIN_RU[] = "GPIO пин:";
const char TXT_NAME_RU[] = "Название:";
const char TXT_ADD_RU[] = "ДОБАВИТЬ РЕЛЕ";
const char TXT_ACTIONS_TITLE_RU[] = "СИСТЕМА";
const char TXT_MANUAL_REBOOT_BTN_RU[] = "ПЕРЕЗАГРУЗИТЬ ВСЕ РОУТЕРЫ";
const char TXT_PAUSE_RU[] = "ОСТАНОВИТЬ МОНИТОРИНГ";
const char TXT_REBOOT_ALERT_RU[] = "Начать перезагрузку роутеров?";
const char TXT_DELETE_CONFIRM_RU[] = "Удалить это реле?";
const char TXT_MODE_AP_RU[] = "Точка Доступа";
const char TXT_MODE_STATION_RU[] = "Станция";

const char TXT_LANG_BTN_EN[] = "RUSSIAN";
const char TXT_STATUS_TITLE_EN[] = "SYSTEM STATUS";
const char TXT_MODE_LABEL_EN[] = "Mode:";
const char TXT_WIFI_LABEL_EN[] = "WiFi Network:";
const char TXT_ROUTERS_LABEL_EN[] = "Active Routers:";
const char TXT_ERRORS_LABEL_EN[] = "Errors:";
const char TXT_SCHEDULE_LABEL_EN[] = "Reboot:";
const char TXT_WIFI_TITLE_EN[] = "WIFI";
const char TXT_WIFI_SSID_EN[] = "Network SSID:";
const char TXT_WIFI_PASS_EN[] = "Password:";
const char TXT_AP_TITLE_EN[] = "ACCESS POINT";
const char TXT_AP_ENABLE_LABEL_EN[] = "Enable Access Point";
const char TXT_AP_SSID_EN[] = "AP Name:";
const char TXT_AP_PASS_EN[] = "AP Password:";
const char TXT_AUTO_AP_LABEL_EN[] = "Auto-AP on WiFi loss";
const char TXT_MONITOR_TITLE_EN[] = "MONITORING";
const char TXT_TARGET_EN[] = "Check Target:";
const char TXT_THRESH_EN[] = "Fail Threshold:";
const char TXT_INTERVAL_EN[] = "Interval (sec):";
const char TXT_BOOTTIME_EN[] = "Boot Time (sec):";
const char TXT_BOOTTIME_HELP_EN[] = "Time to wait after boot";
const char TXT_REBOOT_DELAY_EN[] = "Off Time (sec):";
const char TXT_REBOOT_DELAY_HELP_EN[] = "How long routers are off";
const char TXT_SAVE_EN[] = "SAVE";
const char TXT_SEQ_TITLE_EN[] = "SEQUENTIAL";
const char TXT_SEQ_LABEL_EN[] = "Enable sequential boot";
const char TXT_SEQ_DELAY_EN[] = "Delay (sec):";
const char TXT_SEQ_DELAY_HELP_EN[] = "Delay between routers";
const char TXT_SEQ_WARNING_EN[] = "<div class='warning-text'>Minimum 2 relays required!</div>";
const char TXT_SCHEDULE_TITLE_EN[] = "REBOOT TIME";
const char TXT_SCHED_LABEL_EN[] = "Reboot by time";
const char TXT_HOUR_EN[] = "Hour:";
const char TXT_MIN_EN[] = "Minute:";
const char TXT_RELAY_TITLE_EN[] = "RELAYS CONTROL";
const char TXT_ADD_RELAY_EN[] = "Add Relay";
const char TXT_PIN_EN[] = "GPIO Pin:";
const char TXT_NAME_EN[] = "Name:";
const char TXT_ADD_EN[] = "ADD RELAY";
const char TXT_ACTIONS_TITLE_EN[] = "SYSTEM";
const char TXT_MANUAL_REBOOT_BTN_EN[] = "REBOOT ALL ROUTERS";
const char TXT_PAUSE_EN[] = "PAUSE MONITORING";
const char TXT_REBOOT_ALERT_EN[] = "Start rebooting all routers?";
const char TXT_DELETE_CONFIRM_EN[] = "Delete this relay?";
const char TXT_MODE_AP_EN[] = "Access Point";
const char TXT_MODE_STATION_EN[] = "Station";

// ==========================
// WEB SERVER CORE INITIALIZATION
// Declares URI route callbacks and binds them to specific controller functions.
// ==========================

void initWebServer() {
  server.on("/", handleRoot);
  server.on("/save_wifi", HTTP_POST, handleSaveWifi);
  server.on("/save_ap", HTTP_POST, handleSaveAP);
  server.on("/save_auto_ap", HTTP_POST, handleSaveAutoAP);
  server.on("/save_ping", HTTP_POST, handleSavePing);
  server.on("/save_sequential", HTTP_POST, handleSaveSequential);
  server.on("/save_schedule", HTTP_POST, handleSaveSchedule);
  server.on("/save_language", handleSaveLanguage);
  server.on("/manual_reboot", handleManualReboot);
  server.on("/status", handleStatus);
  server.on("/add_relay", HTTP_POST, handleAddRelay);
  server.on("/pause", handlePause);
  server.on("/resume", handleResume);
  server.on("/relay_order", handleRelayOrder);
  
  server.onNotFound([]() {
    String path = server.uri();
    if (path.startsWith("/relay/")) {
      handleRelayControl();
    } else if (path.startsWith("/delete_relay/")) {
      handleDeleteRelay();
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });
  Serial.println("[WEB] Server initialized");
}

// ==========================
// VIEW CONTROLLER (RENDERING MAIN DASHBOARD)
// Hydrates the HTML payload compressed in PROGMEM with state constraints.
// ==========================

void handleRoot() {
  String html = index_html;
  
  String wifiSsidInfo = "";
  if (sysState.inApMode) {
    wifiSsidInfo = (sysConfig.language == LANG_RU) ? "Режим точки доступа" : "AP Mode";
  } else if (sysState.wifiConnected) {
    wifiSsidInfo = sysConfig.wifiSSID;
  } else {
    wifiSsidInfo = (sysConfig.language == LANG_RU) ? "Не подключено" : "Not connected";
  }
  
  String modeInfo = sysState.inApMode ? 
    (sysConfig.language == LANG_RU ? TXT_MODE_AP_RU : TXT_MODE_AP_EN) : 
    (sysConfig.language == LANG_RU ? TXT_MODE_STATION_RU : TXT_MODE_STATION_EN);
  
  String activeRoutersInfo = getActiveRelaysString();
  String errorsInfo = String(sysState.failCount) + "/" + String(sysConfig.failThreshold);
  
  String scheduleInfo = "";
  if (sysConfig.scheduleEnabled) {
    scheduleInfo = String(sysConfig.scheduleHour) + ":" + String(sysConfig.scheduleMinute < 10 ? "0" : "") + String(sysConfig.scheduleMinute);
  } else {
    scheduleInfo = (sysConfig.language == LANG_RU) ? "Отключено" : "Disabled";
  }
  
  int totalWaitTime = sysConfig.rebootDelay + sysConfig.bootTime;
  String autoApTimeInfo = "";
  if (sysConfig.language == LANG_RU) {
    autoApTimeInfo = "Авто-AP включится если WiFi нет " + String(totalWaitTime) + " сек (выкл: " + String(sysConfig.rebootDelay) + "с + загрузка: " + String(sysConfig.bootTime) + "с)";
  } else {
    autoApTimeInfo = "Auto-AP activates if no WiFi for " + String(totalWaitTime) + "s (off: " + String(sysConfig.rebootDelay) + "s + boot: " + String(sysConfig.bootTime) + "s)";
  }
  
  String wifiStatusClass = (sysState.wifiConnected && !sysState.inApMode) ? "status-online" : "status-offline";
  String errorsClass = (sysState.failCount > 0) ? "error" : "";
  
  bool canUseSequential = (sysConfig.relayCount >= 2);
  String seqDisabledClass = canUseSequential ? "" : "disabled";
  String seqDisabledAttr = canUseSequential ? "" : "disabled";
  String seqWarning = canUseSequential ? "" : (sysConfig.language == LANG_RU ? TXT_SEQ_WARNING_RU : TXT_SEQ_WARNING_EN);

  if (sysConfig.language == LANG_RU) {
    html.replace("%LANG%", "0");
    html.replace("%LANG_BTN%", TXT_LANG_BTN_RU);
    html.replace("%STATUS_TITLE%", TXT_STATUS_TITLE_RU);
    html.replace("%MODE_LABEL%", TXT_MODE_LABEL_RU);
    html.replace("%MODE_INFO%", modeInfo);
    html.replace("%WIFI_LABEL%", TXT_WIFI_LABEL_RU);
    html.replace("%ROUTERS_LABEL%", TXT_ROUTERS_LABEL_RU);
    html.replace("%ERRORS_LABEL%", TXT_ERRORS_LABEL_RU);
    html.replace("%SCHEDULE_LABEL%", TXT_SCHEDULE_LABEL_RU);
    html.replace("%WIFI_STATUS_CLASS%", wifiStatusClass);
    html.replace("%WIFI_SSID_INFO%", wifiSsidInfo);
    html.replace("%ACTIVE_ROUTERS_INFO%", activeRoutersInfo);
    html.replace("%ERRORS_INFO%", errorsInfo);
    html.replace("%ERRORS_CLASS%", errorsClass);
    html.replace("%SCHEDULE_INFO%", scheduleInfo);
    html.replace("%WIFI_TITLE%", TXT_WIFI_TITLE_RU);
    html.replace("%WIFI_SSID_LABEL%", TXT_WIFI_SSID_RU);
    html.replace("%WIFI_PASS_LABEL%", TXT_WIFI_PASS_RU);
    html.replace("%AP_TITLE%", TXT_AP_TITLE_RU);
    html.replace("%AP_ENABLE_LABEL%", TXT_AP_ENABLE_LABEL_RU);
    html.replace("%AP_SSID_LABEL%", TXT_AP_SSID_RU);
    html.replace("%AP_PASS_LABEL%", TXT_AP_PASS_RU);
    html.replace("%AP_ENABLED%", sysConfig.apEnabled ? "checked" : "");
    html.replace("%AP_SSID%", sysConfig.apSSID);
    html.replace("%AP_PASS%", sysConfig.apPASS);
    html.replace("%AUTO_AP_LABEL%", TXT_AUTO_AP_LABEL_RU);
    html.replace("%AUTO_AP_TIME_INFO%", autoApTimeInfo);
    html.replace("%AUTO_AP_ENABLED%", sysConfig.autoApEnabled ? "checked" : "");
    html.replace("%MONITOR_TITLE%", TXT_MONITOR_TITLE_RU);
    html.replace("%TARGET_LABEL%", TXT_TARGET_RU);
    html.replace("%THRESH_LABEL%", TXT_THRESH_RU);
    html.replace("%INTERVAL_LABEL%", TXT_INTERVAL_RU);
    html.replace("%BOOTTIME_LABEL%", TXT_BOOTTIME_RU);
    html.replace("%BOOTTIME_HELP%", TXT_BOOTTIME_HELP_RU);
    html.replace("%REBOOT_DELAY_LABEL%", TXT_REBOOT_DELAY_RU);
    html.replace("%REBOOT_DELAY_HELP%", TXT_REBOOT_DELAY_HELP_RU);
    html.replace("%SAVE_BTN%", TXT_SAVE_RU);
    html.replace("%SEQ_TITLE%", TXT_SEQ_TITLE_RU);
    html.replace("%SEQ_LABEL%", TXT_SEQ_LABEL_RU);
    html.replace("%SEQ_DELAY_LABEL%", TXT_SEQ_DELAY_RU);
    html.replace("%SEQ_DELAY_HELP%", TXT_SEQ_DELAY_HELP_RU);
    html.replace("%SEQ_WARNING%", seqWarning);
    html.replace("%SEQ_DISABLED_CLASS%", seqDisabledClass);
    html.replace("%SEQ_DISABLED_ATTR%", seqDisabledAttr);
    html.replace("%SCHEDULE_TITLE%", TXT_SCHEDULE_TITLE_RU);
    html.replace("%SCHED_LABEL%", TXT_SCHED_LABEL_RU);
    html.replace("%HOUR_LABEL%", TXT_HOUR_RU);
    html.replace("%MIN_LABEL%", TXT_MIN_RU);
    html.replace("%RELAY_TITLE%", TXT_RELAY_TITLE_RU);
    html.replace("%ADD_RELAY_TITLE%", TXT_ADD_RELAY_RU);
    html.replace("%PIN_LABEL%", TXT_PIN_RU);
    html.replace("%NAME_LABEL%", TXT_NAME_RU);
    html.replace("%ADD_BTN%", TXT_ADD_RU);
    html.replace("%ACTIONS_TITLE%", TXT_ACTIONS_TITLE_RU);
    html.replace("%MANUAL_REBOOT_BTN%", TXT_MANUAL_REBOOT_BTN_RU);
    html.replace("%PAUSE_BTN%", TXT_PAUSE_RU);
    html.replace("%REBOOT_ALERT%", TXT_REBOOT_ALERT_RU);
    html.replace("%DELETE_CONFIRM%", TXT_DELETE_CONFIRM_RU);
  } else {
    html.replace("%LANG%", "1");
    html.replace("%LANG_BTN%", TXT_LANG_BTN_EN);
    html.replace("%STATUS_TITLE%", TXT_STATUS_TITLE_EN);
    html.replace("%MODE_LABEL%", TXT_MODE_LABEL_EN);
    html.replace("%MODE_INFO%", modeInfo);
    html.replace("%WIFI_LABEL%", TXT_WIFI_LABEL_EN);
    html.replace("%ROUTERS_LABEL%", TXT_ROUTERS_LABEL_EN);
    html.replace("%ERRORS_LABEL%", TXT_ERRORS_LABEL_EN);
    html.replace("%SCHEDULE_LABEL%", TXT_SCHEDULE_LABEL_EN);
    html.replace("%WIFI_STATUS_CLASS%", wifiStatusClass);
    html.replace("%WIFI_SSID_INFO%", wifiSsidInfo);
    html.replace("%ACTIVE_ROUTERS_INFO%", activeRoutersInfo);
    html.replace("%ERRORS_INFO%", errorsInfo);
    html.replace("%ERRORS_CLASS%", errorsClass);
    html.replace("%SCHEDULE_INFO%", scheduleInfo);
    html.replace("%WIFI_TITLE%", TXT_WIFI_TITLE_EN);
    html.replace("%WIFI_SSID_LABEL%", TXT_WIFI_SSID_EN);
    html.replace("%WIFI_PASS_LABEL%", TXT_WIFI_PASS_EN);
    html.replace("%AP_TITLE%", TXT_AP_TITLE_EN);
    html.replace("%AP_ENABLE_LABEL%", TXT_AP_ENABLE_LABEL_EN);
    html.replace("%AP_SSID_LABEL%", TXT_AP_SSID_EN);
    html.replace("%AP_PASS_LABEL%", TXT_AP_PASS_EN);
    html.replace("%AP_ENABLED%", sysConfig.apEnabled ? "checked" : "");
    html.replace("%AP_SSID%", sysConfig.apSSID);
    html.replace("%AP_PASS%", sysConfig.apPASS);
    html.replace("%AUTO_AP_LABEL%", TXT_AUTO_AP_LABEL_EN);
    html.replace("%AUTO_AP_TIME_INFO%", autoApTimeInfo);
    html.replace("%AUTO_AP_ENABLED%", sysConfig.autoApEnabled ? "checked" : "");
    html.replace("%MONITOR_TITLE%", TXT_MONITOR_TITLE_EN);
    html.replace("%TARGET_LABEL%", TXT_TARGET_EN);
    html.replace("%THRESH_LABEL%", TXT_THRESH_EN);
    html.replace("%INTERVAL_LABEL%", TXT_INTERVAL_EN);
    html.replace("%BOOTTIME_LABEL%", TXT_BOOTTIME_EN);
    html.replace("%BOOTTIME_HELP%", TXT_BOOTTIME_HELP_EN);
    html.replace("%REBOOT_DELAY_LABEL%", TXT_REBOOT_DELAY_EN);
    html.replace("%REBOOT_DELAY_HELP%", TXT_REBOOT_DELAY_HELP_EN);
    html.replace("%SAVE_BTN%", TXT_SAVE_EN);
    html.replace("%SEQ_TITLE%", TXT_SEQ_TITLE_EN);
    html.replace("%SEQ_LABEL%", TXT_SEQ_LABEL_EN);
    html.replace("%SEQ_DELAY_LABEL%", TXT_SEQ_DELAY_EN);
    html.replace("%SEQ_DELAY_HELP%", TXT_SEQ_DELAY_HELP_EN);
    html.replace("%SEQ_WARNING%", seqWarning);
    html.replace("%SEQ_DISABLED_CLASS%", seqDisabledClass);
    html.replace("%SEQ_DISABLED_ATTR%", seqDisabledAttr);
    html.replace("%SCHEDULE_TITLE%", TXT_SCHEDULE_TITLE_EN);
    html.replace("%SCHED_LABEL%", TXT_SCHED_LABEL_EN);
    html.replace("%HOUR_LABEL%", TXT_HOUR_EN);
    html.replace("%MIN_LABEL%", TXT_MIN_EN);
    html.replace("%RELAY_TITLE%", TXT_RELAY_TITLE_EN);
    html.replace("%ADD_RELAY_TITLE%", TXT_ADD_RELAY_EN);
    html.replace("%PIN_LABEL%", TXT_PIN_EN);
    html.replace("%NAME_LABEL%", TXT_NAME_EN);
    html.replace("%ADD_BTN%", TXT_ADD_EN);
    html.replace("%ACTIONS_TITLE%", TXT_ACTIONS_TITLE_EN);
    html.replace("%MANUAL_REBOOT_BTN%", TXT_MANUAL_REBOOT_BTN_EN);
    html.replace("%PAUSE_BTN%", TXT_PAUSE_EN);
    html.replace("%REBOOT_ALERT%", TXT_REBOOT_ALERT_EN);
    html.replace("%DELETE_CONFIRM%", TXT_DELETE_CONFIRM_EN);
  }
  
  html.replace("%SSID%", sysConfig.wifiSSID);
  html.replace("%PASS%", sysConfig.wifiPASS);
  html.replace("%TARGET%", sysConfig.pingTarget);
  html.replace("%THRESH%", String(sysConfig.failThreshold));
  html.replace("%INTERVAL%", String(sysConfig.checkInterval));
  html.replace("%BOOT_TIME%", String(sysConfig.bootTime));
  html.replace("%REBOOT_DELAY%", String(sysConfig.rebootDelay));
  html.replace("%SEQ_CHECKED%", sysConfig.sequentialEnabled ? "checked" : "");
  html.replace("%SEQ_DELAY%", String(sysConfig.relayDelay));
  html.replace("%SCHED_CHECKED%", sysConfig.scheduleEnabled ? "checked" : "");
  html.replace("%SCHED_HOUR%", String(sysConfig.scheduleHour));
  html.replace("%SCHED_MIN%", String(sysConfig.scheduleMinute));
  html.replace("%RELAY_COUNT%", String(sysConfig.relayCount));
  html.replace("%RELAYS_HTML%", generateRelaysHTML());
  
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", html);
}

// ==========================
// FORM SUBMISSION HANDLERS
// Validates standard HTTP POST workflows triggered by web-dashboard clicks.
// Typically modifies 'sysConfig' locally and triggers continuous 'saveSettings()'.
// ==========================

void handleSaveAP() {
  if (server.hasArg("ap_ssid") && server.hasArg("ap_pass")) {
    sysConfig.apEnabled = server.hasArg("enabled");
    sysConfig.autoApEnabled = (server.arg("auto_ap") == "on");
    sysConfig.apSSID = server.arg("ap_ssid");
    sysConfig.apPASS = server.arg("ap_pass");
    saveSettings();
    Serial.printf("[WEB] AP settings saved: %s, enabled=%d, auto=%d\n", sysConfig.apSSID.c_str(), sysConfig.apEnabled, sysConfig.autoApEnabled);
    if (sysConfig.apEnabled) {
      Serial.println("[WEB] AP enabled, switching to AP+STA mode...");
      sysState.inApMode = true;
      sysState.wifiConnected = false;
      WiFi.mode(WIFI_AP_STA);
      delay(100);
      WiFi.softAP(sysConfig.apSSID.c_str(), sysConfig.apPASS.c_str());
      // Keep trying to connect to configured WiFi if credentials exist
      if (sysConfig.wifiSSID != "" && sysConfig.wifiSSID.length() >= 3) {
        WiFi.begin(sysConfig.wifiSSID.c_str(), sysConfig.wifiPASS.c_str());
      }
    } else {
      Serial.println("[WEB] AP disabled, connecting to WiFi...");
      sysState.inApMode = false;
      WiFi.softAPdisconnect(true);
      delay(100);
      connectToWiFi();
    }
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleSaveAutoAP() {
  if (server.hasArg("auto_ap")) {
    sysConfig.autoApEnabled = server.hasArg("auto_ap");
    saveSettings();
    Serial.printf("[WEB] Auto-AP saved: %d\n", sysConfig.autoApEnabled);
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleSaveWifi() {
  if (server.hasArg("ssid") && server.hasArg("pass")) {
    sysConfig.wifiSSID = server.arg("ssid");
    sysConfig.wifiPASS = server.arg("pass");
    saveSettings();
    Serial.printf("[WEB] WiFi settings saved: %s\n", sysConfig.wifiSSID.c_str());
    if (sysConfig.wifiSSID != "") {
      Serial.println("[WEB] WiFi configured, disabling AP...");
      sysConfig.apEnabled = false;
      saveSettings();
      WiFi.softAPdisconnect(true);
      delay(100);
      server.sendHeader("Location", "/");
      server.send(302);
      delay(1000);
      ESP.restart();
    }
  }
}

void handleStatus() {
  String status = "";
  if (sysState.inApMode) {
    status += "AP Mode<br>192.168.4.1<br>";
  } else if (sysState.wifiConnected) {
    status += "Station Mode<br>" + WiFi.localIP().toString() + "<br>";
  } else {
    status += "Connecting...<br>";
  }
  status += getCurrentTime() + "<br>";
  status += "Errors: " + String(sysState.failCount) + "/" + String(sysConfig.failThreshold);
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", status);
}

void handleSaveLanguage() {
  if (server.hasArg("lang")) {
    sysConfig.language = constrain(server.arg("lang").toInt(), 0, 1);
    saveSettings();
    Serial.printf("[WEB] Language saved: %d\n", sysConfig.language);
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleRelayOrder() {
  if (server.hasArg("id") && server.hasArg("dir")) {
    int id = server.arg("id").toInt();
    int dir = server.arg("dir").toInt();
    if (id >= 0 && id < sysConfig.relayCount) {
      int newOrder = sysConfig.relays[id].order + dir;
      if (newOrder >= 0 && newOrder < sysConfig.relayCount) {
        for (int i = 0; i < sysConfig.relayCount; i++) {
          if (sysConfig.relays[i].order == newOrder) {
            sysConfig.relays[i].order = sysConfig.relays[id].order;
            break;
          }
        }
        sysConfig.relays[id].order = newOrder;
        saveSettings();
        sortRelaysByOrder();
      }
    }
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleSavePing() {
  if (server.hasArg("target")) {
    sysConfig.pingTarget = server.arg("target");
    sysConfig.failThreshold = server.arg("threshold").toInt();
    sysConfig.checkInterval = constrain(server.arg("interval").toInt(), 10, 300);
    sysConfig.bootTime = constrain(server.arg("boot_time").toInt(), 30, 300);
    sysConfig.rebootDelay = constrain(server.arg("reboot_delay").toInt(), 5, 120);
    saveSettings();
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleSaveSequential() {
  sysConfig.sequentialEnabled = (sysConfig.relayCount < 2) ? false : server.hasArg("enabled");
  sysConfig.relayDelay = constrain(server.arg("delay").toInt(), 5, 120);
  saveSettings();
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleSaveSchedule() {
  sysConfig.scheduleEnabled = server.hasArg("enabled");
  sysConfig.scheduleHour = constrain(server.arg("hour").toInt(), 0, 23);
  sysConfig.scheduleMinute = constrain(server.arg("minute").toInt(), 0, 59);
  saveSettings();
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleManualReboot() {
  triggerReboot();
  server.sendHeader("Location", "/");
  server.send(302);
}

void handlePause() {
  sysState.monitoringPaused = true;
  server.send(200, "text/plain", "OK");
}

void handleResume() {
  sysState.monitoringPaused = false;
  sysState.failCount = 0;
  server.send(200, "text/plain", "OK");
}

// ==========================
// RELAY MANAGEMENT ENDPOINTS
// Adds, edits or removes dynamic physical routing endpoints mapping to JSON arrays.
// ==========================

void handleAddRelay() {
  if (server.hasArg("pin") && server.hasArg("name")) {
    int pin = server.arg("pin").toInt();
    String name = server.arg("name");
    if (sysConfig.relayCount < MAX_RELAYS && pin >= 0 && pin <= 9) {
      bool pinUsed = false;
      for (int i = 0; i < sysConfig.relayCount; i++) {
        if (sysConfig.relays[i].pin == pin) { pinUsed = true; break; }
      }
      if (!pinUsed) {
        sysConfig.relays[sysConfig.relayCount].pin = pin;
        sysConfig.relays[sysConfig.relayCount].name = name;
        sysConfig.relays[sysConfig.relayCount].enabled = true;
        sysConfig.relays[sysConfig.relayCount].state = true;
        sysConfig.relays[sysConfig.relayCount].order = sysConfig.relayCount;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        sysConfig.relayCount++;
        
        saveSettings();
        
        preferences.begin("router-watch", false);
        String key = "relay_" + String(sysConfig.relayCount - 1);
        preferences.putInt((key + "_pin").c_str(), pin);
        preferences.putString((key + "_name").c_str(), name);
        preferences.putInt((key + "_order").c_str(), sysConfig.relayCount - 1);
        preferences.end();
      }
    }
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleDeleteRelay() {
  String path = server.uri();
  int id = path.startsWith("/delete_relay/") ? path.substring(14).toInt() : -1;
  if (id >= 0 && id < sysConfig.relayCount) {
    for (int i = id; i < sysConfig.relayCount - 1; i++) {
        sysConfig.relays[i] = sysConfig.relays[i + 1];
    }
    sysConfig.relays[sysConfig.relayCount - 1] = {-1, "", false, false, -1};
    sysConfig.relayCount--;
    saveSettings(); 
    
    preferences.begin("router-watch", false);
    for (int i = id; i < sysConfig.relayCount; i++) {
      String keyTo = "relay_" + String(i);
      preferences.putInt((keyTo + "_pin").c_str(), sysConfig.relays[i].pin);
      preferences.putString((keyTo + "_name").c_str(), sysConfig.relays[i].name);
      preferences.putInt((keyTo + "_order").c_str(), sysConfig.relays[i].order);
    }
    String keyFrom = "relay_" + String(sysConfig.relayCount);
    preferences.remove((keyFrom + "_pin").c_str());
    preferences.remove((keyFrom + "_name").c_str());
    preferences.remove((keyFrom + "_order").c_str());
    preferences.end();
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleRelayControl() {
  String path = server.uri();
  int firstSlash = 7;
  int secondSlash = path.indexOf('/', firstSlash + 1);
  if (secondSlash > 0) {
    String idStr = path.substring(firstSlash, secondSlash);
    String action = path.substring(secondSlash + 1);
    int id = idStr.toInt();
    if (id >= 0 && id < sysConfig.relayCount) {
      bool state = (action == "on");
      setRelay(id, state);
      server.send(200, "text/plain", "OK");
      return;
    }
  }
  server.send(400, "text/plain", "Invalid request");
}

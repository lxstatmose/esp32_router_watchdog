#ifndef WEB_HTML_H
#define WEB_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Router Watchdog.</title>
  <style>
    :root {
      --bg-color: #000000;
      --panel-bg: #050505;
      --input-bg: #0a0a0a;
      --border-color: #1f1f1f;
      --text-main: #e5e5e5;
      --text-dim: #737373;
      --accent-target: #00e5ff; /* Neon Cyan */
      --accent-green: #00ffa3;
      --accent-red: #ff3333;
      --accent-warn: #eab308;
      
      --font-sans: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      --font-mono: ui-monospace, "SF Mono", Consolas, "Liberation Mono", Menlo, Courier, monospace;
    }

    * { margin: 0; padding: 0; box-sizing: border-box; }
    
    body {
      font-family: var(--font-mono);
      background: var(--bg-color);
      color: var(--text-main);
      padding: 40px;
      max-width: 1600px;
      margin: 0 auto;
      font-size: 13px;
      -webkit-font-smoothing: antialiased;
    }

    .header-section {
      margin-bottom: 40px;
      display: flex;
      justify-content: space-between;
      align-items: flex-end;
      border-bottom: 1px solid var(--border-color);
      padding-bottom: 32px;
    }

    .logo-container {
      display: flex;
      flex-direction: column;
      gap: 16px;
    }

    .tagline {
      display: flex;
      align-items: center;
      gap: 10px;
      color: var(--accent-target);
      font-size: 10px;
      text-transform: uppercase;
      letter-spacing: 2px;
    }

    .tagline::before {
      content: '';
      display: inline-block;
      width: 6px;
      height: 6px;
      background: var(--accent-target);
    }

    .logo {
      font-family: var(--font-sans);
      font-size: 56px;
      font-weight: 800;
      color: #fff;
      letter-spacing: -2px;
      line-height: 1;
    }

    .grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(420px, 1fr));
      border-top: 1px solid var(--border-color);
      border-left: 1px solid var(--border-color);
    }
    
    .panel {
      background: var(--panel-bg);
      display: flex;
      flex-direction: column;
      border-right: 1px solid var(--border-color);
      border-bottom: 1px solid var(--border-color);
    }
    
    .panel-header {
      font-size: 11px;
      color: var(--text-dim);
      padding: 16px 24px;
      border-bottom: 1px solid var(--border-color);
      display: flex;
      justify-content: space-between;
      align-items: center;
      text-transform: uppercase;
      letter-spacing: 2px;
    }

    .panel-title {
      color: var(--text-main);
    }
    
    .panel-body {
      padding: 32px;
      flex-grow: 1;
      display: flex;
      flex-direction: column;
    }
    
    .status-dot {
      width: 8px;
      height: 8px;
    }
    .status-online { background: var(--accent-green); box-shadow: 0 0 8px rgba(0,255,163,0.4); }
    .status-offline { background: var(--accent-red); box-shadow: 0 0 8px rgba(255,51,51,0.4); }

    .info-line {
      display: flex;
      justify-content: space-between;
      padding: 12px 0;
      border-bottom: 1px dashed var(--border-color);
    }
    .info-line:last-child { border-bottom: none; }
    .info-label { color: var(--text-dim); font-size: 11px; text-transform: uppercase; letter-spacing: 1px; }
    .info-value { color: var(--text-main); font-weight: bold; }
    .info-value.error { color: var(--accent-red); }
    
    .form-group { margin-bottom: 24px; }
    .form-group:last-of-type { margin-bottom: 32px; }
    
    .form-label {
      display: block;
      color: var(--text-dim);
      font-size: 10px;
      text-transform: uppercase;
      letter-spacing: 1.5px;
      margin-bottom: 10px;
    }
    
    .form-input {
      width: 100%;
      background: var(--input-bg);
      border: 1px solid var(--border-color);
      color: #fff;
      padding: 14px;
      font-family: inherit;
      font-size: 13px;
      outline: none;
      transition: all 0.2s;
    }
    
    .form-input:focus {
      border-color: var(--accent-target);
      background: #000;
      box-shadow: inset 2px 0 0 var(--accent-target);
    }

    .btn, .lang-btn {
      background: transparent;
      border: 1px solid var(--border-color);
      color: var(--text-main);
      padding: 14px 24px;
      font-family: inherit;
      font-size: 11px;
      font-weight: bold;
      text-transform: uppercase;
      letter-spacing: 1.5px;
      cursor: pointer;
      transition: all 0.2s;
      width: 100%;
      display: flex;
      justify-content: center;
      align-items: center;
      gap: 10px;
    }
    
    .lang-btn { width: 220px; align-self: flex-end; }
    
    .btn:hover:not(:disabled), .lang-btn:hover:not(:disabled) {
      background: var(--border-color);
    }
    
    .btn.success {
      background: #fff;
      color: #000;
      border-color: #fff;
      margin-top: auto;
    }
    .btn.success:hover:not(:disabled) {
      background: #e5e5e5;
      border-color: #e5e5e5;
    }
    
    .btn.danger {
      color: var(--accent-red);
      border-color: var(--border-color);
      background: #000;
    }
    .btn.danger:hover:not(:disabled) {
      background: var(--accent-red);
      border-color: var(--accent-red);
      color: #000;
    }

    .btn-small {
      padding: 8px 12px;
      background: #000;
      border: 1px solid var(--border-color);
      color: var(--text-dim);
      font-family: inherit;
      font-size: 10px;
      text-transform: uppercase;
      letter-spacing: 1px;
      cursor: pointer;
      transition: all 0.2s;
    }
    .btn-small:hover {
      color: #fff;
      background: var(--border-color);
    }
    
    .checkbox-row {
      display: flex;
      align-items: center;
      gap: 12px;
      padding: 14px 16px;
      background: var(--input-bg);
      border: 1px solid var(--border-color);
      cursor: pointer;
      margin-bottom: 24px;
      transition: all 0.2s;
    }
    .checkbox-row:hover { border-color: var(--text-dim); }
    .checkbox-row.disabled { opacity: 0.5; pointer-events: none; }
    
    .checkbox-row input {
      accent-color: var(--accent-target);
      width: 14px;
      height: 14px;
    }
    .checkbox-row span {
      font-size: 11px;
      text-transform: uppercase;
      letter-spacing: 1px;
    }

    .relay-item {
      border: 1px solid var(--border-color);
      margin-bottom: 16px;
      background: var(--input-bg);
    }
    .relay-item:last-child { margin-bottom: 0; }
    .relay-header {
      padding: 14px 16px;
      display: flex;
      justify-content: space-between;
      border-bottom: 1px solid var(--border-color);
      align-items: center;
    }
    .relay-name {
      font-weight: bold;
      color: var(--accent-target);
      font-size: 11px;
      text-transform: uppercase;
      letter-spacing: 1px;
    }
    .relay-status {
      display: flex;
      align-items: center;
      gap: 8px;
      font-size: 10px;
      text-transform: uppercase;
      letter-spacing: 1px;
      color: var(--text-dim);
    }
    .relay-controls {
      padding: 12px 16px;
      display: flex;
      gap: 8px;
      flex-wrap: wrap;
    }

    .settings-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 16px;
    }

    .help-text {
      color: var(--text-dim);
      font-size: 10px;
      margin-top: 8px;
    }
    .warning-text {
      color: var(--accent-warn);
      font-size: 10px;
      margin-top: 8px;
      text-transform: uppercase;
      letter-spacing: 1px;
    }
    
    .relay-add-section {
      margin-top: 32px;
      padding-top: 32px;
      border-top: 1px dashed var(--border-color);
    }

    @media (max-width: 900px) {
      .grid { grid-template-columns: 1fr; }
      body { padding: 16px; }
      .header-section { flex-direction: column; align-items: flex-start; gap: 24px; }
      .logo { font-size: 40px; }
    }
  </style>
</head>
<body>
  <div class="header-section">
    <div class="logo-container">
      <div class="tagline">ESP32 Device</div>
      <div class="logo">Router Watchdog.</div>
    </div>
    <button class="lang-btn" onclick="toggleLanguage()" aria-label="Toggle Language">%LANG_BTN%</button>
  </div>
  
  <main class="grid">
    <!-- Status Panel -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%STATUS_TITLE%</span>
        <span class="status-dot %WIFI_STATUS_CLASS%"></span>
      </div>
      <div class="panel-body">
        <div class="info-line">
          <span class="info-label">%MODE_LABEL%</span>
          <span class="info-value">%MODE_INFO%</span>
        </div>
        <div class="info-line">
          <span class="info-label">%WIFI_LABEL%</span>
          <span class="info-value">%WIFI_SSID_INFO%</span>
        </div>
        <div class="info-line">
          <span class="info-label">%ROUTERS_LABEL%</span>
          <span class="info-value" style="color: var(--accent-target);">%ACTIVE_ROUTERS_INFO%</span>
        </div>
        <div class="info-line">
          <span class="info-label">%ERRORS_LABEL%</span>
          <span class="info-value %ERRORS_CLASS%">%ERRORS_INFO%</span>
        </div>
        <div class="info-line">
          <span class="info-label">%SCHEDULE_LABEL%</span>
          <span class="info-value">%SCHEDULE_INFO%</span>
        </div>
      </div>
    </section>
    
    <!-- WiFi Configuration -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%WIFI_TITLE%</span>
      </div>
      <div class="panel-body">
        <form action="/save_wifi" method="POST" style="display:flex; flex-direction:column; height:100%;">
          <div class="form-group">
            <label class="form-label">%WIFI_SSID_LABEL%</label>
            <input type="text" name="ssid" class="form-input" value="%SSID%">
          </div>
          <div class="form-group">
            <label class="form-label">%WIFI_PASS_LABEL%</label>
            <input type="password" name="pass" class="form-input" value="%PASS%">
          </div>
          <button type="submit" class="btn success">%SAVE_BTN% ↗</button>
        </form>
      </div>
    </section>
    
    <!-- Access Point Configuration -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%AP_TITLE%</span>
      </div>
      <div class="panel-body">
        <form action="/save_ap" method="POST" style="display:flex; flex-direction:column; height:100%;">
          <label class="checkbox-row">
            <input type="checkbox" name="enabled" %AP_ENABLED%>
            <span>%AP_ENABLE_LABEL%</span>
          </label>
          <div class="form-group">
            <label class="form-label">%AP_SSID_LABEL%</label>
            <input type="text" name="ap_ssid" class="form-input" value="%AP_SSID%">
          </div>
          <div class="form-group">
            <label class="form-label">%AP_PASS_LABEL%</label>
            <input type="password" name="ap_pass" class="form-input" value="%AP_PASS%">
          </div>
          <label class="checkbox-row" style="margin-bottom:8px;">
            <input type="checkbox" name="auto_ap" %AUTO_AP_ENABLED%>
            <span>%AUTO_AP_LABEL%</span>
          </label>
          <div class="help-text" style="margin-bottom: 24px;">%AUTO_AP_TIME_INFO%</div>
          <button type="submit" class="btn success">%SAVE_BTN% ↗</button>
        </form>
      </div>
    </section>
    
    <!-- Monitoring Settings -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%MONITOR_TITLE%</span>
      </div>
      <div class="panel-body">
        <form action="/save_ping" method="POST" style="display:flex; flex-direction:column; height:100%;">
          <div class="form-group">
            <label class="form-label">%TARGET_LABEL%</label>
            <input type="text" name="target" class="form-input" value="%TARGET%">
          </div>
          <div class="settings-grid">
            <div class="form-group">
              <label class="form-label">%THRESH_LABEL%</label>
              <input type="number" name="threshold" class="form-input" value="%THRESH%" min="1">
            </div>
            <div class="form-group">
              <label class="form-label">%INTERVAL_LABEL%</label>
              <input type="number" name="interval" class="form-input" value="%INTERVAL%" min="10" max="300">
            </div>
          </div>
          <div class="settings-grid">
            <div class="form-group">
              <label class="form-label">%BOOTTIME_LABEL%</label>
              <input type="number" name="boot_time" class="form-input" value="%BOOT_TIME%" min="30" max="300">
              <div class="help-text">%BOOTTIME_HELP%</div>
            </div>
            <div class="form-group">
              <label class="form-label">%REBOOT_DELAY_LABEL%</label>
              <input type="number" name="reboot_delay" class="form-input" value="%REBOOT_DELAY%" min="5" max="120">
              <div class="help-text">%REBOOT_DELAY_HELP%</div>
            </div>
          </div>
          <button type="submit" class="btn success">%SAVE_BTN% ↗</button>
        </form>
      </div>
    </section>
    
    <!-- Sequential Boot Settings -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%SEQ_TITLE%</span>
      </div>
      <div class="panel-body">
        <form action="/save_sequential" method="POST" style="display:flex; flex-direction:column; height:100%;">
          <label class="checkbox-row %SEQ_DISABLED_CLASS%">
            <input type="checkbox" name="enabled" %SEQ_CHECKED% %SEQ_DISABLED_ATTR%>
            <span>%SEQ_LABEL%</span>
          </label>
          <div class="form-group">
            <label class="form-label">%SEQ_DELAY_LABEL%</label>
            <input type="number" name="delay" class="form-input" value="%SEQ_DELAY%" min="5" max="120">
            <div class="help-text">%SEQ_DELAY_HELP%</div>
            %SEQ_WARNING%
          </div>
          <button type="submit" class="btn success" %SEQ_DISABLED_ATTR%>%SAVE_BTN% ↗</button>
        </form>
      </div>
    </section>
    
    <!-- Schedule Settings -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%SCHEDULE_TITLE%</span>
      </div>
      <div class="panel-body">
        <form action="/save_schedule" method="POST" style="display:flex; flex-direction:column; height:100%;">
          <label class="checkbox-row">
            <input type="checkbox" name="enabled" %SCHED_CHECKED%>
            <span>%SCHED_LABEL%</span>
          </label>
          <div class="settings-grid">
            <div class="form-group">
              <label class="form-label">%HOUR_LABEL%</label>
              <input type="number" name="hour" class="form-input" value="%SCHED_HOUR%" min="0" max="23">
            </div>
            <div class="form-group">
              <label class="form-label">%MIN_LABEL%</label>
              <input type="number" name="minute" class="form-input" value="%SCHED_MIN%" min="0" max="59">
            </div>
          </div>
          <button type="submit" class="btn success">%SAVE_BTN% ↗</button>
        </form>
      </div>
    </section>
    
    <!-- Relays Control -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%RELAY_TITLE% (%RELAY_COUNT%)</span>
      </div>
      <div class="panel-body">
        <div id="relaysContainer">%RELAYS_HTML%</div>
        
        <div class="relay-add-section">
          <div class="form-group">
            <label class="form-label">%PIN_LABEL%</label>
            <input type="number" id="newRelayPin" class="form-input" min="0" max="99">
          </div>
          <div class="form-group">
            <label class="form-label">%NAME_LABEL%</label>
            <input type="text" id="newRelayName" class="form-input">
          </div>
          <button onclick="addRelay()" class="btn" style="border-color:var(--accent-target); color:var(--accent-target);">%ADD_BTN% ↗</button>
        </div>
      </div>
    </section>
    
    <!-- System Actions -->
    <section class="panel">
      <div class="panel-header">
        <span class="panel-title">%ACTIONS_TITLE%</span>
      </div>
      <div class="panel-body">
        <div class="actions-section" style="display:flex; flex-direction:column; gap:16px;">
          <button class="btn danger" onclick="manualReboot()">%MANUAL_REBOOT_BTN%</button>
          <button class="btn" onclick="togglePause()" id="pauseBtn">%PAUSE_BTN%</button>
        </div>
      </div>
    </section>
  </main>
  
  <script>
    const currentLang = parseInt('%LANG%', 10);
    
    async function apiCall(endpoint, options = {}) {
      try {
        const response = await fetch(endpoint, options);
        if(!response.ok) throw new Error('API Request Failed');
        location.reload();
      } catch (error) {
        console.error('Request detailed error:', error);
        alert(currentLang === 0 
          ? 'Ошибка связи с устройством. Проверьте подключение.' 
          : 'Communication error. Please check connection.');
      }
    }

    function toggleLanguage() {
      apiCall(`/save_language?lang=${currentLang === 0 ? 1 : 0}`);
    }
    
    function toggleRelay(id, state) {
      apiCall(`/relay/${id}/${state ? 'on' : 'off'}`);
    }
    
    function deleteRelay(id) {
      if(confirm('%DELETE_CONFIRM%')) {
        apiCall(`/delete_relay/${id}`, { method: 'POST' });
      }
    }
    
    function changeRelayOrder(id, direction) {
      apiCall(`/relay_order?id=${id}&dir=${direction}`);
    }
    
    function addRelay() {
      const pin = document.getElementById('newRelayPin').value.trim();
      const name = document.getElementById('newRelayName').value.trim();
      
      if (!pin || !name) {
        alert(currentLang === 0 ? 'Заполните пин и название!' : 'Please fill out PIN and name!');
        return;
      }
      
      const formData = new FormData();
      formData.append('pin', pin);
      formData.append('name', name);
      apiCall('/add_relay', { method: 'POST', body: formData });
    }
    
    function manualReboot() {
      if(confirm('%REBOOT_ALERT%')) {
        apiCall('/manual_reboot');
      }
    }
    
    function togglePause() {
      apiCall('/pause');
    }

    document.querySelectorAll('form').forEach(form => {
      form.addEventListener('submit', (e) => {
        const btn = form.querySelector('button[type="submit"]');
        if(btn) {
          btn.disabled = true;
          // btn.innerText = currentLang === 0 ? 'СОХРАНЕНИЕ...' : 'SAVING...';
        }
      });
    });
  </script>
</body>
</html>
)rawliteral";

#endif

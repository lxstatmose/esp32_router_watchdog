# ESP32 Router Watchdog 🌐📡

*[Читать на русском (Russian)](README.md)*

An advanced, highly reliable hardware watchdog for routers, modems, and networking equipment, powered by **ESP32** (fully optimized for ESP32-C3, ESP32-S3, and classic ESP32 boards).

## What it does
Networking equipment sometimes freezes or drops the connection. This project actively pings an internet target (like `google.com`) and automatically **power-cycles your networking hardware** via physical relays if the connection drops for too long. 

It comes with a beautiful, industrial-themed Dark Web UI to configure rules completely wirelessly.

## Features ✨
- 🖥️ **Slick Web Interface (Cyberpunk Theme)**: Monospace fonts, flat grid layouts, strict 1px borders, and neon accents. Hosted natively on the ESP32.
- 🔄 **Sequential Boot**: If you have multiple routers/switches, you can delay their boot sequentially. This prevents electrical spikes and allows modems to establish a DSL/Fiber line before the router requests an IP.
- 🕒 **Scheduled Reboot**: Force a clean reboot at a specific time every day (e.g., 03:30 AM).
- 🆘 **Auto-AP Fallback**: If the ESP strictly can't connect to your Wi-Fi, it will broadcast its own "Access Point" so you can connect directly from your phone and fix the settings.
- 💾 **Flash Memory Persistence**: All settings are safely saved into the ESP32 NVS via the `Preferences` library and reliably loaded upon boot.
- 🇬🇧🇷🇺 **Bilingual UI**: Switch seamlessly between English and Russian languages in the Web Interface.

---

## 🚀 How to Install & Compile

1. Open `ESP32_Router_Watchdog.ino` in your **Arduino IDE**.
2. Make sure you have the [ESP32 Core]installed in your Boards Manager.
3. Connect your ESP32 board and select it in `Tools -> Board`.
4. Compile and Upload!

### Initial Setup
1. On the very first boot (or if it can't find your Wi-Fi), the ESP32 will host an Access Point (e.g., `RouterWatchdog-AP`).
2. Connect to this Wi-Fi network using your phone or laptop.
3. Open a browser and navigate to `http://192.168.4.1`.
4. Open the Web Interface, select your home Wi-Fi network, enter the password, and hit **SAVE**.
5. The board will reboot, connect to your home network, and start protecting your connection!

## Hardware Requirements
- **ESP32 Microcontroller** (ESP32-C3 tested and highly recommended).
- **5V Relay Module** (1-channel, 2-channel, 4-channel depending on how many devices you want to control).
- Wires.

## Wiring Guide
- Connect the **VCC** of the relay module to the **5V/VIN** pin of the ESP32.
- Connect the **GND** of the relay module to **GND**.
- Connect the **IN1 / IN2** pins to any free **GPIO** pins on your ESP32 (e.g., GPIO4, GPIO5). You can map these dynamically inside the Web Interface!

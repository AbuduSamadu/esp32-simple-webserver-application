#pragma once

// =========================================================
// APPLICATION CONFIGURATION
// Keep all hardware pins, credentials, and settings here
// to decouple them from the application logic.
// =========================================================

namespace Config {
    // Wi-Fi Access Point Settings
    const char* const AP_SSID = "ESP32-Dashboard";
    const char* const AP_PASSWORD = "password123";

    // Hardware Settings
    const int LED_PIN = 2; // Built-in LED on GPIO 2

    // Network Ports
    const int HTTP_PORT = 80;
    const int DNS_PORT = 53;
}

#include "DashboardServer.h"
#include "Config.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Initialize Server and DNS instances
static AsyncWebServer server(Config::HTTP_PORT);
static DNSServer dnsServer;
static bool ledState = false;

void DashboardServer::begin() {
    // 1. Initialize Hardware
    pinMode(Config::LED_PIN, OUTPUT);
    digitalWrite(Config::LED_PIN, LOW);

    // 2. Initialize Subsystems
    setupFS();
    setupWiFi();
    setupDNS();
    setupServer();
}

void DashboardServer::setupFS() {
    if (!LittleFS.begin(true)) {
        Serial.println("ERROR: Could not mount LittleFS! Did you upload the filesystem image?");
        return;
    }
    Serial.println("LittleFS mounted successfully.");
}

void DashboardServer::setupWiFi() {
    Serial.println("Setting up Access Point...");
    WiFi.softAP(Config::AP_SSID, Config::AP_PASSWORD);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void DashboardServer::setupDNS() {
    // Start DNS server that resolves all domains to our AP IP (Captive Portal requirement)
    dnsServer.start(Config::DNS_PORT, "*", WiFi.softAPIP());
}

String DashboardServer::formatUptime() {
    unsigned long seconds = millis() / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;
    return String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
}

void DashboardServer::setupServer() {
    // Route 1: Serve Static Files automatically from LittleFS
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    // Route 2: JSON API to fetch live data (ArduinoJson 7 format)
    server.on("/api/data", HTTP_GET, [this](AsyncWebServerRequest *request) {
        JsonDocument doc;
        doc["uptime"] = formatUptime();
        doc["memory"] = ESP.getFreeHeap() / 1024;
        doc["led"] = ledState;
        
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    // Route 3: API to toggle LED
    server.on("/api/toggle", HTTP_POST, [](AsyncWebServerRequest *request) {
        ledState = !ledState;
        digitalWrite(Config::LED_PIN, ledState ? HIGH : LOW);
        request->send(200, "text/plain", "OK");
    });

    // Route 4: Captive Portal Redirect (Catch-all for unknown domains)
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("http://" + WiFi.softAPIP().toString() + "/");
    });

    server.begin();
    Serial.println("Async Web Server started on port " + String(Config::HTTP_PORT));
}

void DashboardServer::handleClient() {
    // AsyncWebServer runs in the background automatically,
    // but DNSServer still requires us to process requests in the loop.
    dnsServer.processNextRequest();
}

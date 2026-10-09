#pragma once
#include <Arduino.h>

class DashboardServer {
public:
    void begin();
    void handleClient();

private:
    void setupFS();
    void setupWiFi();
    void setupDNS();
    void setupServer();
    
    String formatUptime();
};

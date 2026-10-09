#include <Arduino.h>
#include "DashboardServer.h"

// Instantiate our decoupled application logic
DashboardServer dashboard;

void setup() {
    Serial.begin(115200);
    Serial.println("\n--- ESP32 Application Starting ---");
    
    // Start the application
    dashboard.begin();
}

void loop() {
    // Keep the application running
    dashboard.handleClient();
}
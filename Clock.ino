
void displayClock(){
  // clock functions only available if ssid and password is present
  if (!strlen(ssid) == 0 || !strlen(password) == 0) {
      struct tm timeinfo;
      configTime(-5*3600, 3600, "pool.ntp.org");
      // Wait until we get a valid time (only when button is pressed)
      while (!getLocalTime(&timeinfo)) {
        Serial.println("No time available (yet), retrying...");
        delay(1000);  // Retry every second
      }
    
    char timeString[50];
    strftime(timeString, sizeof(timeString), "%a, %m/%d/%Y %H:%M", &timeinfo);
    Serial.println(timeString);  // Print the formatted time to the Serial Monitor
    
     Serial.println(timeString);
     tft.drawRect(164,171,150,16,TFT_GREEN);
     tft.fillRect(166,173,146,14,TFT_BLACK);
     tft.setCursor(175,176);
     tft.setTextSize(1);
     tft.setTextColor(TFT_WHITE);
     tft.println(timeString);
  }

}

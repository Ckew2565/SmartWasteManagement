#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "IOT Engineering";
const char* password = "coeai123";

const char* apiKey = "957GJAASJCJNEIZ5";
const char* channelID = "2465858";

const int flyingFishPin = 23; // Pin connected to Flying Fish sensor
const int threshold = 80; // Threshold value for garbage level

void setup() {
  Serial.begin(115200);
  pinMode(flyingFishPin, INPUT);
  connectToWiFi();
}

void loop() {
  int sensorValue = digitalRead(flyingFishPin);
  
  if (sensorValue == HIGH) {
    // If the sensor detects an object (e.g., garbage), send data to Thingspeak and check for notification
    sendToThingspeak(sensorValue);
    delay(200); // Wait 10 seconds to avoid continuous requests
  }
  
  // Check garbage level
  if (sensorValue >= threshold) {
    sendNotification(); // Send notification through Line
    delay(100); // Wait 5 minutes before sending another notification
  }
  
  delay(100); // Adjust delay according to your requirements
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void sendToThingspeak(int value) {
  HTTPClient http;
  
  const char* thingspeakAPI = "http://api.thingspeak.com/update";
  
  http.begin(thingspeakAPI);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String postData = "api_key=" + String(apiKey) + "&field1=" + String(value);
  
  int httpResponseCode = http.POST(postData);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

void sendNotification() {
  HTTPClient http;
  
  const char* lineToken = "zvZVfaOoG4eEg6wmBf0VZTt5FKTX8Ierd6gHXoxRfyR";
  const char* lineNotifyAPI = "https://notify-api.line.me/api/notify";
  
  http.begin(lineNotifyAPI);
  http.addHeader("Authorization", "Bearer " + String(lineToken));
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String message = "Garbage bin is full!";
  
  int httpResponseCode = http.POST("message=" + message);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

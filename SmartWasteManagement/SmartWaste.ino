#include <WiFi.h>
#include <Wire.h>
const char *ssid = "IOT Engineering"; // Replace with your WiFi credentials
const char *password = "coeai123";
const char *thingSpeakAPIKey = "LURWBFIGII1NG6Z8";

#define SENSOR_PIN 12 // ESP32 pin GPIO18 connected to OUT pin of IR obstacle avoidance sensor

void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the Arduino's pin as aninput
  pinMode(SENSOR_PIN, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  // read the state of the the input pin:
  int state = digitalRead(SENSOR_PIN);

int on;

  if (state == LOW){

    Serial.println("The trash is Full");
    on = 1;
  } else {
    Serial.println("The trash is not Full");
    on = 0;
    
  }
 sendToThingSpeak(on);
}


void sendToThingSpeak(int x) {
    const char* z;

    if (x) {
        z = "The trash is Full";
    } else {
        z = "The trash is not Full";
    }

    WiFiClient client;
    const char* server = "api.thingspeak.com";
    String data = "GET /update?api_key=" + String(thingSpeakAPIKey);
    data += "&field1=" + String(x);
    data += "\r\n";
    if (client.connect(server, 80)) {
        client.print(data);
        client.stop();
        Serial.println("Data sent to ThingSpeak!");
    } else {
        Serial.println("Failed to connect to ThingSpeak server!");
    }
}

#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Defining the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

int counter = 0;
bool senderState = true; // Assume it's initially ON
String comment;
const char* ssid = "Hello";
const char* password = "12345678";
const char* serverAddress = "http://192.168.8.187:30001";

void setup() {
  // Initializing Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  // Setting LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  LoRa.setSpreadingFactor(7);
  LoRa.setCodingRate4(5);
  LoRa.setSignalBandwidth(62.5E3);

  // Setting the LoRa.begin(---E-) argument with our location's frequency 
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }

  // Setting sync word (0xF3) to match the receiver
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Check the sender state
  HTTPClient http;
  http.begin(String(serverAddress) + "/sender-state");
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);
    if(!error){
      Serial.println(response);
      senderState =  doc["senderState"]; // Update sender state based on the server response
      comment = String(doc["comment"].as<const char*>());
      Serial.print("Sender State: ");
      Serial.println(senderState);
    } else{
      Serial.print("JSON Parsing Error: ");
      Serial.println(error.c_str());
    }
    
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  if (senderState) {
    Serial.print("Sending packet: ");
    Serial.println(counter);

    // Sending LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print("{\"Data\":\"hello " + String(counter) + "\",\"Comment\":\"" + String(comment) + "\"}");
    LoRa.endPacket();
    counter++;

    delay(1000);
  } else {
    Serial.println("Sender is OFF. Not sending data.");
    delay(1000); // Wait and check the sender state again
  }
}

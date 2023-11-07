
// Including necessary libraries
#include <SPI.h>
// #include <RH_RF95.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
// Defining the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

const char* ssid = "Hello";
  const char* password = "12345678";

  // Node.js server information
  const char* serverAddress = "http://192.168.8.187:30001";

void setup() {

  // Initializing Serial Monitor
  Serial.begin(115200);
  
  while (!Serial);
  Serial.println("LoRa Receiver");

  

  // Setting up LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  LoRa.setSpreadingFactor(7);

  LoRa.setCodingRate4(5);

  LoRa.setSignalBandwidth(62.5E3);

  
  
  // Setting the LoRa.begin(---E-) argument with our location's frequency 
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  
  // Setting sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
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
  DynamicJsonDocument doc(1024);
  // Trying to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("RCV2: ");
    String LoRaData;
    // Reading packet
    while (LoRa.available()) {

      LoRaData = LoRa.readString();
      
      DeserializationError error = deserializeJson(doc, LoRaData);
      Serial.println(LoRaData); 

      
    }
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());

    HTTPClient http;
    http.begin(String(serverAddress) + "/esp32data");
    http.addHeader("Content-Type", "application/json");
    String dataToSend = "{\"rcv\": 1 ,\"Data\":\"" + String(doc["Data"].as<const char*>()) + "\",\"RSSI\":\"" + String(LoRa.packetRssi()) + "\",\"Comment\":\"" + String(doc["Comment"].as<const char*>()) + "\"}";
    Serial.println("Data: " + dataToSend);
    Serial.println("WTF: " + String(doc["Data"].as<const char*>()));
    // Send data to the server
    int httpResponseCode = http.POST(dataToSend);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Server Response: ");
      Serial.println(response);
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
    // Printing RSSI of packet
    
  }

}

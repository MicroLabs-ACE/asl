#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include "Sample.h"

// Constants
const uint8_t number_of_frames = 10;
const uint8_t frames_per_second = 10;
const char* datasetFilePath = "/dataset.txt";

// WiFi and server settings
const char* ssid = "Fayolunet";
const char* password = "Tunmise1134@";
WebServer server(80);

File datasetFile;
Frame frames[number_of_frames];

void handleRoot() {
  datasetFile = SPIFFS.open(datasetFilePath, FILE_READ);
  server.streamFile(datasetFile, "text/plain");
  datasetFile.close();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();

  // Mount filesystem
  if (!SPIFFS.begin(true)) {
    Serial.println("Error while mounting SPIFFS.");
    return;
  }
  Serial.println("Mounted SPIFFS.");

  // Write header to dataset file
  datasetFile = SPIFFS.open(datasetFilePath, FILE_WRITE);
  datasetFile.println("thumb,index,middle,ring,pinky,target");
  datasetFile.close();
}

void loop() {
  server.handleClient();

  // Funtionality to add data to dataset file 
  while (Serial.available() > 0) {
    String target = Serial.readStringUntil('\n');
    if (capture_sequence(frames, number_of_frames, frames_per_second) == 0) {
      String data = String(frames->thumb) + "," + String(frames->index) + "," + String(frames->middle) + "," + String(frames->ring) + "," + String(frames->pinky) + "," + target;
      Serial.println(data);

      // Append to dataset file
      datasetFile = SPIFFS.open(datasetFilePath, FILE_APPEND);
      datasetFile.println(data);
      datasetFile.close();
    }
  }
}

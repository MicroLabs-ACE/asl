#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>

// Constants
const uint8_t thumbPin = 35;
const uint8_t indexPin = 33;
const uint8_t middlePin = 26;
const uint8_t ringPin = 25;
const uint8_t pinkyPin = 27;

const uint8_t number_of_frames = 10;
const uint8_t frames_per_second = 10;
const char* datasetFilePath = "/dataset.txt";

// WiFi and server settings
const char* ssid = "Fayolunet";
const char* password = "Tunmise1134@";
WebServer server(80);

File datasetFile;

void handleRoot() {
  datasetFile = SPIFFS.open(datasetFilePath, FILE_READ);
  server.streamFile(datasetFile, "text/plain");
  datasetFile.close();
}

void setup() {
  Serial.begin(115200);

  pinMode(thumbPin, INPUT);
  pinMode(indexPin, INPUT);
  pinMode(middlePin, INPUT);
  pinMode(ringPin, INPUT);
  pinMode(pinkyPin, INPUT);

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
    String data = String(analogRead(thumbPin)) + "," + String(analogRead(indexPin)) + "," + String(analogRead(middlePin)) + "," + String(analogRead(ringPin)) + "," + String(analogRead(pinkyPin)) + "," + target;
    Serial.println(data);

    // Append to dataset file
    datasetFile = SPIFFS.open(datasetFilePath, FILE_APPEND);
    datasetFile.println(data);
    datasetFile.close();
  }
}

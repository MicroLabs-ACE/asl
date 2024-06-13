#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))  // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial2
#endif

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
#else
  FPSerial.begin(9600);
#endif

  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true)) {  // Use serial to communicate with mp3.
    Serial.println(F("Unable to begin."));
    while (true) { delay(0); }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30);  // Set volume value. From 0 to 30
}

void loop() {
  if (Serial.available() > 0) {
    String number = Serial.readStringUntil('\n');
    Serial.println(number.toInt());
    myDFPlayer.play(number.toInt());
  }
}

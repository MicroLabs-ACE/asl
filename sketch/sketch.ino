#include "Sample.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
  if (init_module() == 0) {
    Serial.println("MPU6050 Found!");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Frame Raw_Frames[50];
  String detected_word;

  if (capture_sequence(Raw_Frames, 50, 10) == 0) {
    Serial.println("Data captured");

    Serial.println(Raw_Frames.accel_X);

    /*
    VICTOR WILL WRITE CODE HERE.

    probably inside the detect_word(Raw_Frames) function . It should return String so I can just pass it as argument into the next function

    Then I will play out after using play_word(String detected_word) function
    */

    // if (play_word(detected_word, 30) != 0) {
    //   Serial.println("Error playing out word.");
    // }
  }
}

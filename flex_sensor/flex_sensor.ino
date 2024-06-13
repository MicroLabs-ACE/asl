const uint8_t thumbPin = 35;
const uint8_t indexPin = 33;
const uint8_t middlePin = 26;
const uint8_t ringPin = 25;
const uint8_t pinkyPin = 27;

float thumbValue;
float indexValue;
float middleValue;
float ringValue;
float pinkyValue;

void setup() {
  Serial.begin(115200);
  pinMode(thumbPin, INPUT);
  pinMode(indexPin, INPUT);
  pinMode(middlePin, INPUT);
  pinMode(ringPin, INPUT);
  pinMode(pinkyPin, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    thumbValue = float(map(analogRead(thumbPin), 2700, 4095, 0, 100));
    indexValue = float(map(analogRead(indexPin), 2700, 4095, 0, 100));
    middleValue = float(map(analogRead(middlePin), 2700, 4095, 0, 100));
    ringValue = float(map(analogRead(ringPin), 2700, 4095, 0, 100));
    pinkyValue = float(map(analogRead(pinkyPin), 2700, 4095, 0, 100));
    String letter = Serial.readStringUntil('\n');

    Serial.print("'");
    Serial.print(letter);
    Serial.println("': {");

    Serial.print("'thumb': (");
    Serial.print(thumbValue - 5);
    Serial.print(", ");
    Serial.print(thumbValue + 5);
    Serial.println("),");

    Serial.print("'index': (");
    Serial.print(indexValue - 5);
    Serial.print(", ");
    Serial.print(indexValue + 5);
    Serial.println("),");

    Serial.print("'middle': (");
    Serial.print(middleValue - 5);
    Serial.print(", ");
    Serial.print(middleValue + 5);
    Serial.println("),");

    Serial.print("'ring': (");
    Serial.print(ringValue - 5);
    Serial.print(", ");
    Serial.print(ringValue + 5);
    Serial.println("),");

    Serial.print("'pinky': (");
    Serial.print(pinkyValue - 5);
    Serial.print(", ");
    Serial.print(pinkyValue + 5);
    Serial.println("),");

    Serial.println("},");
    Serial.println();
    Serial.println();
  }

  // Serial.println(thumbValue);
  // Serial.println(indexValue);
  // Serial.println(middleValue);
  // Serial.println(ringValue);
  // Serial.println(pinkyValue);
  // Serial.println();

  delay(1500);
}

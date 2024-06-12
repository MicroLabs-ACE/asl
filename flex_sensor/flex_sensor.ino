const uint8_t thumbPin = 35;
const uint8_t indexPin = 33;
const uint8_t middlePin = 26;
const uint8_t ringPin = 25;
const uint8_t pinkyPin = 27;

void setup() {
  Serial.begin(115200);
  pinMode(thumbPin, INPUT);
  pinMode(indexPin, INPUT);
  pinMode(middlePin, INPUT);
  pinMode(ringPin, INPUT);
  pinMode(pinkyPin, INPUT);
}

void loop() {
  Serial.println(analogRead(thumbPin));
  Serial.println(analogRead(indexPin));
  Serial.println(analogRead(middlePin));
  Serial.println(analogRead(ringPin));
  Serial.println(analogRead(pinkyPin));
  Serial.println();
  delay(1000);
}

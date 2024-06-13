#include <TensorFlowLite_ESP32.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))  // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial2
#endif

#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "model.h"

namespace {
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;
constexpr int kTensorArenaSize = 48 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}

// Flex sensor constants and variables
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

// Model constants and variables
const char labels[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
const uint8_t frames_per_second = 10;
const uint8_t number_of_frames = 10;
const uint8_t number_of_labels = sizeof(labels) / sizeof(char);

int max_i;

// MP3 constants and variables
const uint8_t volume = 30;

DFRobotDFPlayerMini myDFPlayer;

void setup() {
// MP3 player
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
#else
  FPSerial.begin(9600);
#endif

  Serial.begin(115200);

  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true)) {
    Serial.println(F("Unable to begin."));
    while (true) { delay(0); }
  }
  myDFPlayer.volume(volume);

  // Model
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;
  model = tflite::GetModel(model_tflite);

  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report("Model provided is schema version %d not equal to supported version %d.", model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::ops::micro::AllOpsResolver resolver;
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  // Flex sensors
  pinMode(thumbPin, INPUT);
  pinMode(indexPin, INPUT);
  pinMode(middlePin, INPUT);
  pinMode(ringPin, INPUT);
  pinMode(pinkyPin, INPUT);
}

void max_index(float array[], int length) {
  float max_value = -1.0;
  max_i = -1;
  for (int i = 0; i < length; i++) {
    if (array[i] > max_value) {
      max_i = i;
      max_value = array[i];
    }
  }
}

void get_sensor_data() {
  thumbValue = float(map(analogRead(thumbPin), 2700, 4095, 0, 100));
  indexValue = float(map(analogRead(indexPin), 2700, 4095, 0, 100));
  middleValue = float(map(analogRead(middlePin), 2700, 4095, 0, 100));
  ringValue = float(map(analogRead(ringPin), 2700, 4095, 0, 100));
  pinkyValue = float(map(analogRead(pinkyPin), 2700, 4095, 0, 100));

  // thumbValue = float(random(0, 100));
  // indexValue = float(random(0, 100));
  // middleValue = float(random(0, 100));
  // ringValue = float(random(0, 100));
  // pinkyValue = float(random(0, 100));
}

void loop() {
  Serial.println("Got here.");
  get_sensor_data();

  input->data.f[0] = thumbValue;
  input->data.f[1] = indexValue;
  input->data.f[2] = middleValue;
  input->data.f[3] = ringValue;
  input->data.f[4] = pinkyValue;

  Serial.print(thumbValue);
  Serial.print(", ");
  Serial.print(indexValue);
  Serial.print(", ");
  Serial.print(middleValue);
  Serial.print(", ");
  Serial.print(ringValue);
  Serial.print(", ");
  Serial.print(pinkyValue);
  Serial.print(": ");

  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    Serial.println("Error occurred in invoking interpreter.");
    return;
  }

  float output_array[number_of_labels];
  for (int i = 0; i < number_of_labels; i++) {
    output_array[i] = output->data.f[i];
  }

  max_index(output_array, number_of_labels);
  Serial.println(labels[max_i]);
  Serial.println();
  myDFPlayer.play((max_i * 2) + 1);

  delay(2500);
}
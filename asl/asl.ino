#include <DFRobotDFPlayerMini.h>
#include <TensorFlowLite_ESP32.h>

#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "model.h"
#include "Sample.h"

namespace {
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;
constexpr int kTensorArenaSize = 6 * 1024;
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
static const uint8_t PIN_MP3_TX = 19;
static const uint8_t PIN_MP3_RX = 18;

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);

  // MP3 player
  Serial1.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
  if (!myDFPlayer.begin(Serial1, /*isACK = */ true, /*doReset = */ true)) {
    return -2;
  }
  myDFPlayer.setTimeOut(500);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

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
  thumbValue = float(analogRead(thumbPin));
  indexValue = float(analogRead(indexPin));
  middleValue = float(analogRead(middlePin));
  ringValue = float(analogRead(ringPin));
  pinkyValue = float(analogRead(pinkyPin));
}

void loop() {
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
  String detected_character = String(labels[max_i]);
  play_word(detected_character, volume);

  Serial.println(labels[max_i]);
  Serial.println();

  delay(2000);
}
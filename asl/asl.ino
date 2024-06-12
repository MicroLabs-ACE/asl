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

// Constants and variables
const char labels[] = { 'B', 'C', 'D', 'L', 'Y', '\0' };
const uint8_t frames_per_second = 10;
const uint8_t number_of_frames = 10;
const uint8_t number_of_labels = sizeof(labels) / sizeof(char);
const uint8_t volume = 30;
int max_i;

void setup() {
  Serial.begin(115200);
  init_module();

  // Model setup
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

void loop() {
  Frame frames[number_of_frames];
  if (capture_sequence(frames, number_of_frames, frames_per_second) == 0) {
    input->data.f[0] = (float)frames->thumb;
    input->data.f[1] = (float)frames->index;
    input->data.f[2] = (float)frames->pinky;

    Serial.print(frames->thumb);
    Serial.print(", ");
    Serial.print(frames->index);
    Serial.print(", ");
    Serial.print(frames->middle);
    Serial.print(", ");
    Serial.print(frames->ring);
    Serial.print(", ");
    Serial.print(frames->pinky);
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
  }

  delay(2000);
}
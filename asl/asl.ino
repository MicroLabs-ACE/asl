#include <TensorFlowLite_ESP32.h>

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model.h"
#include "Sample.h"

namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
constexpr int kTensorArenaSize = 6 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}

// Constants
const uint8_t number_of_frames = 10;
const uint8_t frames_per_second = 10;

void setup() {
  Serial.begin(115200);

  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter, "Model provided is schema version %d not equal to supported version %d.", model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::AllOpsResolver resolver;

  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);
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
    Serial.print(frames->pinky);
    Serial.print(": ");
  }

  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    Serial.println("Error occured in invoking interpreter.");
    return;
  }

  if (output->data.f[0] == 1.0) {
    Serial.print("B");
  } else if (output->data.f[1] == 1.0) {
    Serial.print("C");
  } else if (output->data.f[2] == 1.0) {
    Serial.print("D");
  } else if (output->data.f[3] == 1.0) {
    Serial.print("L");
  } else if (output->data.f[4] == 1.0) {
    Serial.print("Y");
  }

  Serial.println();
  Serial.println();

  delay(2000);
}
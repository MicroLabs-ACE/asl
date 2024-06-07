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
constexpr int kTensorArenaSize = 3 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}


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

int splitString(String data, char delimiter, String* parts) {
  int delimiterIndex = data.indexOf(delimiter);

  if (delimiterIndex == -1) return 0;
  else {
    parts[0] = data.substring(0, delimiterIndex);
    parts[1] = data.substring(delimiterIndex + 1);

    return 2;
  }
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    String parts[2];

    int count = splitString(data, ',', parts);
    if (count == 2) {
      Serial.print(parts[0]);
      Serial.print(", ");
      Serial.println(parts[1]);
    } else {
      Serial.println("Invalid input. Please provide two comma-separated values.");
    }

    input->data.f[0] = parts[0].toFloat();
    input->data.f[1] = parts[1].toFloat();

    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
      Serial.println("Error occured in invoking interpreter.");
      return;
    }

    Serial.print(output->data.f[0]);
    Serial.print(" ");
    Serial.print(output->data.f[1]);
    Serial.print(" ");
    Serial.print(output->data.f[2]);
    Serial.print(" ");
    Serial.print(output->data.f[3]);
    Serial.println();
    Serial.println();
  }
}
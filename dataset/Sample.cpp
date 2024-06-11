#include "Sample.h"

const uint8_t thumbPin = 32;
const uint8_t indexPin = 33;
const uint8_t middlePin = 25;
const uint8_t ringPin = 26;
const uint8_t pinkyPin = 27;

int capture_frame(Frame* frame) {
  if (frame == nullptr) { return -1; }

  frame->thumb = analogRead(thumbPin);
  frame->index = analogRead(indexPin);
  frame->middle = analogRead(middlePin);
  frame->ring = analogRead(ringPin);
  frame->pinky = analogRead(pinkyPin);

  return 0;
}

int capture_sequence(Frame* frames, int number_of_frames, int frames_per_second) {
  if (frames == nullptr) { return -1; }

  int delay_time = (int)(1000 / frames_per_second);
  for (int i = 0; i < number_of_frames; i++) {
    capture_frame((frames + i));
    delay(delay_time);
  }

  return 0;
}
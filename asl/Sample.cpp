#include "Sample.h"


int capture_frame(Frame* newFrame) {
  if (newFrame == nullptr) { return -1; }

  newFrame->thumb = map(analogRead(36), 0, 4095, 0, 100);
  newFrame->index = map(analogRead(39), 0, 4095, 0, 100);
  newFrame->middle = map(analogRead(34), 0, 4095, 0, 100);
  newFrame->ring = map(analogRead(35), 0, 4095, 0, 100);
  newFrame->pinky = map(analogRead(32), 0, 4095, 0, 100);

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
#ifndef _SAMPLE
#define _SAMPLE

#include <Arduino.h>

typedef struct Frame {
  uint8_t thumb;
  uint8_t index;
  uint8_t middle;
  uint8_t ring;
  uint8_t pinky;
};

int capture_frame(Frame* newFrame);
int capture_sequence(Frame* frames, int number_of_frames, int frames_per_second);

#endif
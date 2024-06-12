#ifndef _SAMPLE
#define _SAMPLE

#include <Arduino.h>

typedef struct Frame
{
  uint8_t thumb;
  uint8_t index;
  uint8_t middle;
  uint8_t ring;
  uint8_t pinky;

  float accel_X;
  float accel_Y;
  float accel_Z;

  float gyro_X;
  float gyro_Y;
  float gyro_Z;
};

int init_module();
int capture_frame(Frame *newFrame);
int capture_sequence(Frame *frames, int number_of_frames, int frames_per_second);
int play_word(String detected_word, int volume);

#endif
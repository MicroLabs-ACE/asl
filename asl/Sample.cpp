#include <DFRobotDFPlayerMini.h>

#include "Sample.h"

const uint8_t thumbPin = 32;
const uint8_t indexPin = 33;
const uint8_t middlePin = 25;
const uint8_t ringPin = 26;
const uint8_t pinkyPin = 27;

static const uint8_t PIN_MP3_TX = 26;
static const uint8_t PIN_MP3_RX = 27;

DFRobotDFPlayerMini myDFPlayer;

int init_module()
{
  Serial1.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
  if (!myDFPlayer.begin(Serial1, /*isACK = */ true, /*doReset = */ true))
  {
    return -2;
  }
  myDFPlayer.setTimeOut(500);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  return 0;
}

int capture_frame(Frame *frame)
{
  if (frame == nullptr)
  {
    return -1;
  }

  frame->thumb = analogRead(thumbPin);
  frame->index = analogRead(indexPin);
  frame->middle = analogRead(middlePin);
  frame->ring = analogRead(ringPin);
  frame->pinky = analogRead(pinkyPin);

  return 0;
}

int capture_sequence(Frame *frames, int number_of_frames, int frames_per_second)
{
  if (frames == nullptr)
  {
    return -1;
  }

  int delay_time = (int)(1000 / frames_per_second);
  for (int i = 0; i < number_of_frames; i++)
  {
    capture_frame((frames + i));
    delay(delay_time);
  }

  return 0;
}

bool is_word_saved_in_sd_card(String checked_word)
{
  if (
      checked_word.equals("a") || checked_word.equals("b") || checked_word.equals("c") || checked_word.equals("d") || checked_word.equals("e") || checked_word.equals("f") || checked_word.equals("g") || checked_word.equals("h") || checked_word.equals("i") || checked_word.equals("j") || checked_word.equals("k") || checked_word.equals("l") || checked_word.equals("m") || checked_word.equals("n") || checked_word.equals("o") || checked_word.equals("p") || checked_word.equals("q") || checked_word.equals("r") || checked_word.equals("t") || checked_word.equals("u") || checked_word.equals("v") || checked_word.equals("w") || checked_word.equals("x") || checked_word.equals("y") || checked_word.equals("z"))
  {
    return true;
  }
  else
    return false;
}

int retrieve_word_address(String toPlay)
{
  if (toPlay.equals("a"))
  {
    return 0;
  }
  else if (toPlay.equals("b"))
  {
    return 1;
  }
  else if (toPlay.equals("c"))
  {
    return 2;
  }
  else if (toPlay.equals("d"))
  {
    return 3;
  }
  else if (toPlay.equals("e"))
  {
    return 4;
  }
  else if (toPlay.equals("f"))
  {
    return 5;
  }
  else if (toPlay.equals("g"))
  {
    return 6;
  }
  else if (toPlay.equals("h"))
  {
    return 7;
  }
  else if (toPlay.equals("i"))
  {
    return 8;
  }
  else if (toPlay.equals("j"))
  {
    return 9;
  }
  else if (toPlay.equals("k"))
  {
    return 10;
  }
  else if (toPlay.equals("l"))
  {
    return 11;
  }
  else if (toPlay.equals("m"))
  {
    return 12;
  }
  else if (toPlay.equals("n"))
  {
    return 13;
  }
  else if (toPlay.equals("o"))
  {
    return 14;
  }
  else if (toPlay.equals("p"))
  {
    return 15;
  }
  else if (toPlay.equals("q"))
  {
    return 16;
  }
  else if (toPlay.equals("r"))
  {
    return 17;
  }
  else if (toPlay.equals("s"))
  {
    return 18;
  }
  else if (toPlay.equals("t"))
  {
    return 19;
  }
  else if (toPlay.equals("u"))
  {
    return 20;
  }
  else if (toPlay.equals("v"))
  {
    return 21;
  }
  else if (toPlay.equals("w"))
  {
    return 22;
  }
  else if (toPlay.equals("x"))
  {
    return 23;
  }
  else if (toPlay.equals("y"))
  {
    return 24;
  }
  else if (toPlay.equals("z"))
  {
    return 25;
  }
}

int play_word(String detected_word, int volume)
{
  if (!is_word_saved_in_sd_card(detected_word))
  {
    return -1;
  }

  if (volume > 0 && volume <= 30)
    myDFPlayer.volume(volume);
  else
    return -2;

  myDFPlayer.playFolder(15, retrieve_word_address(detected_word));
  return 0;
}
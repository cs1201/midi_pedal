#include <Arduino.h>

#define SEGMENT_A (1<<0)
#define SEGMENT_B (1<<1)
#define SEGMENT_C (1<<2)
#define SEGMENT_D (1<<3)
#define SEGMENT_E (1<<4)
#define SEGMENT_F (1<<5)
#define SEGMENT_G (1<<6)
#define SEGMENT_DP (1<<7)

#define SEGMENT_DISPLAY_0 (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F)
#define SEGMENT_DISPLAY_1 (SEGMENT_B | SEGMENT_C)
#define SEGMENT_DISPLAY_2 (SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_E | SEGMENT_D)
#define SEGMENT_DISPLAY_3 (SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_C | SEGMENT_D)
#define SEGMENT_DISPLAY_4 (SEGMENT_F | SEGMENT_G | SEGMENT_B | SEGMENT_C)
#define SEGMENT_DISPLAY_5 (SEGMENT_A | SEGMENT_F | SEGMENT_G | SEGMENT_C | SEGMENT_D)
#define SEGMENT_DISPLAY_6 (SEGMENT_A | SEGMENT_E | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G)
#define SEGMENT_DISPLAY_7 (SEGMENT_A | SEGMENT_B | SEGMENT_C)
#define SEGMENT_DISPLAY_8 (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G)
#define SEGMENT_DISPLAY_9 (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G)
#define SEGMENT_DISPLAY_DP (SEGMENT_DP)
#define SEGMENT_DISPLAY_OFF (0)
#define DISPLAY_OFF (11)
#define DISPLAY_DP (10)

int led_segment_display[] = {
  SEGMENT_DISPLAY_0, 
  SEGMENT_DISPLAY_1, 
  SEGMENT_DISPLAY_2, 
  SEGMENT_DISPLAY_3, 
  SEGMENT_DISPLAY_4, 
  SEGMENT_DISPLAY_5, 
  SEGMENT_DISPLAY_6, 
  SEGMENT_DISPLAY_7, 
  SEGMENT_DISPLAY_8, 
  SEGMENT_DISPLAY_9, 
  SEGMENT_DISPLAY_DP,
  SEGMENT_DISPLAY_OFF
};

class DisplayLED
{
  int current_number;
  int data_pin;
  int latch_pin;
  int clock_pin;
  public:
    DisplayLED(int data_pin, int latch_pin, int clock_pin);
    void write(int number);
    void clear();
};

DisplayLED::DisplayLED(int data, int latch, int clock)
{
  data_pin = data;
  latch_pin = latch;
  clock_pin = clock;
  pinMode(data_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  digitalWrite(latch_pin, LOW);
};

void DisplayLED::write(int number)
{

  if (number < 0 || number > 11)
  {
    return;
  }

  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, MSBFIRST, led_segment_display[number]);
  digitalWrite(latch_pin, HIGH);

  current_number = number;
}

void DisplayLED::clear()
{
  write(DISPLAY_OFF);
};
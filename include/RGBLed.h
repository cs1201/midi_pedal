#include <Arduino.h>

#define OFF (0)
#define ON (1)
#define NUM_RGB_LED_PINS (3)

typedef enum {
  LED_OFF = 0,
  RED,
  GREEN,
  BLUE,
  WHITE,
  YELLOW,
  PURPLE,
  CYAN
} COLOUR_NAME;

struct RGB_COLOUR_T
{
  int r;
  int g;
  int b;
};

RGB_COLOUR_T rgb_colours[] = {
  {0, 0, 0},        // LED_OFF
  {255, 0, 0},      // RED
  {0, 255, 0},      // GREEN
  {0, 0, 255},      // BLUE
  {255, 255, 255},  // WHITE
  {255, 255, 0},    // YELLOW
  {255, 0, 255},    // PURPLE
  {0, 255, 255}     // CYAN
};

class RGBLed
{
    int state = OFF;
    int last_colour;
    int r_pin, g_pin, b_pin;
    public:
        RGBLed(int red_pin, int green_pin, int blue_pin);
        void write(int colour);
        void chase();
        void toggle();
        void off();
};

RGBLed::RGBLed(int red_pin, int green_pin, int blue_pin)
{
    r_pin = red_pin;
    g_pin = green_pin;
    b_pin = blue_pin;
    
    pinMode(r_pin, OUTPUT);
    pinMode(g_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);

    write(LED_OFF);
    last_colour = LED_OFF;
};

void RGBLed::write(int colour_id)
{
    RGB_COLOUR_T colour = rgb_colours[colour_id];
    //Only record last colour if not turning LED OFF
    if (colour_id != LED_OFF)
    {
        last_colour = colour_id;
    }
    analogWrite(r_pin, colour.r);
    analogWrite(g_pin, colour.g);
    analogWrite(b_pin, colour.b);
};

void RGBLed::chase()
{
  //Do a quick flash of all LED colours
  for (int i = 0; i < 3; i++)
  {
    for (int c = RED; c <= CYAN; c++)
    {
      write(c);
      delay(100);
    }
  }
};

void RGBLed::toggle()
{
  int colour;
  state = !state;
  colour = state? LED_OFF : last_colour;
  write(colour);
};

void RGBLed::off()
{
    write(LED_OFF);
};
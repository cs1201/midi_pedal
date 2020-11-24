#include <Bounce2.h>

#define NUM_BUTTONS (3)
#define NUM_RGB_LED_PINS (3)
#define BTN_A (1 << 0)
#define BTN_B (1 << 1)
#define BTN_C (1 << 2)
#define DEFAULT_MIDI_CHANNEL (1)

int midi_channel = DEFAULT_MIDI_CHANNEL;
const int ledPins[NUM_RGB_LED_PINS] = { 15, 14, 13 };
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
unsigned long doubleClickDelay = 150;

int ledState = LOW;

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

int last_led_colour = LED_OFF;

typedef struct Button
{
  int pin_id;
  Bounce *state;
  int program_id;
  int colour;
} Button;

Bounce bounce[3] = {Bounce(), Bounce(), Bounce()};

Button controls[3] = {
  {23, NULL, 1, BLUE},
  {21, NULL, 2, GREEN},
  {19, NULL, 3, RED},
};

void updateLED(int colour_id)
{
  RGB_COLOUR_T colour = rgb_colours[colour_id];
  //Only record last colour if not turning LED OFF
  if (colour_id != LED_OFF)
  {
    last_led_colour = colour_id;
  }
  analogWrite(ledPins[0], colour.r);
  analogWrite(ledPins[1], colour.g);
  analogWrite(ledPins[2], colour.b);
}

void setup()
{    
    Serial.begin(115200);
    for(int i = 0; i < NUM_BUTTONS; i++)
    {
      controls[i].state = &bounce[i];
      pinMode(controls[i].pin_id, INPUT_PULLUP);
      pinMode(ledPins[i], OUTPUT);
      controls[i].state->attach(controls[i].pin_id);
      controls[i].state->interval(10);
    }
    updateLED(LED_OFF);
}

int getButtonStates()
{
  int stateVector = 0;

  for (int i = 0; i < NUM_BUTTONS; i++)
  {   
    stateVector |= (!digitalRead(controls[i].pin_id) << i);
  }
  return stateVector;
}

#define DEFAULT_BANK (1)
#define MAX_BANK (8)
#define MIN_BANK (1)
int bank = DEFAULT_BANK;

void bank_increase()
{
  if (bank < MAX_BANK)
  {
    bank++;
  }
}

void bank_decrease()
{
  if (bank > MIN_BANK)
  {
    bank--;
  }
}

void led_toggle()
{
  ledState = !ledState;
  Serial.println(ledState);
  if (ledState)
  {
    updateLED(LED_OFF);
  }
  else
  {
    updateLED(last_led_colour);
  }
}

void loop()
{
//   Determine state of each button control
   for(int i = 0; i < NUM_BUTTONS; i++)
   {
     Button control = controls[i];
    
      if (control.state->update())
      {
        if (control.state->fell())
        {
            delay(doubleClickDelay);
            int btnState = getButtonStates();

            switch (btnState)
            {
              case BTN_A:
                Serial.println("A");
                updateLED(controls[0].colour);
                usbMIDI.sendProgramChange(control.program_id, midi_channel);
                break;
              case BTN_B:
                Serial.println("B");
                updateLED(controls[1].colour);
                usbMIDI.sendProgramChange(control.program_id, midi_channel);
                break;
              case BTN_C:
                Serial.println("C");
                updateLED(controls[2].colour);
                usbMIDI.sendProgramChange(control.program_id, midi_channel);
                break;
              case (BTN_A | BTN_B):
                bank_decrease();
                Serial.println(bank);
                break;
              case (BTN_B | BTN_C):
                bank_increase();
                Serial.println(bank);
                break;
              case (BTN_A | BTN_C):
                led_toggle();
                Serial.println("LED_TOGGLE");
                break;
              default:
                break;
            }
        }
      }
    }

//  while (usbMIDI.read()) {
//    // read & ignore incoming messages
//  }
}

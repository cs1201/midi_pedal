#include <Bounce2.h>

const int default_midi_channel = 1;
const int numLedPins = 3;
const int numButtons = 3;
const int ledPins[numLedPins] = { 15, 14, 13 };
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
int ledState = HIGH;

const int CLICKED = LOW;
const int UNCLICKED = HIGH;

typedef enum {
  LED_OFF = -1,
  RED = 0,
  GREEN,
  BLUE,
} Colour;

typedef struct Button
{
  int pin_id;
  Bounce *state;
  int program_id;
  int colour;
} Button;

Bounce bounce[3] = {Bounce(), Bounce(), Bounce()};

Button controls[3] = {
  {23, NULL, 1, RED},
  {21, NULL, 2, GREEN},
  {19, NULL, 3, BLUE},
};

void updateLED(int colour)
{
  
  for(int i = 0; i < numLedPins; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }

  if (colour != LED_OFF){
    digitalWrite(ledPins[colour], HIGH);
  }
}

void setup()
{    
    Serial.begin(115200);
    for(int i = 0; i < numButtons; i++)
    {
      controls[i].state = &bounce[i];
      pinMode(controls[i].pin_id, INPUT_PULLUP);
      pinMode(ledPins[i], OUTPUT);
//      bounce[i].attach(controls[i].pin_id);
//      bounce[i].interval(10);
      controls[i].state->attach(controls[i].pin_id);
      controls[i].state->interval(10);
    }
    
    updateLED(LED_OFF);
}

void loop()
{
  // Determine state of each button control
  for(int i = 0; i < numButtons; i++)
  {
    Button control = controls[i];
    
    if (control.state->update())
    {
      if (control.state->fell())
      {
        usbMIDI.sendProgramChange(control.program_id, default_midi_channel);
        updateLED(control.colour);
        Serial.println(i);
      }
    }
  }
  
  while (usbMIDI.read()) {
    // read & ignore incoming messages
  }
}

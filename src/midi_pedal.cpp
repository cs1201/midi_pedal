#include <Arduino.h>
#include <Bounce2.h>
#include <led_display.h>
#include <rgb_led.h>

#define NUM_BUTTONS (3)

#define BTN_A (1 << 0)
#define BTN_B (1 << 1)
#define BTN_C (1 << 2)
#define DEFAULT_MIDI_CHANNEL (1)

int midi_channel = DEFAULT_MIDI_CHANNEL;
const int ledPins[NUM_RGB_LED_PINS] = { 15, 14, 13 };
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
unsigned long doubleClickDelay = 150;

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

RGBLed rgb_led(15, 14, 13);

void setup()
{    
    Serial.begin(115200);
    for(int i = 0; i < NUM_BUTTONS; i++)
    {
      controls[i].state = &bounce[i];
      pinMode(controls[i].pin_id, INPUT_PULLUP);
      controls[i].state->attach(controls[i].pin_id);
      controls[i].state->interval(10);
    }
    rgb_led.chase();
    rgb_led.off();
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
              // PRORGAM SELECT ======
              case BTN_A:
                Serial.println("A");
                rgb_led.write(controls[0].colour);
                usbMIDI.sendProgramChange(control.program_id, midi_channel);
                break;
              case BTN_B:
                Serial.println("B");
                rgb_led.write(controls[1].colour);
                usbMIDI.sendProgramChange(control.program_id, midi_channel);
                break;
              case BTN_C:
                Serial.println("C");
                rgb_led.write(controls[2].colour);
                usbMIDI.sendProgramChange(control.program_id, midi_channel);
                break;
              // ======================
              
              // BANK DOWN
              case (BTN_A | BTN_B):
                bank_decrease();
                Serial.println(bank);
                break;
              // BANK UP  
              case (BTN_B | BTN_C):
                bank_increase();
                Serial.println(bank);
                break;
              // LED ON/OFF
              case (BTN_A | BTN_C):
                rgb_led.toggle();
                Serial.println("LED_TOGGLE");
                break;
              // MUTE
              case (BTN_A | BTN_B | BTN_C):
                usbMIDI.sendProgramChange(127, 1);
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

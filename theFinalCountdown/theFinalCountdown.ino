/*
  Melody
  Plays a melody
  circuit:
  - 8 ohm speaker on digital pin 8
*/

#include "pitches.h"
#include "Adafruit_NeoPixel.h"

#define SPEAKERPIN 2
#define Neo_PIN 3
#define button_PIN 6
#define buttonGND_PIN 7

#define NUM_LEDS 4

int melody[] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,0,
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,0,
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,0,
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,0,
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,0,
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,0,
     };
int melodyLength = sizeof(melody)/sizeof(melody[0]);

float noteWaitFactor = 1.3;
int melodyLengthMs = 10000;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, Neo_PIN, NEO_GRB + NEO_KHZ800);
void setup()
{
  pinMode(button_PIN, INPUT);
  pinMode(buttonGND_PIN, OUTPUT);
  digitalWrite(button_PIN, HIGH);
  digitalWrite(buttonGND_PIN, LOW);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(115200);
  LEDfadeChange(10,10,10,500);
  // playTheSong();
}

void loop()
{
  // doCoolPiano();
  // if (checkButton() == 1)
  if (digitalRead(button_PIN) == 0)
  {
    // LEDfadeChange(250,10,10,500);
    // LEDfadeChange(10,10,10,500);
    washHands();
    // LEDfadeChangeNonBlock(250,10,10,1500, true);
    // playTheSong();
    // playSong(true);
    // LEDfadeChange(10,250,10,500);
    // LEDfadeChange(1,1,1,2000);
  }
  // playSong(false);
  // LEDfadeChangeNonBlock(250,10,10,1500, false);
  // delay(5);
}

void washHands()
{
  LEDfadeChangeNonBlock(250, 10, 10, 1000, true);
  playSong(true);
  unsigned long startTime = millis();
  unsigned long now = millis();
  while ((now - startTime) < melodyLengthMs)
  {
    now = millis();
    LEDfadeChangeNonBlock(0, 0, 0, 0, false);
    playSong(false);
    delay(5);
  }
  LEDfadeChange(10,250,10,500);
  LEDfadeChange(1,1,1,2000);
}

void playTheSong()
{
  int noteDuration = melodyLengthMs / (melodyLength * (1 + noteWaitFactor));
  for (int i = 0; i < melodyLength; i++)
  {
    if (melody[i] != 0)
    {
      tone(SPEAKERPIN, melody[i]);
    }
    delay(noteDuration);
    int pauseBetweenNotes = noteDuration * noteWaitFactor;
    noTone(SPEAKERPIN);
    delay(pauseBetweenNotes);
  }
}

//Can play a song without blocking the CPU. Send restartSong == true to restart the song. otherwise send False
void playSong(bool restartSong)
{
  unsigned long now = millis();
  // static unsigned long startTime = 0;
  static unsigned long lastChange = 0;
  static int noteCtr = 1000;
  int noteDuration = melodyLengthMs / (melodyLength * (1 + noteWaitFactor));
  int pauseBetweenNotes = noteDuration * noteWaitFactor;
  if(restartSong)
  {
    // startTime = now;
    lastChange = now;
    noteCtr = 0;
    tone(SPEAKERPIN, melody[noteCtr]);
  }
  if ((now - lastChange) > (noteDuration + pauseBetweenNotes) && (noteCtr < melodyLength))
  {
    noteCtr++;
    lastChange = now;
    if (melody[noteCtr] != 0)
    {
      tone(SPEAKERPIN, melody[noteCtr]);
    }
    else
    {
      noTone(SPEAKERPIN);
    }
  }
  else if (((now - lastChange) > noteDuration) && (noteCtr < melodyLength))
  {
    noTone(SPEAKERPIN);
  }
}


void LEDfadeChange(uint8_t r, uint8_t g, uint8_t b, uint16_t changetime)
{
  uint32_t startColor = strip.getPixelColor(0);

  uint8_t r_start = (uint8_t)(startColor >> 16);
  uint8_t g_start = (uint8_t)(startColor >> 8);
  uint8_t b_start = (uint8_t)startColor;

  int steps=changetime/4;
  for(int j = 1; j<=steps; j++)
  {
    float fac = 1.0*j/steps;
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      
      strip.setPixelColor(i, r_start*(1.0-fac)+r*fac, g_start*(1-fac)+g*fac, b_start*(1-fac)+b*fac);
    }
    strip.show();
    delay(changetime/steps);
    // if(syncPulse==true){  //Cancel the sequence if we receive a new interrupt 
    //   if(j >20){  //debounce
    //   break;
    //   }else{
    //     syncPulse = false;
    //   }
    // }
  }
}

void LEDfadeChangeNonBlock(uint8_t r, uint8_t g, uint8_t b, uint16_t changetime, bool newVal)
{
  static unsigned long lastChange = 0;
  static uint32_t startColor = 0;
  static uint8_t r_start = 0;
  static uint8_t g_start = 0;
  static uint8_t b_start = 0;
  static uint8_t r_goal = 0;
  static uint8_t g_goal = 0;
  static uint8_t b_goal = 0;
  static int steps = 1;
  static int stepCtr = 1000;
  static int stepDuration = changetime / steps;
  unsigned long now = millis();

  if(newVal){
    lastChange = now;
    startColor = strip.getPixelColor(0);
    r_start = (uint8_t)(startColor >> 16);
    g_start = (uint8_t)(startColor >> 8);
    b_start = (uint8_t)startColor;
    steps=changetime/4;
    stepCtr = 0;
    r_goal = r;
    g_goal = g;
    b_goal = b;

    stepDuration = changetime / steps;
  }

  if (((now - lastChange) > stepDuration) && (stepCtr < steps))
  {
    lastChange = now;
    stepCtr++;
    // for(int j = 1; j<=steps; j++)
    // {
    float fac = (1.0 * stepCtr) / steps;
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {

      strip.setPixelColor(i, r_start * (1.0 - fac) + r_goal * fac, g_start * (1 - fac) + g_goal * fac, b_start * (1 - fac) + b_goal * fac);
    }
    strip.show();
    // delay(changetime / steps);
  }
  // }
}



void doCoolPiano(){
  if (Serial.available() > 0)
  {
    int keyboard[]={'q','w','e','r','t','y','u','i'};
    int notes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
    int receivedChar = Serial.read();
    int note = 0;
    for(int i = 0; i<8; i++){
      if(receivedChar==keyboard[i]){
        note = notes[i];
        break;
      }
    }
    LEDfadeChange(note,note/2,note/3,50);
    tone(SPEAKERPIN, note,250);
  }
}



//Todo: This debounce function doesnt work properly...
int checkButton()
{
  static int buttonState;                    // the current reading from the input pin
  static int lastButtonState = HIGH;          // the previous reading from the input pin
  static unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
  static unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers

  int retVal = 0;
  int reading = digitalRead(button_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState)
    {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW)
      {
        // ledState = !ledState;
        retVal = 1;
      }
    }
    return retVal;
  }
}
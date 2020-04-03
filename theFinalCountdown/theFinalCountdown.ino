/*
  Melody
  Plays a melody
  circuit:
  - 8 ohm speaker on digital pin 8
*/

#include "pitches.h"

int melody[] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,0,
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,0,
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,0,
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,0,
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,0,
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,0,
     };
int melodyLength = sizeof(melody)/sizeof(melody[0]);

#define SPEAKERPIN 8
float noteWaitFactor = 1.3;
int melodyLengthMs = 2000;
void setup()
{
  Serial.begin(115200);
  playTheSong();
}

void loop() {
  doCoolPiano();
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

    tone(SPEAKERPIN, note,250);
  }
}
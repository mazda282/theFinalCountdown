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
int melodyLengthMs = 20000;
void setup()
{
  // int noteDuration = 180;
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

void loop() {
  // no need to repeat the melody.
}

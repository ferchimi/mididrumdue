
/*
   This examples shows how to make a simple seven keys MIDI keyboard with volume control

   Created: 4/10/2015
   Author: Arturo Guadalupi <a.guadalupi@arduino.cc>
   
   http://www.arduino.cc/en/Tutorial/MidiDevice
*/

#include "MIDIUSB.h"
#include "PitchToNote.h"
#define NUM_PADS  7

const uint8_t pad1 = 2;
const uint8_t pad2 = 3;
const uint8_t pad3 = 4;
const uint8_t pad4 = 5;

const int intensityPot = 0;  //A0 input

const uint8_t pad[NUM_PADS] = {pad1, pad2, pad3, pad4};
const byte notePitches[NUM_BUTTONS] = {pitchC3, pitchD3, pitchE3, pitchF3};

uint8_t notesTime[NUM_BUTTONS];
uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;
uint8_t intensity;

void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT);
}


void loop() {
  readPads();
//  readIntensity();
  playNotes();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

/*void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      bitWrite(pressedButtons, i, 1);
      delay(50);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}*/


void readPads()
{
  for (int i=; i< NUM_PADS; i++){
  int val = analogRead(intensityPot);
  intensity = (uint8_t) (map(val, 0, 1023, 0, 127));
}

void playNotes()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i , 1);
        noteOn(0, notePitches[i], intensity);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousButtons, i , 0);
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

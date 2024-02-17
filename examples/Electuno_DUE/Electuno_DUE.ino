/*
Electuno for Arduino Due at 21Khz

FILE: Electuno_DUE.ino
AUTHOR: Israel Reyes Rodríguez (isRasta)
URL: https://github.com/amiga68k/electuno
MAIL: amiga68k@gmail.com

MIDI  -> TX1/RX1 Pin
AUDIO    -> DAC0 Pin

///////////////////////
//Compilation options//
///////////////////////
*/

#define WAVEMIXMODE 1
#define FREQTUNE 2.125
#define VOLUMECONTROL 0
#define EXPRESSIONPEDAL 1
#define CHORUS 2
#define CHORUSBUFFERSIZE 5
#define REVERB 1
#define OVERDRIVE 1
#define WAVESIZE 10
#define POLYPHONY  10
#define UPPERMODE 2
#define LOWERMODE 2
#define PEDALMODE 1
#define LESLIE 2
#define LESLIEBUFFERSIZE 10

///////////////////////////
//End compilation options//
///////////////////////////

#include <DueTimer.h> // Get from official library manager or download here: https://github.com/ivanseidel/DueTimer
#include <MIDI.h> // Get from official library manager or download here: https://github.com/FortySevenEffects/arduino_midi_library
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
#include <electuno.h>

void MySettings()
{ 
/////////////////////////////////
//User/midi realtime parameters//
/////////////////////////////////

  upperDrawbar[0]=8;
  upperDrawbar[1]=8;
  upperDrawbar[2]=8;
  upperDrawbar[3]=0;
  upperDrawbar[4]=0;
  upperDrawbar[5]=0;
  upperDrawbar[6]=0;
  upperDrawbar[7]=0;
  upperDrawbar[8]=1;
  
  lowerDrawbar[0]=0;
  lowerDrawbar[1]=0;
  lowerDrawbar[2]=8;
  lowerDrawbar[3]=8;
  lowerDrawbar[4]=0;
  lowerDrawbar[5]=0;
  lowerDrawbar[6]=0;
  lowerDrawbar[7]=0;
  lowerDrawbar[8]=0;

  pedalDrawbar[0]=0;
  pedalDrawbar[1]=0;
  pedalDrawbar[2]=8;
  pedalDrawbar[3]=0;
  pedalDrawbar[4]=0;
  pedalDrawbar[5]=0;
  pedalDrawbar[6]=0;
  pedalDrawbar[7]=0;
  pedalDrawbar[8]=0;  

  rotaryValue = 0;
  leslieDrumVibrato = 6;
  leslieHornVibrato = 10;
  leslieLowpassFilter = 16 ;
  leslieHipassFilter = 14 ; 
  leslieDrumPhase = 10  ; 
  leslieHornPhase = 16; 
  leslieDrumMicVolume = 16 ; 
  leslieHornMicVolume = 16; 

///////////////////////////
//User on boot parameters//
///////////////////////////

  leslieHornDeceleration = 200;
  leslieHornAcceleration = 255;
  leslieDrumDeceleration = 1;
  leslieDrumAcceleration = 16;  
  // chorusSpeed = 6.86; // in Hz
  // upperVibratoSwitch = 0; // 0=Off  1=On
  // vibratoType = 0; // 0=C1  1=V1
  // rotaryValue = 0; // initial rotary status : 0=off  1=slow  2=fast 
  // leslieHornSpeedSlow = 0.83; // Horn slow speed Hz
  // leslieHornSpeedFast = 7.5; // Horn speed Hz  
  // leslieDrumSpeedSlow = 0.66; // Drum slow speed Hz
  // leslieDrumSpeedFast = 5.9; // Drum fast speed Hz  
}

void setup() {
  MySettings();
  OrganSetup();
  MidiSetup();  
  DAC_setup();
  TimerSetup();  
}

void TimerSetup()
{  
  //  Set output frequency
  Timer3.attachInterrupt(OutputTimer).setFrequency(21000).start();
}

void MidiSetup()
{

  MIDI.setHandleNoteOn(electunoNoteOn);
  MIDI.setHandleNoteOff(electunoNoteOff);
  MIDI.setHandleControlChange(electunoControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

void DAC_setup()
{
  analogWriteResolution(12);
  pinMode(DAC0, OUTPUT);
}

void loop()
{
  MIDI.read();
  OrganRun();
}

void OutputTimer()
{
  analogWrite(DAC0, ( OrganOutput() >> 3 ) + 2048 ) ;
}

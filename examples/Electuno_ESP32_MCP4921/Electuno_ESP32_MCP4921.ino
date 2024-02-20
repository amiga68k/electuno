/*
Electuno v0.1.3 for ESP32 at 22Khz

FILE: Electuno_ESP32_MCP4921.ino
AUTHOR: Israel Reyes Rodríguez (isRasta)
URL: https://github.com/amiga68k/electuno
MAIL: amiga68k@gmail.com

///////////////////////
//Compilation options//
///////////////////////
*/
#define OUTBITSHIFT 3
#define WAVEMIXMODE 1
#define FREQTUNE 2
#define VOLUMECONTROL 0
#define EXPRESSIONPEDAL 1
#define CHORUS 2
#define CHORUSBUFFERSIZE 5
#define REVERB 1
#define OVERDRIVE 1
#define WAVESIZE 10
#define POLYPHONY  12
#define UPPERMODE 2
#define LOWERMODE 2
#define PEDALMODE 1
#define LESLIE 2
#define LESLIEBUFFERSIZE 10

///////////////////////////
//End compilation options//
///////////////////////////

#include <SPI.h>
#include <MCP_DAC.h>
MCP4911 MCP;
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
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
  upperDrawbar[8]=8;
  
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
  leslieDrumPhase = 10 ; 
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
  leslieHornSpeedFast = 7.3; // Horn speed Hz  
  // leslieDrumSpeedSlow = 0.66; // Drum slow speed Hz
  // leslieDrumSpeedFast = 5.9; // Drum fast speed Hz  
}

void setup() {  
  MySettings();
  MidiSetup();  
  OrganSetup();
  DAC_setup();
  Esp32TimerSetup();  
}

void ICACHE_RAM_ATTR onTimer() //  ICACHE_RAM_ATTR onTimerISR()
{ 
   MCP.fastWriteA(( OrganOutput() >> OUTBITSHIFT ) + 2047 ) ;
}

hw_timer_t *My_timer = NULL;
void Esp32TimerSetup()
{    
    //  Set output frequency
  My_timer = timerBegin(1, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 45, true);
  timerAlarmEnable(My_timer);
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
  SPI.begin();
  MCP.begin(15);
  MCP.fastWriteA(0); 
}

void loop()
{
  MIDI.read();
  OrganRun();
}

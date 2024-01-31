/*
Electuno for ESP32 at 22Khz
Get schematics and more info here:
https://github.com/amiga68k/electuno

///////////////////////
//Compilation options//
///////////////////////

LOWRAM           
  Forces setting of main variables to 8 bits and select fixed 8 popyphony tonegenerator. Avaiable options:                
  Disable by default. No value is required, only write:
  #define LOWRAM

WAVEMIXMODE
  Affects the volume of the drawbars in certain parts of the keyboard. Avaiable options:
  0 = All drawbars and tones have same volume.
  1 = Some tones have atenuation like analog organs. -Default-

FREQTUNE 
  Used to multiply the base number by which the frequencies of musical notes are generated. Avaiable options:
  x = Float number. 1 is for 44100Hz output frequency. 2 for 22050Hz... Etc.
  2 = For a 22050Hz output. -Default- 

VOLUMECONTROL
  Turns the independent upper/lower/pedal volume control on or off. Avaiable options:
  0 = Disable. -Default-
  1 = Enable.
  2 = Enable with fix for 8 bit microcontrollers as 328P.

EXPRESSIONPEDAL
  Set up the expression pedal. Avaiable options:
  0 = Disable. -Default-
  1 = Enable

WAVESIZE
  Configures the size in bits of the sine wave and the arrays that contain the mix samples.
  Theoretically, more is better, but it consumes more memory.Avaiable options:
  7 = 7 bits wave size ( Only tested from 7 to 12 ). -Default-
  
POLYPHONY          
  Configures the polyphony. Avaiable options:
  8 = 8 note polyphony of 14(max). -Default- 

UPPERMODE
  Defines the operation of the upper keyboard. Avaiable options:
  0 = Disable upper keyboard
  1 = Enable only flutes, without percusive harmonics -Default- 
  2 = Enable flutes and percusive keys harmonics

LOWERMODE
  Defines the operation of the lower keyboard. Avaiable options:
  0 = Disable lower keyboard. -Default-
  1 = Enable only fors  four flutes harmonics.
  2 = Enable all flutes harmonics.

PEDALMODE
  Defines the operation of the pedal bass keyboard. Avaiable options:
  0 = Disable bass pedal. -Default-
  1 = Enable with separated dry audio bus as Yamaha Electone style organs
  2 = Enable with upper/lower mixed audio bus. Overdrive, reverb and rotary affects in this mode

CHORUS
Defines the operation of the pedal bass keyboard. Avaiable options:
  0 = Disable chorus vibrato effect. -Default-
  1 = Enable with mixed bus for upper/lower
  2 = Enable with an independent audio bus for each keyboard

CHORUSBUFFERSIZE
Sets the buffer size for the chorus effect, in bits.
  ATTENTION: The size of the buffer for this effect directly affects the tuning of the vibrato effect;
  For this reason there is no real default value, as it depends on the output frequency. Avaiable options:
  4 = 16 steps vibrato effect. -Default-

REVERB
  Enable or disable reverb effect. Avaiable options:
  0 = Disable- -Default-
  1 = Enable reverb effect.

OVERDRIVE
  Enable or disable overdrive effect. Avaiable options:
  0 = Disable. -Default-
  1 = Enable overdrive effect.

LESLIE 
  Sets the rotary effect on or off. Avaiable options:
  0 = Disable. -Default-
  1 = Enable but only one rotor simulation, same as spinet organs internal rotary speaker.
  2 = Enable with two bus of audio for drum/horn speakers, try to simulate external leslie cabinets.

LESLIEBUFFERSIZE
  Sets the buffer size for the rotary effect, in bits.
  ATTENTION: The size of the buffer for this effect directly affects the tuning of the rotary effect.
  Avaiable options:
  7 = 7 bit rotary effect buffer. -Default-
*/

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
   MCP.fastWriteA(( OrganOutput() >> 3 ) + 2047 ) ;
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
  MCP.begin(15);
  MCP.fastWriteA(0); 
}

void loop()
{
  MIDI.read();
  OrganRun();
}

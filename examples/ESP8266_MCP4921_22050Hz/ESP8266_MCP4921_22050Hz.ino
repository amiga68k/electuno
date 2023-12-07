/*
Default MIDI configuration

Upper keyboard listen on channel 1
Lower keyboard listen on channel 2
All controllers on channel 1
68 = Percusion key ON/OFF
69 = Percusion harmonic
71 = Lower vibrato/chorus ON/OFF
72 = Upper vibrato/chorus ON/OFF
73 = Leslie OFF/SLOW/FAST
76 = Reverb
77 = Overdrive
79 = Upper drawbar 1
80 = Upper drawbar 2
81 = Upper drawbar 3
82 = Upper drawbar 4
83 = Upper drawbar 5
84 = Upper drawbar 6
85 = Upper drawbar 7
86 = Upper drawbar 8
87 = Upper drawbar 9
88 = Lower drawbar 1
89 = Lower drawbar 2
90 = Lower drawbar 3
91 = Lower drawbar 4
92 = Lower drawbar 5
93 = Lower drawbar 6
94 = Lower drawbar 7
95 = Lower drawbar 8
96 = Lower drawbar 9
*/

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MCP_DAC.h>
#include <MIDI.h>
MCP4911 MCP;
MIDI_CREATE_DEFAULT_INSTANCE();

//Static settings
#define WAVESIZE 10 // in bits ( 4=16, 5=32, 6=64, etc)
#define POLYPHONY 12 
#define UPPERMODE 2 // 0 = disable, 1 = 9 flutes, 2= 9 flutes+percusion 
#define LOWERMODE 1 // 0 = disable, 1 = 4 flutes, 2= 9 flutes
#define CHORUS 1 // 0=disable, 1=enable
#define CHORUSBUFFERSIZE 6 // in bits ( 4=16, 5=32, 6=64, etc)
#define REVERB 1
#define OVERDRIVE 1
#define LESLIE 2 // 0 = disable, 1 = enable , 2 = full
#define LESLIEBUFFERSIZE 9 // in bits ( 4=16, 5=32, 6=64, etc)
#define LESLIEEQBUFFERSIZE 2 // in bits ( 3=8, 4=16, 5=32, 6=64, etc)
#define LESLIEFILTERSIZE 3 // in bits ( 3=8, 4=16, 5=32, 6=64, etc)

#include <electuno.h>

// Dynamic Settings
void MySettings()
{ 
  upperDrawbar[0]=8;
  upperDrawbar[1]=8;
  upperDrawbar[2]=8;
  upperDrawbar[3]=8;
  upperDrawbar[4]=0;
  upperDrawbar[5]=0;
  upperDrawbar[6]=0;
  upperDrawbar[7]=0;
  upperDrawbar[8]=8;
  
  lowerDrawbar[0]=8;
  lowerDrawbar[1]=8;
  lowerDrawbar[2]=8;
  lowerDrawbar[3]=8;
  
  rotaryValue = 1;
  // loCut = 8 ; // horn input EQ point
   hiCut = 11 ; // drum input EQ point
   drumVol = 7 ; // drum input Gain
   hornVol = 8 ; // horn input GAin
  // chorusSpeed = 6.86; // in Hz
  // upperVibratoSwitch = 0; // 0=Off  1=On
  // vibratoType = 0; // 0=C1  1=V1
  // rotaryValue = 0; // initial rotary status : 0=off  1=slow  2=fast
  // leslieHornOffset = 0; 
  // leslieHornSpeedSlow = 0.83; // Horn slow speed Hz
  // leslieHornSpeedFast = 7.5; // Horn speed Hz
  // leslieHornInertia = 3; // simulates weight of horn speaker (needs fix and optimize for correct simulation)
  // leslieHornMotorTorque = 10; // (needs fix and optimize for correct simulation)
  // leslieDrumSpeedSlow = 0.66; // Drum slow speed Hz
  // leslieDrumSpeedFast = 5.9; // Drum fast speed Hz
  // leslieDrumInertia = 2; // simulates weight of horn speaker (needs fix and optimize for correct simulation)
  // leslieDrumMotorTorque = 2; // 
  finetune = 2; // 1 for 44100Hz
}



void setup() {
  WiFi.mode(WIFI_OFF);
  MySettings();
  MidiSetup();  
  OrganSetup();
  DAC_setup();
  Esp8266TimerSetup();
}
void ICACHE_RAM_ATTR onTimerISR()
{ 
  MCP.fastWriteA( (OrganOutput() + 2047)>>1 ) ;
}
void Esp8266TimerSetup()
{    
    timer1_attachInterrupt(onTimerISR);
    timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
    //timer1_write(6666); //12kHz
    //timer1_write(4999); //16000Hz
    timer1_write(3627); //22050Hz
    //timer1_write(2851); //28050Hz
    //timer1_write(2499); //32000Hz
    //timer1_write(1813); //44100Hz

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

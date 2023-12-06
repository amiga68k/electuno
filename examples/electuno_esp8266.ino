#include <ESP8266WiFi.h>
#include "SPI.h"
#include "MCP_DAC.h"
#include <MIDI.h>
MCP4911 MCP;
MIDI_CREATE_DEFAULT_INSTANCE();

//Static settings
#define CHORUSBUFFERSIZE 6 // in bits ( 4=16, 5=32, 6=64, etc)
//#define LESLIEBUFFERSIZE 256
#define LOWERMODE 1 // 0=disable, 1=simple, 2=complete
#define LESLIEPREBUFFERSIZE 16
#define WAVESIZE 10 // in bits ( 4=16, 5=32, 6=64, etc)
#define POLYPHONY 10 
//#define LESLIEEQBUFFERSIZE 8

#include "electuno.h"

// Dynamic Settings
void MySettings()
{ 
  // chorusSpeed = 6.86; // in Hz
  // upperVibratoSwitch = 0; // 0=Off  1=On
  // vibratoType = 0; // 0=C1  1=V1
  // rotaryValue = 0; // initial rotary status : 0=off  1=slow  2=fast
  // leslieHornOffset = 0; 
  // leslieHornSpeedSlow = 0.83; // Horn slow speed Hz
  // leslieHornSpeedFast = 7.1; // Horn speed Hz
  // leslieHornInertia = 5; // simulates weight of horn speaker (needs fix and optimize for correct simulation)
  // leslieHornMotorTorque = 10; // (needs fix and optimize for correct simulation)
  // leslieDrumSpeedSlow = 0.66; // Drum slow speed Hz
  // leslieDrumSpeedFast = 6.00; // Drum fast speed Hz
  // leslieDrumInertia = 2; // simulates weight of horn speaker (needs fix and optimize for correct simulation)
  // leslieDrumMotorTorque = 2; // 
  finetune = 1 ; //1 for 24Khz
}



void setup() {
  MySettings();
  MidiSetup();  
  OrganSetup();
  DAC_setup();
  Esp8266TimerSetup();
}
void ICACHE_RAM_ATTR onTimerISR()
{ 
  MCP.fastWriteA( OrganOutput() + 2047 ) ;
}
void Esp8266TimerSetup()
{    
    timer1_attachInterrupt(onTimerISR);
    timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
    timer1_write(6666); //12kHz
    //timer1_write(3332); //24kHz
    timer1_write(3330); //24kHz
    //timer1_write(1665); //48khz?

}

void MidiSetup()
{

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
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

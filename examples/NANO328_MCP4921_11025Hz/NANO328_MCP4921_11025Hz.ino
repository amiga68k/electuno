//Electuno for Arduino Nano. 11025HZ

#include "SPI.h"
#define MCP4921_CS_PIN A0
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

//Static settings
#define WAVESIZE 6 // in bits ( 4=16, 5=32, 6=64, etc)
#define POLYPHONY 8 
#define UPPERMODE 1 // 0 = disable, 1 = 9 flutes, 2= 9 flutes+percusion 
#define LOWERMODE 1 // 0 = disable, 1 = 4 flutes, 2= 9 flutes
#define CHORUS 0 // 0=disable, 1=enable
#define CHORUSBUFFERSIZE 4 // in bits ( 4=16, 5=32, 6=64, etc)
#define REVERB 0
#define OVERDRIVE 0
#define LESLIE 1 // 0 = disable, 1 = enable , 2 = full
#define LESLIEBUFFERSIZE 7 // in bits ( 4=16, 5=32, 6=64, etc)
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
  // hiCut = 11 ; // drum input EQ point
  // drumVol = 7 ; // drum input Gain
  // hornVol = 8 ; // horn input GAin
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
  finetune = 4; // 1 for 44100Hz
}

void TimerSetup()
{  

  TCCR1A = 0b00000010;
  TCCR1B = 0b11011001;  

  ICR1 = 1450; //11025HZ
  //ICR1 = 999; //16000Hz
  //ICR1 = 724; //22050Hz
  //ICR1 = 569; //28050Hz
  //ICR1 = 499; //32000Hz
  TIMSK1 |= (1 << TOIE1);
}

void DAC_setup()
{
  pinMode(MCP4921_CS_PIN, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(MCP4921_CS_PIN, HIGH);
  SPI.begin();
}

void setup() {
  MySettings();
  MidiSetup();  
  OrganSetup();
  DAC_setup();
  TimerSetup();
}

void MidiSetup()
{

  MIDI.setHandleNoteOn(electunoNoteOn);
  MIDI.setHandleNoteOff(electunoNoteOff);
  MIDI.setHandleControlChange(electunoControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

void myDac(uint16_t value)
{
  uint16_t data = 0x3000 | value;
  digitalWrite(MCP4921_CS_PIN, LOW);
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer((uint8_t)(data >> 8));
  SPI.transfer((uint8_t)(data & 0xFF));
  SPI.endTransaction();
  digitalWrite(MCP4921_CS_PIN, HIGH);
}

void loop()
{
  MIDI.read();
  OrganRun();
}

ISR(TIMER1_OVF_vect) 
{
  myDac(OrganOutput() + 2047 ) ;
}

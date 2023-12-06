#include "SPI.h"
#define MCP4921_CS_PIN A0
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

//Static settings
// #define CHORUSBUFFERSIZE 5 // in bits ( 4=16, 5=32, 6=64, etc)
// #define LESLIEBUFFERSIZE 7 // in bits ( 4=16, 5=32, 6=64, etc)
// #define WAVESIZE 8 // in bits ( 4=16, 5=32, 6=64, etc)
// #define POLYPHONY 8 

#include "electuno.h"

// Dynamic Settings
void MySettings()
{ 
  // chorusSpeed = 6.86; // in Hz
  // upperVibratoSwitch = 0; // 0=Off  1=On
  // vibratoType = 0; // 0=C1  1=V1
  // rotaryValue = 0; // initial rotary status : 0=off  1=slow  2=fast
  // leslieHornOffset = 0; 
  // leslieHornSpeedSlow = 0.83; // slow speed Hz
  // leslieHornSpeedFast = 6.66; // fast speed Hz
  // leslieHornInertia = 15; // simulates weight of horn speaker (needs fix and optimize for correct simulation)
  // leslieHornMotorTorque = 5; // (needs fix and optimize for correct simulation)
}

void TimerSetup()
{  
  TCCR1A = 0b00000010;
  TCCR1B = 0b11011001;
  ICR1 = 1333; // 12kHz aprox
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

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
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

/*
Electuno for Arduino NANO/UNO/MEGA. 11025HZ

FILE: Electuno_ATMEL328P_MCP4921.ino
AUTHOR: Israel Reyes Rodríguez (isRasta)
URL: https://github.com/amiga68k/electuno
MAIL: amiga68k@gmail.com

///////////////////////
//Compilation options//
///////////////////////
*/
#define OUTBITSHIFT 3
#define LOWRAM
#define WAVEMIXMODE 1
#define FREQTUNE 4
#define EXPRESSIONPEDAL 1
#define CHORUS 1
#define CHORUSBUFFERSIZE 3
#define UPPERMODE 1
#define LESLIE 1

///////////////////////////
//End compilation options//
///////////////////////////


#include "SPI.h"
#define MCP4921_CS_PIN A0
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

  rotaryValue = 0;

  leslieHornVibrato = 8;

///////////////////////////
//User on boot parameters//
///////////////////////////

   leslieHornDeceleration = 200;
   leslieHornAcceleration = 255;
    
  // chorusSpeed = 6.86; // in Hz
  // upperVibratoSwitch = 0; // 0=Off  1=On
  // leslieHornSpeedSlow = 0.83; // Horn slow speed Hz
  // leslieHornSpeedFast = 7.05; // Horn speed Hz  
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
  myDac(( OrganOutput() << OUTBITSHIFT ) + 2047 ) ;
}

# Electuno 0.1.2. Vintage organ simulator

AUTHOR: Israel Reyes Rodríguez (isRasta)
MAIL: amiga68k@gmail.com

This library attempts to simulate a Hammond B3 organ and a Leslie speaker with two cabinets (Drum & horn).
Unlike other organ simulators, the tones are generated from a sinus wave calculated at the start of the performance.
The code only uses integers except in the calculation of the musical note. This is so that it works on CPUs without a floating point unit.
Electuno has a lot of glitches and unwanted noise, but by using some capacitors and resistors at the output of the DAC, it can sound similar to a Hammond organ. 
You can hear it in the sample video.

Demo video: https://youtu.be/f7KLLlDKEW8?si=Gs59R1dpGhj4l2iC



**Requirements:**

- Tested on different cards and architectures:
Arduino NANO, UNO, MEGA (8 bits) and DUE (32 bits).
ESP8266 and ESP32 also working (See example files).

- MCP4921 12 bits DAC. (Optional, but example code has writen for this).

- Midi library. Get from Arduino library manager or from https://github.com/FortySevenEffects/arduino_midi_library.

- MCP_DAC library. Get from Arduino library manager or from https://github.com/RobTillaart/MCP_DAC.

- Arduino Due: Needs DueTimer library from Arduinolibrary manager or get from: https://github.com/ivanseidel/DueTimer.


**Changes in this version:**
-  Fixed problem in accelerating the Leslie speaker simulation.
-  Added more MIDI controls (See list in this document below).
-  Added pedal keyboard.
-  Added independent volume controls for each keyboard.
-  Added support for expression pedal.
-  Added phase modulation for Leslie speaker code.
-  Added new LOWRAM method designed for Atmel328P and similar.
-  Added Arduino Due example test code.
-  Added ESP32 WROOM 32 example test code.


Specifications:
- Sound output through DAC MCP4921 (It is possible to operate it through PWM or other DACs).
- Controllable via MIDI (You need the MIDI library from the Arduino repositories).
- Fully modular to be able to disable functions on low-power CPUs.
- Simulates two independent keyboards (upper & lower) with nine drawbars each.
- Foldback effect on last 2 octaves.
- Percussion effect on the upper keyboard.
- Independent vibrato chorus scanner effect for each keyboard.
- Delay effect to simulate a reverb.
- Overdrive effect.
- Two-way Leslie speaker with different speeds and accelerations.

Only tested with Arduino IDE 1.8.19.

**Arduino NANO/UNO/MEGA**

To use this library on an 8-bit AVR, it is necessary to define LOWRAM.
In this mode the polyphony is limited to 8, and it is only capable of simulating one way of the rotary effect. Because of this, the sound is more similar to a Yamaha Electone organ with its built-in rotary speaker.


**ESP8266 and ESP32 notes**

First you must install support for this MCU in Arduino IDE:
ESP8266: https://www.google.com/search?q=esp8266+on+arduino+ide
ESP32: https://www.google.com/search?q=esp32+on+arduino+ide
Once support for your board is installed, you must install this library from the Arduino library manager;
Just search for 'electuno' and the available versions will appear.
Inside Arduino IDE, go to the menu 'file->examples->electuno->ESP8266xxxxxx'.
Choose the type of board in the 'Tools' menu.
Choose the maximum CPU speed.

See schematics for ESP8266: https://github.com/amiga68k/electuno/blob/main/extras/electuno_schematics_nodeMCU.png

Pin configuration for ESP32 and MCP4921:

	ESP32		MCP4921

	GPIO23	->	SDI(pin4)

	GPIO18	->	SCK(pin3)

	GPIO15	->	CS(pin2)



**COMPILATION OPTIONS:**

**LOWRAM**

  Forces setting of main variables to 8 bits and select fixed 8 popyphony tonegenerator. Avaiable options:                
  Disable by default. No value is required, only write:
   - #define LOWRAM

**WAVEMIXMODE**

  Affects the volume of the drawbars in certain parts of the keyboard. Avaiable options:
 - 0 = All drawbars and tones have same volume.
 - 1 = Some tones have atenuation like analog organs. -Default-

**FREQTUNE**

  Used to multiply the base number by which the frequencies of musical notes are generated. Avaiable options:
 - x = Float number. 1 is for 44100Hz output frequency. 2 for 22050Hz... Etc.
 - 2 = For a 22050Hz output. -Default- 

**VOLUMECONTROL**

  Turns the independent upper/lower/pedal volume control on or off. Avaiable options:
 - 0 = Disable. -Default-
 - 1 = Enable.
 - 2 = Enable with fix for 8 bit microcontrollers as 328P.

**EXPRESSIONPEDAL**

  Set up the expression pedal. Avaiable options:
 - 0 = Disable. -Default-
 - 1 = Enable

**WAVESIZE**

  Configures the size in bits of the sine wave and the arrays that contain the mix samples.
  Theoretically, more is better, but it consumes more memory.Avaiable options:
 - 7 = 7 bits wave size ( Only tested from 7 to 12 ). -Default-
  
**POLYPHONY**

 - Configures the polyphony. Avaiable options:
 - 8 = 8 note polyphony of 16(max). -Default- 

**UPPERMODE**

  Defines the operation of the upper keyboard. Avaiable options:
 - 0 = Disable upper keyboard
 - 1 = Enable only flutes, without percusive harmonics -Default- 
 - 2 = Enable flutes and percusive keys harmonics

**LOWERMODE**

  Defines the operation of the lower keyboard. Avaiable options:
 - 0 = Disable lower keyboard. -Default-
 - 1 = Enable only fors  four flutes harmonics.
 - 2 = Enable all flutes harmonics.

**PEDALMODE**

  Defines the operation of the pedal bass keyboard. Avaiable options:
 - 0 = Disable bass pedal. -Default-
 - 1 = Enable with separated dry audio bus as Yamaha Electone style organs
 - 2 = Enable with upper/lower mixed audio bus. Overdrive, reverb and rotary affects in this mode

**CHORUS**

Defines the operation of the pedal bass keyboard. Avaiable options:
 - 0 = Disable chorus vibrato effect. -Default-
 - 1 = Enable with mixed bus for upper/lower
 - 2 = Enable with an independent audio bus for each keyboard

**CHORUSBUFFERSIZE**

Sets the buffer size for the chorus effect, in bits.
  ATTENTION: The size of the buffer for this effect directly affects the tuning of the vibrato effect;
  For this reason there is no real default value, as it depends on the output frequency. Avaiable options:
 - 4 = 16 steps vibrato effect. -Default-

**REVERB**

  Enable or disable reverb effect. Avaiable options:
 - 0 = Disable- -Default-
 - 1 = Enable reverb effect.

**OVERDRIVE**

  Enable or disable overdrive effect. Avaiable options:
 - 0 = Disable. -Default-
 - 1 = Enable overdrive effect.

**LESLIE**

  Sets the rotary effect on or off. Avaiable options:
 - 0 = Disable. -Default-
 - 1 = Enable but only one rotor simulation, same as spinet organs internal rotary speaker.
 - 2 = Enable with two bus of audio for drum/horn speakers, try to simulate external leslie cabinets.

**LESLIEBUFFERSIZE**

  Sets the buffer size for the rotary effect, in bits.
  ATTENTION: The size of the buffer for this effect directly affects the tuning of the rotary effect.
  Avaiable options:
 - 7 = 7 bit rotary effect buffer. -Default-





**Default MIDI configuration**

**Channel 1 (Upper & effects control):**
-  3   Upper volume
-  11  Expression
-  50  Lower vibrato switch
-  51  Upper vibrato switch
-  54  Overdrive
-  55  Rotary off/slow/fast
-  56  Rotary drum vibrato amount
-  57  Rotary horn vibrato amount  
-  58  Rotary lowpass filter
-  59  Rotary hipass filter
-  80  Enable percusive keys
-  81  Percusive key harmonic (4' & 2'2/3)
-  82  Percusive key decay time (slow & fast )
-  70  Drawbar 16'
-  71  Drawbar 5'1/3
-  72  Drawbar 8'
-  73  Drawbar 4'
-  74  Drawbar 2'2/3
-  75  Drawbar 2'
-  76  Drawbar 1'3/5
-  77  Drawbar 1'1/3
-  78  Drawbar 1'

**Channel 2 (Lower):**
-  3   Lower volume
-  70  Drawbar 16'
-  71  Drawbar 5'1/3
-  72  Drawbar 8'
-  73  Drawbar 4'
-  74  Drawbar 2'2/3
-  75  Drawbar 2'
-  76  Drawbar 1'3/5
-  77  Drawbar 1'1/3
-  78  Drawbar 1'
-  91  Reverb

**Channel 3 (Pedals):**
-  3   Pedal volume
-  70  Drawbar 16'
-  71  Drawbar 5'1/3
-  72  Drawbar 8'
-  73  Drawbar 4'
-  74  Drawbar 2'2/3
-  75  Drawbar 2'
-  76  Drawbar 1'3/5
-  77  Drawbar 1'1/3
-  78  Drawbar 1'

**Schematics**

R4 and C1 must be adjusted to achieve a suitable sound filter according to the output frequency.

For this I have seen the following website: https://www.learningaboutelectronics.com/Articles/Low-pass-filter-calculator.php


Arduino Nano Scheme
![Arduino Nano Scheme](extras/electuno_schematics_nano-uno.png)

NodeMCU esp8266 Scheme
![NodeMCU esp8266 Schemes](extras/electuno_schematics_nodeMCU.png)


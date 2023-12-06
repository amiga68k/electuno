// File "electuno.h"
// Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
// Version 0.1
// Copyright 2019-2023 Israel Reyes Rodríguez.

// This file is part of Electuno organ simulator library.
// Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

//  Version notes:
//    This is a first public version (and my first library).

//  file notes:
//




//uint8_t h1,h2,h3,h4;
// control variables
// chorus vibrato
double chorusSpeed = 6.86;
bool lowerVibratoSwitch;
bool upperVibratoSwitch;
bool vibratoType = 0;
// reverb
uint8_t reverb = 0;
// overdrive
uint8_t overdrive;
// leslie vars
uint8_t leslieDrumVib = 16;
uint8_t leslieHornVib = 16;
uint8_t loCut = 8 ; // horn input EQ point
uint8_t	hiCut = 8 ; // drum input EQ point
uint8_t	drumVol = 16 ; // drum input Gain
uint8_t	hornVol = 16 ; // horn input GAin
uint8_t rotaryValue = 0; // initial rotary status : 0=off  1=slow  2=fast
// Horn related vars
float leslieHornSpeedSlow = 0.515; // slow speed Hz
float leslieHornSpeedFast = 7.1; // fast speed Hz
float leslieHornDeceleration = 1; // simulates weight of horn speaker
float leslieHornAcceleration = 15; // for speedup acceleration
// Drum related vars
float leslieDrumSpeedSlow = 0.66; // slow speed Hz
float leslieDrumSpeedFast = 6.00; // fast speed Hz
float leslieDrumDeceleration = 0; // simulates weight of horn speaker
float leslieDrumAcceleration = 1; // for speedup acceleration

//set default parameters if not set
// Wave
#ifndef WAVESIZE
	#define WAVESIZE 8
#endif

#ifndef POLYPHONY
	#define POLYPHONY 8
#endif

#ifndef LOWERMODE
	#define LOWERMODE 0
#endif

#ifndef UPPERMODE
	#define UPPERMODE 1
#endif

//Effects
#ifndef CHORUS
	#define CHORUS 0
#endif

#ifndef CHORUSBUFFERSIZE
	#define CHORUSBUFFERSIZE 5
#endif

#ifndef REVERB
	#define REVERB 0
#endif

#ifndef REVERBBUFFER
	#define REVERBBUFFER 8192
#endif

#ifndef OVERDRIVE
	#define OVERDRIVE 0
#endif

#ifndef LESLIE
	#define LESLIE 1
#endif

#ifndef LESLIEEQBUFFERSIZE
	#define LESLIEEQBUFFERSIZE 3 //in bits
#endif

#ifndef LESLIEFILTERSIZE
	#define LESLIEFILTERSIZE 3 //in bits
#endif

#ifndef LESLIEBUFFERSIZE
	#define LESLIEBUFFERSIZE 8 // in bits
#endif

//Create variables based on user parameters
#if LOWERMODE == 1
	uint8_t lowerDrawbar[4];
	#elif LOWERMODE == 2
		uint8_t lowerDrawbar[9];
#endif

#if UPPERMODE == 1
	uint8_t upperDrawbar[9];
	bool enableDrawbar8=1;
	#elif UPPERMODE == 2
		bool enableDrawbar8=1;
		uint8_t upperDrawbar[9];
		// Percusive key related variables
		int16_t perc;
		uint8_t percCounter;
		byte percVolume;
		bool percEnable;
		byte harmonic[2] = {4, 6};
		byte percType = harmonic[0];
		// Decay time (in seconds)
		const float percSpeedSlow = 1.0;
		const float percSpeedFast = 0.4;
		uint16_t percSpeed[2] = {
			(1000000*percSpeedSlow)/300,
			(1000000*percSpeedFast)/300
		};
		uint16_t percPeriod = percSpeed[0];
		uint32_t percStartMicros = micros();
		uint32_t percCurrentMicros = micros();

#endif

#include "wavegenerator.h"
#include "tonegenerator.h"
#include "control.h"

#if CHORUS > 0
	#include "chorus.h"
#endif

#if REVERB > 0
	#include "reverb.h"
#endif

#if OVERDRIVE > 0
	#include "overdrive.h"
#endif

#if LESLIE > 0
	#include "leslie.h"
#endif


void WaveSetup()
{
	int16_t s = ((sizeof(sine) / sizeof(sine[0]))-1)>>1;
	for ( int i = 0 ; i < s ; i++)
	{
		sine[i] = (sin(i * PI / s )) * s;
	}
	for ( int i = 0 ; i < s ; i++)
	{
		sine[i + s ] = (sin(i * PI / s )) * -s;
	}
}

void OrganSetup()
{
	WaveSetup();
}

void OrganRun()
{
	WaveMix();

}

int OrganOutput()
{
	ToneGenerator();

	#if CHORUS > 0
		Chorus();
	#endif

	#if OVERDRIVE > 0
		OverDrive();
	#endif

	#if REVERB > 0
		Reverb();
	#endif

	#if LESLIE > 0
		if ( rotaryValue > 0 )
		{
			Leslie();
		}
	#endif

	return int16_t(mainOut);
}

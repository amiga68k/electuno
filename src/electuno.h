/*
File "electuno.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.12 
Copyright 2019-2023 Israel Reyes Rodríguez.

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	The variables have been moved to a new file: varsetup.h.
	Some changes in OrganSetup() and OrganOutput().	

Problems:

*/

#ifndef WAVEMIXMODE
	#define WAVEMIXMODE 1
#endif
  
#ifndef FREQTUNE
	#define FREQTUNE 2
#endif

#ifndef VOLUMECONTROL
	#define VOLUMECONTROL 0
#endif

#ifndef EXPRESSIONPEDAL
	#define EXPRESSIONPEDAL 0
#endif

#ifndef WAVESIZE
	#define WAVESIZE 7
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

#ifndef PEDALMODE
	#define PEDALMODE 0
#endif

#ifndef CHORUS
	#define CHORUS 0
#endif

#ifndef CHORUSBUFFERSIZE
	#define CHORUSBUFFERSIZE 4
#endif

#ifndef REVERB
	#define REVERB 0
#endif

#ifndef REVERBBUFFERSIZE
	#define REVERBBUFFERSIZE 13
#endif

#ifndef OVERDRIVE
	#define OVERDRIVE 0
#endif

#ifndef LESLIE
	#define LESLIE 0
#endif

#ifndef LESLIEBUFFERSIZE
	#define LESLIEBUFFERSIZE 7 // in bits
#endif

#include "varsetup.h"
#include "control.h"
#include "wavegenerator.h"
#include "tonegenerator.h"

#if CHORUS > 0
	#include "chorustimer.h"
	#include "chorus.h"
#else
	void ChorusTimer(){};
	void Chorus(){};
#endif

#if REVERB > 0
	#include "reverb.h"
#else
	void Reverb(){};
#endif

#if OVERDRIVE > 0
	#include "overdrive.h"
#else
	void OverDrive(){};
#endif

#if LESLIE > 0
	#include "leslietimer.h"
	#include "leslie.h"	
#else
	void LeslieTimer(){};
	void Leslie(){};
#endif

#if EXPRESSIONPEDAL == 1	// Working with Esp8266 but not work on arduino Mega/Nano
	void ExpressionPedal()
	{
		mainOut = ( mainOut * expressionPedal) >> 7 ;
	}
#elif EXPRESSIONPEDAL == 2	// Working for all, but has bad quality output. This is a FIX for 8 bit AVRs
	void ExpressionPedal()
	{
		mainOut = ( mainOut >> 7 ) * expressionPedal ;
	}
#else
	void ExpressionPedal(){};
#endif

#if VOLUMECONTROL > 0
	void VolumeControl()
	{
		upperOut = ( upperOut * upperVolume ) >> 7;
		lowerOut = ( lowerOut * lowerVolume ) >> 7;
		pedalOut = ( pedalOut * pedalVolume ) >> 7;
	}
#else
	void VolumeControl(){};
#endif


#if PEDALMODE == 2
	void BassOutMixed()
	{
		mainOut += pedalOut ;
	};
	void BassOutClean(){};
#elif PEDALMODE == 1
	void BassOutMixed(){};
	void BassOutClean()
	{
		mainOut += pedalOut ;
	};
#elif PEDALMODE == 0
	void BassOutMixed(){};
	void BassOutClean(){};
#endif


void OrganSetup()
{
	WaveSetup();
}

void OrganRun()
{	
	ChorusTimer();
	LeslieTimer();
	WaveMix();
}

int OrganOutput()
{
	LeslieTimer();
	ToneGenerator();
	VolumeControl();
	Chorus();
	BassOutMixed();
	ExpressionPedal();
	OverDrive();
	Reverb();
	Leslie();
	BassOutClean();

	return int16_t( mainOut );
}
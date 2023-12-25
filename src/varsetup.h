/*
File "varsetup.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.12
Copyright 2019-2023 Israel Reyes Rodríguez.

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	This is new file with all initial variable declarations

Problems:
*/

#ifndef LOWRAM
	//	Wave generator variables
	const uint8_t waveMode = 0 ; 
	const int16_t waveLength = (1 << WAVESIZE) ;
	int16_t sine[1 << WAVESIZE];
	int16_t upperWave1[waveLength];
	int16_t upperWave2[waveLength];
	int16_t upperWave3[waveLength];
	int16_t lowerWave1[waveLength];
	int16_t lowerWave2[waveLength];
	int16_t lowerWave3[waveLength];
	int16_t pedalWave[waveLength];
	int16_t* wave[7] = {upperWave1, upperWave2, upperWave3, lowerWave1, lowerWave2, lowerWave3, pedalWave};
	int16_t perc;
	uint8_t percCounter;
	uint8_t percVolume;
	bool percEnable;
	uint8_t harmonic[2] = {4, 6};
	uint8_t percType = harmonic[0];
	const float percSpeedSlow = 1.0;
	const float percSpeedFast = 0.4;
	uint16_t percSpeed[2] = {( 1000000 * percSpeedSlow ) / 300 , ( 1000000 * percSpeedFast ) / 300 };
	uint16_t percPeriod = percSpeed[0];
	uint32_t percStartMicros = micros();
	uint32_t percCurrentMicros = micros();
	uint8_t lowerDrawbar[9] = { 8, 8, 8, 8, 8, 8, 8, 8, 8 };
	uint8_t upperDrawbar[9] = { 8, 8, 8, 8, 8, 8, 8, 8, 8 };
	uint8_t pedalDrawbar[9] = { 8, 8, 8, 8, 8, 8, 8, 8, 8 };
	bool enableDrawbar8=1;

//	Tone generator variables

	float finetune	= 1;
	uint8_t  noteChannel[POLYPHONY];
	uint8_t freeChannel = 0;
	uint16_t channelFreq[POLYPHONY];
	uint16_t oscillators[POLYPHONY];
	uint8_t audioChannel[POLYPHONY];
	uint8_t waveNumber[POLYPHONY];

// Chorus vibrato variables

	float chorusSpeed = 6.86;	//	chorus speed in Hz
	bool lowerVibratoSwitch;	//	lower vibrato on/off (0/1)
	bool upperVibratoSwitch;	//	lower vibrato in/off (0/1)
	bool vibratoType = 0;

//	Effects and volume variables

	int16_t mainOut;
	int16_t lowerOut;
	int16_t upperOut;
	int16_t pedalOut;
	uint8_t expressionPedal = 127 ;
	uint8_t lowerVolume = 127 ;
	uint8_t upperVolume = 127 ;
	uint8_t pedalVolume = 127 ;
	uint8_t reverb = 0;
	uint8_t overdrive;

	//	Leslie simulator variables
	int16_t leslieBuffer[1<<LESLIEBUFFERSIZE];	//	create single buffer for all
	int16_t leslieDrumBuffer[1<<LESLIEBUFFERSIZE];   // create Drum buffer
	int16_t leslieHornBuffer[1<<LESLIEBUFFERSIZE];   // create Horn buffer
	uint8_t rotaryValue = 0; // initial rotary status : 0=off  1=slow  2=fast
	uint8_t	leslieHipassFilter = 8 ; // horn hipass filter
	uint8_t leslieHornPhase = 10;	// horn phase 
	uint8_t	leslieHornVolume = 16 ; // horn output volume
	uint8_t leslieHornVibrato = 10;	// horn vibrato 
	float leslieHornSpeedSlow = 0.515; // horn slow speed Hz
	float leslieHornSpeedFast = 7.2; // horn fast speed Hz
	uint8_t leslieHornDeceleration = 127; // simulates weight of horn speaker
	uint8_t leslieHornAcceleration = 127; // for speedup acceleration
	uint8_t leslieLowpassFilter = 8 ; // drum lowpass filter
	uint8_t	leslieDrumVolume = 16 ; // drum output volume
	uint8_t leslieDrumPhase = 10;	// horn Phase 
	uint8_t leslieDrumVibrato = 10; // drum vibrato
	float leslieDrumSpeedSlow = 0.66; // drum slow speed Hz
	float leslieDrumSpeedFast = 6.00; // drum fast speed Hz
	uint8_t leslieDrumDeceleration = 1; // simulates weight of drum speaker
	uint8_t leslieDrumAcceleration = 2; // for speedup acceleration

#else
	//TEST FOR 8 BIT
	//	Wave generator variables
	const uint8_t waveMode = 6 ; 
	const int16_t waveLength = (1 << WAVESIZE) ;
	int16_t sine[1 << WAVESIZE];
	int8_t upperWave1[waveLength];
	int8_t upperWave2[waveLength];
	int8_t upperWave3[waveLength];
	int8_t lowerWave1[waveLength];
	int8_t lowerWave2[waveLength];
	int8_t lowerWave3[waveLength];
	int8_t pedalWave[waveLength];
	int8_t* wave[7] = {upperWave1, upperWave2, upperWave3, lowerWave1, lowerWave2, lowerWave3, pedalWave};
	int8_t perc;
	uint8_t percCounter;
	uint8_t percVolume;
	bool percEnable;
	uint8_t harmonic[2] = {4, 6};
	uint8_t percType = harmonic[0];
	const float percSpeedSlow = 1.0;
	const float percSpeedFast = 0.4;
	uint16_t percSpeed[2] = {( 1000000 * percSpeedSlow ) / 300 , ( 1000000 * percSpeedFast ) / 300 };
	uint16_t percPeriod = percSpeed[0];
	uint32_t percStartMicros = micros();
	uint32_t percCurrentMicros = micros();
	uint8_t lowerDrawbar[9] = { 8, 8, 8, 8, 8, 8, 8, 8, 8 };
	uint8_t upperDrawbar[9] = { 8, 8, 8, 8, 8, 8, 8, 8, 8 };
	uint8_t pedalDrawbar[9] = { 8, 8, 8, 8, 8, 8, 8, 8, 8 };
	bool enableDrawbar8=1;

	//	Tone generator variables

	float finetune	= 1;
	uint8_t  noteChannel[POLYPHONY];
	uint8_t freeChannel = 0;
	uint16_t channelFreq[POLYPHONY];
	uint16_t oscillators[POLYPHONY];
	uint8_t audioChannel[POLYPHONY];
	uint8_t waveNumber[POLYPHONY];

	// Chorus vibrato variables

	float chorusSpeed = 6.86;	//	chorus speed in Hz
	bool lowerVibratoSwitch;	//	lower vibrato on/off (0/1)
	bool upperVibratoSwitch;	//	lower vibrato in/off (0/1)
	bool vibratoType = 0;

	//	Effects and volume variables

	int8_t mainOut;
	int8_t lowerOut;
	int8_t upperOut;
	int8_t pedalOut;
	uint8_t expressionPedal = 127 ;
	uint8_t lowerVolume = 127 ;
	uint8_t upperVolume = 127 ;
	uint8_t pedalVolume = 127 ;
	uint8_t reverb = 0;
	uint8_t overdrive;

	//	Leslie simulator variables
	int8_t leslieBuffer[1<<LESLIEBUFFERSIZE];	//	create single buffer for all
	int8_t leslieDrumBuffer[1<<LESLIEBUFFERSIZE];   // create Drum buffer
	int8_t leslieHornBuffer[1<<LESLIEBUFFERSIZE];   // create Horn buffer
	uint8_t rotaryValue = 0; // initial rotary status : 0=off  1=slow  2=fast
	uint8_t	leslieHipassFilter = 8 ; // horn hipass filter
	uint8_t leslieHornPhase = 10;	// horn phase 
	uint8_t	leslieHornVolume = 16 ; // horn output volume
	uint8_t leslieHornVibrato = 10;	// horn vibrato 
	float leslieHornSpeedSlow = 0.515; // horn slow speed Hz
	float leslieHornSpeedFast = 7.2; // horn fast speed Hz
	uint8_t leslieHornDeceleration = 127; // simulates weight of horn speaker
	uint8_t leslieHornAcceleration = 127; // for speedup acceleration
	uint8_t leslieLowpassFilter = 8 ; // drum lowpass filter
	uint8_t	leslieDrumVolume = 16 ; // drum output volume
	uint8_t leslieDrumPhase = 10;	// horn Phase 
	uint8_t leslieDrumVibrato = 10; // drum vibrato
	float leslieDrumSpeedSlow = 0.66; // drum slow speed Hz
	float leslieDrumSpeedFast = 6.00; // drum fast speed Hz
	uint8_t leslieDrumDeceleration = 1; // simulates weight of drum speaker
	uint8_t leslieDrumAcceleration = 2; // for speedup acceleration

#endif
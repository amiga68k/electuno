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

// file notes:


////////////



uint8_t leslieHornOffset;
uint16_t hornChorus1, hornChorus2, drumChorus1, drumChorus2;
const uint16_t leslieBufferSize=(1<<LESLIEBUFFERSIZE);
const uint8_t leslieEQBufferSize=(1<<LESLIEEQBUFFERSIZE);
const uint8_t leslieFilterSize=(1<<LESLIEFILTERSIZE);

void LeslieTimer(){
	#if LESLIE > 1
//Leslie Drum timer
		static uint16_t rotaryStimatedDrumSpeed[3] =
		{
			65535,
			(1000000/leslieDrumSpeedSlow)/(leslieBufferSize),
			(1000000/leslieDrumSpeedFast)/(leslieBufferSize)
		};
		static uint32_t leslieDrumStartMicros = micros();
		static uint16_t rotaryDrumSpeed  = rotaryStimatedDrumSpeed[1];
		static uint16_t leslieDrumCurrentMicros, RotaryDrumStep;

		leslieDrumCurrentMicros = micros();
		if ( rotaryValue != 0 and leslieDrumCurrentMicros - leslieDrumStartMicros >= rotaryDrumSpeed )
		{
			if (rotaryDrumSpeed < rotaryStimatedDrumSpeed[0])
			{
				if (RotaryDrumStep < leslieBufferSize)
				{
					RotaryDrumStep++;
				}else{
					RotaryDrumStep = 0;
				}
				if (RotaryDrumStep < leslieBufferSize>>1){
					drumChorus1 = ((1+RotaryDrumStep)*leslieDrumVib)>>4;
				}else{
					drumChorus1 = ((1+leslieBufferSize-RotaryDrumStep)*leslieDrumVib)>>4;
				}
				drumChorus2 = 1+(drumChorus1>>1);
			}
			leslieDrumStartMicros = leslieDrumCurrentMicros;
		}
// Drum acceleration timer
		static uint16_t leslieDrumInertiaPeriod = 10;
		static uint32_t leslieDrumInertiaStartMillis = millis();
		static uint32_t leslieDrumInertiaCurrentMillis;

		leslieDrumInertiaCurrentMillis = millis();
		if (rotaryValue != 0 and leslieDrumInertiaCurrentMillis - leslieDrumInertiaStartMillis >= leslieDrumInertiaPeriod )
		{
			if (rotaryDrumSpeed > rotaryStimatedDrumSpeed[rotaryValue])
			{
				rotaryDrumSpeed--;
				rotaryDrumSpeed-=leslieDrumAcceleration;
			}
			if (rotaryDrumSpeed < rotaryStimatedDrumSpeed[rotaryValue])
			{
				rotaryDrumSpeed++;
				rotaryDrumSpeed+=leslieDrumDeceleration;
			}
			leslieDrumInertiaStartMillis = leslieDrumInertiaCurrentMillis;
		}
	#endif

	#if LESLIE > 0
// Leslie horn timer
		static uint16_t rotaryStimatedHornSpeed[3] =
			{ 65535,
				(1000000/leslieHornSpeedSlow)/(leslieBufferSize),
				(1000000/leslieHornSpeedFast)/(leslieBufferSize)
			};
		static uint32_t leslieHornStartMicros = micros();
		static uint16_t rotaryHornSpeed  = rotaryStimatedHornSpeed[1];
		static uint16_t leslieHornCurrentMicros, RotaryHornStep;

		leslieHornCurrentMicros = micros();
		if ( rotaryValue != 0 and leslieHornCurrentMicros - leslieHornStartMicros >= rotaryHornSpeed )
		{
			if (rotaryHornSpeed < rotaryStimatedHornSpeed[0])
			{
				if (RotaryHornStep < leslieBufferSize)
				{
					RotaryHornStep++;
				}else{
					RotaryHornStep = 0;
				}
				if (RotaryHornStep < leslieBufferSize>>1){
					hornChorus1 = ((1+RotaryHornStep)*leslieHornVib)>>4;
				}else{
					hornChorus1 = ((1+leslieBufferSize-RotaryHornStep)*leslieHornVib)>>4;
				}
				hornChorus2 = 1+(hornChorus1>>1);
			}
			leslieHornStartMicros = leslieHornCurrentMicros;
		}

// Horn acceleration timer
		static uint16_t leslieHornInertiaPeriod = 10;
		static uint32_t leslieHornInertiaStartMillis = millis();
		static uint32_t leslieHornInertiaCurrentMillis;

		leslieHornInertiaCurrentMillis = millis();
		if (rotaryValue != 0 and leslieHornInertiaCurrentMillis - leslieHornInertiaStartMillis >= leslieHornInertiaPeriod )
		{
			if (rotaryHornSpeed > rotaryStimatedHornSpeed[rotaryValue])
			{
				rotaryHornSpeed--;
				rotaryHornSpeed-=leslieHornAcceleration;
			}
			if (rotaryHornSpeed < rotaryStimatedHornSpeed[rotaryValue])
			{
				rotaryHornSpeed++;
				rotaryHornSpeed+=leslieHornDeceleration;
			}

			leslieHornInertiaStartMillis = leslieHornInertiaCurrentMillis;
		}
	#endif
}

void Leslie()
{
	LeslieTimer();
	#if LESLIE == 1

		static int16_t leslieBuffer[leslieBufferSize];   // create simple buffer
		static uint16_t leslieCounter;
		int16_t leslieOut;

		if ( leslieCounter > leslieBufferSize-1 ){ leslieCounter = 0; }
		leslieBuffer[leslieCounter] = mainOut;
		leslieCounter++;
		leslieBuffer[leslieCounter] = mainOut;
		leslieCounter++;
		leslieBuffer[leslieCounter] = mainOut;
		leslieCounter++;
		leslieBuffer[leslieCounter] = mainOut;
		leslieCounter++;

		leslieOut=
		(
			leslieBuffer[leslieCounter + hornChorus1  & (leslieBufferSize-1)]
			+
			leslieBuffer[leslieCounter + hornChorus1 +1 & (leslieBufferSize-1)]
			+
			leslieBuffer[leslieCounter + hornChorus1 +2 & (leslieBufferSize-1)]
			+
			leslieBuffer[leslieCounter + hornChorus1 +3 & (leslieBufferSize-1)]
			+
			leslieBuffer[leslieCounter - hornChorus2  & (leslieBufferSize-1)]
			+
			leslieBuffer[leslieCounter - hornChorus2 -1 & (leslieBufferSize-1)]
			+
			leslieBuffer[leslieCounter - hornChorus2 -2 & (leslieBufferSize-1)]
			+
			leslieBuffer[leslieCounter - hornChorus2 -3 & (leslieBufferSize-1)]
		);

		mainOut = leslieOut>>2;

	#elif LESLIE == 2

		static int16_t drumOut;
		static int16_t hornOut;
		static uint16_t leslieEQCounter;
		static uint16_t leslieCounter;
		static int16_t leslieDrumBuffer[leslieBufferSize];   // create Drum buffer
		static int16_t leslieHornBuffer[leslieBufferSize];   // create Horn buffer
		static int16_t sampleBuffer[4];
		static uint8_t sampleStep;

		sampleStep++;
		if (sampleStep > 3){ sampleStep = 0; }
		sampleBuffer[sampleStep] = mainOut ;
		int16_t tempSample = ( sampleBuffer[sampleStep-1 & 3] + sampleBuffer[sampleStep-2 & 3] + sampleBuffer[sampleStep-3 & 3] )>>2;
		int16_t lowfreqSample = (( mainOut + (( tempSample * loCut ) >>3 ))*( 16 - loCut )) >>5;
		int16_t hifreqSample =  (( mainOut - (( tempSample * hiCut ) >>3 ))*( 8 + hiCut )) >>4;

		for (uint8_t i; i < leslieFilterSize; i++) ///Revisar este for. Creo que no hace falta aquí. Está más abajo.
		{
			leslieCounter++;
			if ( leslieCounter > leslieBufferSize ){ leslieCounter = 0; }
			leslieDrumBuffer[leslieCounter] = lowfreqSample;
			leslieHornBuffer[leslieCounter] = hifreqSample;
		}
  
		drumOut = hornOut = 0 ;

		for (uint8_t i; i < leslieFilterSize; i++)
		{
			drumOut+=
				(
					leslieDrumBuffer[leslieCounter + drumChorus1 + i & (leslieBufferSize-1)]
					+
					leslieDrumBuffer[leslieCounter - drumChorus2 - i  & (leslieBufferSize-1)]
				)>>2
			;
			hornOut+=
				(
					leslieHornBuffer[leslieCounter + hornChorus1 + i & (leslieBufferSize-1)]
					+
					leslieHornBuffer[leslieCounter - hornChorus2 - i & (leslieBufferSize-1)]
				)>>2
			;
		}

		mainOut = 
			((drumOut*drumVol)>>4)
			+
			((hornOut*hornVol)>>4)
		;
	#endif
}






/*
File "chorus.h"
Author: Israel Reyes Rodríguez (isRasta)
Url: https://github.com/amiga68k/electuno
Mail: amiga68k@gmail.com

Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.13
Copyright 2019-2024 Israel Reyes Rodríguez (amiga68k@gmail.com).

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	Added LeslieTimer() function to this file
	Rewrite two Leslie() function, again.
	FIXED:Acceleration was a bug when the buffer size changes
	FIXED:The RPM calculation was wrong.

Problems:
	High pass filter generates a lot of noise.
	Horn part generates a lot of noise


*/


uint16_t hornChorus1, hornChorus2, drumChorus1, drumChorus2;
const uint16_t leslieBufferSize=(1<<LESLIEBUFFERSIZE) - 1 ;
const uint16_t leslieTimerSize=(1<<LESLIEBUFFERSIZE) ;


#if LESLIE == 1

	void LeslieTimer()
	{
	// Leslie horn timer

		static uint16_t rotaryStimatedHornSpeed[3] =
			{ 65535 ,
				( 1000000 / leslieHornSpeedSlow ) / ( leslieBufferSize ),
				( 1000000 / leslieHornSpeedFast ) / ( leslieBufferSize )
			};
		static uint32_t leslieHornStartMicros ;
		static uint32_t rotaryHornSpeed  = rotaryStimatedHornSpeed[1] ;
		static uint32_t leslieHornCurrentMicros, RotaryHornStep ;

		leslieHornCurrentMicros = Micros ;
		if ( rotaryValue != 0 and leslieHornCurrentMicros - leslieHornStartMicros >= rotaryHornSpeed )
		{
			if ( rotaryHornSpeed < rotaryStimatedHornSpeed[0] )
			{
				if ( RotaryHornStep < leslieTimerSize )
				{
					RotaryHornStep++;
				}else{
					RotaryHornStep = 0;
				}
				if ( RotaryHornStep < leslieTimerSize>>1 )
				{
					hornChorus1 = (( RotaryHornStep ) * leslieHornVibrato ) >> 4 ;
				}else{
					hornChorus1 = (( leslieTimerSize - RotaryHornStep ) * leslieHornVibrato ) >> 4 ;
				}
				hornChorus2 = ( LESLIEBUFFERSIZE >> 1 ) + ( hornChorus1 >> 1 ) ;
				hornChorus1 = ( LESLIEBUFFERSIZE >> 1 ) + hornChorus1 ;
			}
			leslieHornStartMicros = leslieHornCurrentMicros;
		}

	// Horn acceleration timer

		static uint32_t leslieHornInertiaPeriod = 200000 ;
		static uint32_t leslieHornInertiaStartMillis ;
		static uint32_t leslieHornInertiaCurrentMillis ;

		leslieHornInertiaCurrentMillis = Micros ;
		if ( leslieHornInertiaCurrentMillis - leslieHornInertiaStartMillis >= leslieHornInertiaPeriod )
		{
			switch (rotaryValue)
			{
				case 1:
					if ( rotaryHornSpeed < rotaryStimatedHornSpeed[rotaryValue] )
					{
						rotaryHornSpeed += ( 127 + ( leslieHornDeceleration )) >> ( LESLIEBUFFERSIZE - 7 ) ;
					}
					if ( rotaryHornSpeed > rotaryStimatedHornSpeed[rotaryValue] )
					{
						rotaryHornSpeed = rotaryStimatedHornSpeed[rotaryValue];
					}
				break ;

				case 2:
					if (rotaryHornSpeed > rotaryStimatedHornSpeed[rotaryValue])
					{			
						rotaryHornSpeed -= ( 127 + ( leslieHornAcceleration << 2 )) >> ( LESLIEBUFFERSIZE - 7 ) ;
					}
					if (rotaryHornSpeed < rotaryStimatedHornSpeed[rotaryValue])
					{
					rotaryHornSpeed = rotaryStimatedHornSpeed[rotaryValue];
					}
				break;
			}
		
			leslieHornInertiaStartMillis = leslieHornInertiaCurrentMillis;
		}
	}

	void Leslie()
	{	
		static uint16_t leslieCounter;
		int16_t leslieOut;

		if ( rotaryValue != 0 )
		{
			if ( leslieCounter > leslieBufferSize ){ leslieCounter = 0; }
			uint8_t mainOut8 = mainOut >> 3 ;
			leslieBuffer[leslieCounter] = mainOut8 ;
			leslieCounter++;
			leslieBuffer[leslieCounter] = mainOut8 ;
			leslieCounter++;
			leslieBuffer[leslieCounter] = mainOut8 ;
			leslieCounter++;
			leslieBuffer[leslieCounter] = mainOut8 ;
			leslieCounter++;

			mainOut =
				(
					leslieBuffer[leslieCounter - hornChorus1  & leslieBufferSize]
					+
					leslieBuffer[leslieCounter + hornChorus2  & leslieBufferSize]
					+
					leslieBuffer[leslieCounter - hornChorus1 -1  & leslieBufferSize]
					+
					leslieBuffer[leslieCounter + hornChorus2 +1 & leslieBufferSize]
					+
					leslieBuffer[leslieCounter - hornChorus1 -2  & leslieBufferSize]
					+
					leslieBuffer[leslieCounter + hornChorus2 +2 & leslieBufferSize]
					+
					leslieBuffer[leslieCounter - hornChorus1 -3  & leslieBufferSize]
					+
					leslieBuffer[leslieCounter + hornChorus2 +3 & leslieBufferSize]
				)
			;
		}
	}

#elif LESLIE == 2

	void LeslieTimer()
	{
		uint32_t microSeconds = micros() ;

	//Leslie Drum timer
		static uint16_t rotaryStimatedDrumSpeed[3] =
		{
			65535,
			(1000000/leslieDrumSpeedSlow)/(leslieBufferSize),
			(1000000/leslieDrumSpeedFast)/(leslieBufferSize)
		};
		static uint32_t leslieDrumStartMicros ;
		static uint32_t rotaryDrumSpeed  = rotaryStimatedDrumSpeed[1];
		static uint32_t leslieDrumCurrentMicros, RotaryDrumStep;

		leslieDrumCurrentMicros = Micros ;
		if ( rotaryValue != 0 and leslieDrumCurrentMicros - leslieDrumStartMicros >= rotaryDrumSpeed )
			{
			if (rotaryDrumSpeed < rotaryStimatedDrumSpeed[0])
			{
				if (RotaryDrumStep < leslieTimerSize)
				{
					RotaryDrumStep++;
				}else{
					RotaryDrumStep = 0;
				}
				if (RotaryDrumStep < leslieTimerSize>>1){
					drumChorus1 = ((1+RotaryDrumStep)*leslieDrumVibrato)>>4;
				}else{
					drumChorus1 = (( leslieTimerSize - RotaryDrumStep ) * leslieDrumVibrato ) >> 4 ;
				}
				drumChorus2 = ( LESLIEBUFFERSIZE >> 1 ) + ( drumChorus1 >> 1 ) ;
				drumChorus1 = ( LESLIEBUFFERSIZE >> 1 ) + drumChorus1 ;
			}
			leslieDrumStartMicros = leslieDrumCurrentMicros;
		}

	// Drum acceleration timer

		static uint32_t leslieDrumInertiaPeriod = 200000 ;
		static uint32_t leslieDrumInertiaStartMillis ;
		static uint32_t leslieDrumInertiaCurrentMillis ;

		leslieDrumInertiaCurrentMillis = Micros ;
		if ( leslieDrumInertiaCurrentMillis - leslieDrumInertiaStartMillis >= leslieDrumInertiaPeriod )
		{
			switch (rotaryValue)
			{
				case 1:
					if ( rotaryDrumSpeed < rotaryStimatedDrumSpeed[rotaryValue] )
					{
						rotaryDrumSpeed += ( 127 + ( leslieDrumDeceleration )) >> ( LESLIEBUFFERSIZE - 7 ) ;
					}
					if ( rotaryDrumSpeed > rotaryStimatedDrumSpeed[rotaryValue] )
					{
						rotaryDrumSpeed = rotaryStimatedDrumSpeed[rotaryValue];
					}
				break ;

				case 2:
					if (rotaryDrumSpeed > rotaryStimatedDrumSpeed[rotaryValue])
					{			
						rotaryDrumSpeed -= ( 127 + ( leslieDrumAcceleration << 2 )) >> ( LESLIEBUFFERSIZE - 7 ) ;
					}
					if (rotaryDrumSpeed < rotaryStimatedDrumSpeed[rotaryValue])
					{
					rotaryDrumSpeed = rotaryStimatedDrumSpeed[rotaryValue];
					}
				break;
			}
			leslieDrumInertiaStartMillis = leslieDrumInertiaCurrentMillis;
		}

	// Leslie horn timer

		static uint16_t rotaryStimatedHornSpeed[3] =
			{ 65535 ,
				( 1000000 / leslieHornSpeedSlow ) / ( leslieBufferSize ),
				( 1000000 / leslieHornSpeedFast ) / ( leslieBufferSize )
			};
		static uint32_t leslieHornStartMicros ;
		static uint32_t rotaryHornSpeed  = rotaryStimatedHornSpeed[1] ;
		static uint32_t leslieHornCurrentMicros, RotaryHornStep ;

		leslieHornCurrentMicros = Micros ;
		if ( rotaryValue != 0 and leslieHornCurrentMicros - leslieHornStartMicros >= rotaryHornSpeed )
		{
			if ( rotaryHornSpeed < rotaryStimatedHornSpeed[0] )
			{
				if ( RotaryHornStep < leslieTimerSize )
				{
					RotaryHornStep++;
				}else{
					RotaryHornStep = 0;
				}
				if ( RotaryHornStep < leslieTimerSize>>1 )
				{
					hornChorus1 = (( RotaryHornStep ) * leslieHornVibrato ) >> 4 ;
				}else{
					hornChorus1 = (( leslieTimerSize - RotaryHornStep ) * leslieHornVibrato ) >> 4 ;
				}
				hornChorus2 = ( LESLIEBUFFERSIZE >> 1 ) + ( hornChorus1 >> 1 ) ;
				hornChorus1 = ( LESLIEBUFFERSIZE >> 1 ) + hornChorus1 ;
			}
			leslieHornStartMicros = leslieHornCurrentMicros;
		}

	// Horn acceleration timer

		static uint32_t leslieHornInertiaPeriod = 150000 ;
		static uint32_t leslieHornInertiaStartMillis ;
		static uint32_t leslieHornInertiaCurrentMillis ;

		leslieHornInertiaCurrentMillis = Micros ;
		if ( leslieHornInertiaCurrentMillis - leslieHornInertiaStartMillis >= leslieHornInertiaPeriod )
		{
			switch (rotaryValue)
			{
				case 1:
					if ( rotaryHornSpeed < rotaryStimatedHornSpeed[rotaryValue] )
					{
						rotaryHornSpeed += ( 127 + ( leslieHornDeceleration )) >> ( LESLIEBUFFERSIZE - 7 ) ;
					}
					if ( rotaryHornSpeed > rotaryStimatedHornSpeed[rotaryValue] )
					{
						rotaryHornSpeed = rotaryStimatedHornSpeed[rotaryValue];
					}
				break ;

				case 2:
					if (rotaryHornSpeed > rotaryStimatedHornSpeed[rotaryValue])
					{			
						rotaryHornSpeed -= ( 127 + ( leslieHornAcceleration << 2 )) >> ( LESLIEBUFFERSIZE - 7 ) ;
					}
					if (rotaryHornSpeed < rotaryStimatedHornSpeed[rotaryValue])
					{
					rotaryHornSpeed = rotaryStimatedHornSpeed[rotaryValue];
					}
				break;
			}
		
			leslieHornInertiaStartMillis = leslieHornInertiaCurrentMillis;
		}
	}

	void Leslie()
	{
		
		if ( rotaryValue != 0 )
		{

			static int32_t drumPhaseSample ;
			static int32_t hornPhaseSample ;
			static int32_t drumOut ;
			static int32_t hornOut ;

//	High and low pass filter
//	**NOTE**	At the moment it has fixed values, surely the sound will change when using another output frequency other than 22050Hz.
			static const int8_t eqsb = 4 ;	//	Equalizer sample buffer size, in bits
			static const int8_t eqsbs = ( 1 << eqsb ) - 1 ;
			static int16_t sampleBuffer[ 1 << eqsb ] ;
			static uint8_t sampleStep ;

			sampleStep++; if ( sampleStep > eqsbs ){ sampleStep = 0 ; }	//	Start counter and prevents overflow
			sampleBuffer[sampleStep] = mainOut ;
			
			int16_t lowpassFilterSample = 0;
			
			uint8_t lpfc = 0 ;	do
			{	//	Add the entire buffer into a single sample to get lowpass sample
				lowpassFilterSample += sampleBuffer[sampleStep - lpfc & eqsbs];
				lpfc++ ;
			} while ( lpfc < eqsbs );
			lowpassFilterSample >>= 4 ;
			int16_t hipassFilterSample = sampleBuffer[sampleStep - 1 & eqsbs] ;//	Gets the last sample in the buffer to subtract from the current sample

			int16_t lowfreqSample = lowpassFilterSample + (( mainOut * ( 16 - leslieLowpassFilter )) >>5 ) ; //	Gets the processed low frequency sample
			int16_t hifreqSample = mainOut - (( hipassFilterSample * leslieHipassFilter ) >>4 ) ;	//	Gets the processed high frequency sample
//	End filters

//	Main leslie drum/horn generator

			static uint16_t lc ;	//	Main leslie counter
			if ( lc > leslieBufferSize ){ lc = 0; }	//Prevents leslie counter overflow

			uint8_t lfc = 0 ;	//	Leslie filter counter
			do
			{	//	Pitch filter. Fill 8 buffers with current sample. This remove some noise but change the leslie vibrato pitch
			leslieDrumBuffer[lc] = lowfreqSample ;
			leslieHornBuffer[lc] = hifreqSample ;
			lc++ ; lfc++ ;
			} while ( lfc < 8 ) ;


			drumPhaseSample = hornPhaseSample = drumOut = hornOut = 0 ;	//	Set samples = 0
			lfc = 0 ;	//Reset leslie filter counter
			do
			{	//	//	Cabinet filters. Modifies the speed of the sample obtained from the buffer.
			drumPhaseSample += 
				(
					leslieDrumBuffer[lc + lfc + drumChorus1 & leslieBufferSize] +
					leslieDrumBuffer[lc - lfc - drumChorus2 & leslieBufferSize] 
				)
			;
			hornPhaseSample +=
				(
					leslieHornBuffer[lc + lfc + hornChorus1 & leslieBufferSize] +
					leslieHornBuffer[lc - lfc - hornChorus2 & leslieBufferSize]
				)
			; 
			drumOut +=
				(
			 		leslieDrumBuffer[lc - lfc - drumChorus1 & leslieBufferSize] +
					leslieDrumBuffer[lc + lfc + drumChorus2 & leslieBufferSize]
				)
			;
			hornOut +=
				(
			 		leslieHornBuffer[lc - lfc - hornChorus1 & leslieBufferSize] +
					leslieHornBuffer[lc + lfc + hornChorus2 & leslieBufferSize]
				)
			;
			lfc++ ;
			} while ( lfc < 8 ) ;	//	Applied 8 times to reduce noise

			//	Generate the output and apply the volume to each part
			drumPhaseSample >>= 4;
			hornPhaseSample >>= 4;
			drumOut >>= 3;
			hornOut >>= 3;


			drumOut-= ( drumPhaseSample * leslieDrumPhase ) >> 4 ;
			hornOut+= ( hornPhaseSample * leslieHornPhase ) >> 4 ;
			
			mainOut = (( drumOut * leslieDrumMicVolume ) >> 4 ) + (( hornOut * leslieHornMicVolume ) >> 4 ) ;
		}
	}
	
#endif






/*
File "leslietimer.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.12
Copyright 2019-2023 Israel Reyes Rodríguez.

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
    This is a new file
	FIXED:Acceleration has a bug when the buffer size changes

Problems:


*/

uint16_t hornChorus1, hornChorus2, drumChorus1, drumChorus2;
const uint16_t leslieBufferSize=(1<<LESLIEBUFFERSIZE) - 1 ;
const uint16_t leslieTimerSize=(1<<LESLIEBUFFERSIZE) ;

void LeslieTimer(){
#if LESLIE > 1
//Leslie Drum timer
		static uint16_t rotaryStimatedDrumSpeed[3] =
		{
			65535,
			(1000000/leslieDrumSpeedSlow)/(leslieTimerSize),
			(1000000/leslieDrumSpeedFast)/(leslieTimerSize)
		};
		static uint32_t leslieDrumStartMicros = micros();
		static uint32_t rotaryDrumSpeed  = rotaryStimatedDrumSpeed[1];
		static uint32_t leslieDrumCurrentMicros, RotaryDrumStep;

		leslieDrumCurrentMicros = micros();
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
					drumChorus1 = ((1+leslieTimerSize-RotaryDrumStep)*leslieDrumVibrato)>>4;
				}
				drumChorus2 = 1+(drumChorus1>>1);
			}
			leslieDrumStartMicros = leslieDrumCurrentMicros;
		}

// Drum acceleration timer

		static uint32_t leslieDrumInertiaPeriod = 120;
		static uint32_t leslieDrumInertiaStartMillis = millis();
		static uint32_t leslieDrumInertiaCurrentMillis;

		leslieDrumInertiaCurrentMillis = millis();
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
#endif

#if LESLIE > 0
// Leslie horn timer
		static uint16_t rotaryStimatedHornSpeed[3] =
			{ 65535,
				(1000000/leslieHornSpeedSlow)/(leslieTimerSize),
				(1000000/leslieHornSpeedFast)/(leslieTimerSize)
			};
		static uint32_t leslieHornStartMicros = micros();
		static uint32_t rotaryHornSpeed  = rotaryStimatedHornSpeed[1];
		static uint32_t leslieHornCurrentMicros, RotaryHornStep;

		leslieHornCurrentMicros = micros();
		if ( rotaryValue != 0 and leslieHornCurrentMicros - leslieHornStartMicros >= rotaryHornSpeed )
		{
			if (rotaryHornSpeed < rotaryStimatedHornSpeed[0])
			{
				if (RotaryHornStep < leslieTimerSize)
				{
					RotaryHornStep++;
				}else{
					RotaryHornStep = 0;
				}
				if (RotaryHornStep < leslieTimerSize>>1){
					hornChorus1 = ((1+RotaryHornStep)*leslieHornVibrato)>>4;
				}else{
					hornChorus1 = ((1+leslieTimerSize-RotaryHornStep)*leslieHornVibrato)>>4;
				}
				hornChorus2 = 1+(hornChorus1>>1);
			}
			leslieHornStartMicros = leslieHornCurrentMicros;
		}

// Horn acceleration timer
		static uint32_t leslieHornInertiaPeriod = 100 ;
		static uint32_t leslieHornInertiaStartMillis = millis();
		static uint32_t leslieHornInertiaCurrentMillis;

		leslieHornInertiaCurrentMillis = millis();
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
#endif
}

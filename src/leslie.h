/*
File "chorus.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.12
Copyright 2019-2023 Israel Reyes Rodríguez.

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	Rewrite two Leslie() function. Remove for/do/while loops for better performance.

Problems:


*/

#if LESLIE == 1

	void Leslie()
	{	
		static uint16_t leslieCounter;
		int16_t leslieOut;

		if ( rotaryValue != 0 )
		{
			if ( leslieCounter > leslieBufferSize ){ leslieCounter = 0; }

			leslieBuffer[leslieCounter] = mainOut ;
			leslieCounter++;
			leslieBuffer[leslieCounter] = mainOut ;
			leslieCounter++;
			leslieBuffer[leslieCounter] = mainOut ;
			leslieCounter++;
			leslieBuffer[leslieCounter] = mainOut ;
			leslieCounter++;

			mainOut =
				(
					leslieBuffer[leslieCounter - hornChorus1 -1 & leslieBufferSize]
					+
					leslieBuffer[leslieCounter + hornChorus1 +0 & leslieBufferSize]
					+
					leslieBuffer[leslieCounter + hornChorus1 +1 & leslieBufferSize]
					+
					leslieBuffer[leslieCounter - hornChorus2 -1 & leslieBufferSize]
					+
					leslieBuffer[leslieCounter - hornChorus1 -0 & leslieBufferSize]
					+
					leslieBuffer[leslieCounter + hornChorus2 +1 & leslieBufferSize]
				)>>2
			;
		}
	}

#elif LESLIE == 2
	void Leslie()
	{
		if ( rotaryValue != 0 )
		{
			
			static int16_t drumPhaseSample;
			static int16_t hornPhaseSample;
			static int16_t drumOut;
			static int16_t hornOut;
			static int16_t equalizerBuffer[8];

			static uint8_t eqc ; eqc ++ ; if ( eqc > 3 ){ eqc = 0; }

			equalizerBuffer[eqc] = mainOut ;
			
			int16_t lowfreqSample =
				( mainOut + ((( equalizerBuffer[0] + equalizerBuffer[1] + equalizerBuffer[2] + equalizerBuffer[3] ) * leslieLowpassFilter ) >> 6 )) 
			;
			int16_t hifreqSample =
				( mainOut - ((( equalizerBuffer[0] + equalizerBuffer[1] + equalizerBuffer[2] + equalizerBuffer[3] ) * leslieHipassFilter ) >> 6 )) << 1
			;

			static uint16_t lc ; if ( lc > leslieBufferSize ){ lc = 0; }

			leslieDrumBuffer[lc] =lowfreqSample ;
			leslieHornBuffer[lc] =hifreqSample ;
			lc++;
			leslieDrumBuffer[lc] =lowfreqSample ;
			leslieHornBuffer[lc] =hifreqSample ;
			lc++;
			leslieDrumBuffer[lc] =lowfreqSample ;
			leslieHornBuffer[lc] =hifreqSample ;
			lc++;
			leslieDrumBuffer[lc] =lowfreqSample ;
			leslieHornBuffer[lc] =hifreqSample ;
			lc++;

			drumPhaseSample =	//	Get drum phase tremolo
				(
					(
						leslieDrumBuffer[lc - drumChorus1 -1 & leslieBufferSize]
						+
						leslieDrumBuffer[lc - drumChorus1 -2 & leslieBufferSize]
						+
						leslieDrumBuffer[lc - drumChorus1 -3 & leslieBufferSize]
						+
						leslieDrumBuffer[lc - drumChorus1 -4 & leslieBufferSize]
						+
						leslieDrumBuffer[lc + drumChorus2 +1 & leslieBufferSize]
						+
						leslieDrumBuffer[lc + drumChorus2 +2 & leslieBufferSize]
						+
						leslieDrumBuffer[lc + drumChorus2 +3 & leslieBufferSize]
						+
						leslieDrumBuffer[lc + drumChorus2 +4 & leslieBufferSize]
					) * leslieDrumPhase
				) >> 7
			;

			hornPhaseSample =	//	Get horn phase tremolo
				(
					(
						leslieHornBuffer[lc - hornChorus1 -1 & leslieBufferSize]
						+
						leslieHornBuffer[lc - hornChorus1 -2 & leslieBufferSize]
						+
						leslieHornBuffer[lc - hornChorus1 -3 & leslieBufferSize]
						+
						leslieHornBuffer[lc - hornChorus1 -4 & leslieBufferSize]
						+
						leslieHornBuffer[lc + hornChorus2 +1 & leslieBufferSize]
						+
						leslieHornBuffer[lc + hornChorus2 +2 & leslieBufferSize]
						+
						leslieHornBuffer[lc + hornChorus2 +3 & leslieBufferSize]
						+
						leslieHornBuffer[lc + hornChorus2 +4 & leslieBufferSize]
					) * leslieHornPhase 
				) >> 7
			;

			drumOut =	//	Generates drum output and subtracts the previous phase sample
			(
				(
					leslieDrumBuffer[lc + drumChorus1 +1 & leslieBufferSize]
					+
					leslieDrumBuffer[lc + drumChorus1 +2 & leslieBufferSize]
					+
					leslieDrumBuffer[lc + drumChorus1 +3 & leslieBufferSize]
					+
					leslieDrumBuffer[lc + drumChorus1 +4 & leslieBufferSize]
					+
					leslieDrumBuffer[lc - drumChorus2 -1 & leslieBufferSize]
					+
					leslieDrumBuffer[lc - drumChorus2 -2 & leslieBufferSize]
					+
					leslieDrumBuffer[lc - drumChorus2 -3 & leslieBufferSize]
					+
					leslieDrumBuffer[lc - drumChorus2 -4 & leslieBufferSize]
				) >> 3
			) - drumPhaseSample
			;
 
			hornOut =	//	Generates horn output and subtracts the previous phase sample
			(
				(
					leslieHornBuffer[lc + hornChorus1 +1 & leslieBufferSize]
					+
					leslieHornBuffer[lc + hornChorus1 +2 & leslieBufferSize]
					+
					leslieHornBuffer[lc + hornChorus1 +3 & leslieBufferSize]
					+
					leslieHornBuffer[lc + hornChorus1 +4 & leslieBufferSize]
					+
					leslieHornBuffer[lc - hornChorus2 -1 & leslieBufferSize]
					+
					leslieHornBuffer[lc - hornChorus2 -2 & leslieBufferSize]
					+
					leslieHornBuffer[lc - hornChorus2 -3 & leslieBufferSize]
					+
					leslieHornBuffer[lc - hornChorus2 -4 & leslieBufferSize]
				) >> 3
			) - hornPhaseSample
			;

			//	Generate the output and apply the volume to each part

			mainOut = (( drumOut * leslieDrumVolume ) >> 4 ) + (( hornOut * leslieHornVolume ) >> 4 );

		}
	}
	
#endif






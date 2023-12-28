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
	Rewrite two Leslie() function. Remove some for/do/while loops for better performance.
	New phase tremolo effect when LESLIE option is 2 ( #defile LESLIE 2 ).
	Added an improved low pass filter for LESLIE option 2 ( #defile LESLIE 2 ).

Problems:
	A lot of noises in general, but it seems that on the drum part it is not so much.
	High pass filter generates a lot of noise.
	Horn part generates a lot of noise

*/

#if LESLIE == 1

	void Leslie()
	{	
		LeslieTimer();
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

			static int16_t drumPhaseSample ;
			static int16_t hornPhaseSample ;
			static int16_t drumOut ;
			static int16_t hornOut ;

//	Lowpass filter with 64 samples array buffer ( 64 samples tested for 22050Hz )
			static const uint8_t lpfbs = 64 ;	//	Lowpass filter buffer size
			static int16_t lowpassBuffer[ lpfbs];	//	Create buffer
			static uint8_t eqc ; eqc ++ ; if ( eqc >= lpfbs ){ eqc = 0; }	//	Create buffer step counter and prevents overflow
			lowpassBuffer[eqc] = mainOut ;	//	Fill buffer with current sample			
			
			int32_t lowpassSample = 0;
			uint8_t lps = 0 ; do
			{
				lowpassSample += lowpassBuffer[lps] ; //	Add all sample array for get lowpass sample
				lps++ ;
			} while ( lps <= lpfbs );

			lowpassSample = lowpassSample >> 4 ;	//	Limit output

			int16_t lowfreqSample = lowpassSample + ((mainOut  * leslieLowpassFilter ) >> 4 ) ;	//	Generate main leslie drum input.

//	Hipass filter whit last sample method

			static int16_t hipassSample ;
			int16_t hifreqSample = mainOut - ((hipassSample * leslieHipassFilter ) >> 4 );
			hipassSample = mainOut ;
//	End filters

//Main leslie drum/horn generator
			static uint16_t lc ;
			
			 if ( lc >= leslieBufferSize ){ lc = 0; }else{lc++;}	//	Create buffer step counter and prevents overflow

			//	places the sample in the buffer 4 at a time. This adjusts the excessive pitch change in the leslie effect.
			//	This function consumes a lot of resources if I program these lines with for/do-while loops.
			//	For that reason I have had to program 'in-line'.

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
			) - drumPhaseSample	//	subtract an amount of drum phase sample
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
			) - hornPhaseSample	//	subtract an amount of horn phase sample
			;

			//	Generate the output and apply the volume to each part
			
			mainOut = (( drumOut * leslieDrumVolume ) >> 4 ) + (( hornOut * leslieHornVolume ) >> 4 );
			//mainOut = hifreqSample ;
		}
	}
	
#endif






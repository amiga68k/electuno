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



byte noteChannel[POLYPHONY];

const int16_t waveLength = (1 << WAVESIZE) ;
uint8_t waveNumber[POLYPHONY];
const uint8_t waveDivider = 16 - WAVESIZE;
const uint8_t wavevolume = 0;

const uint8_t wavemixerBitShifter = WAVESIZE - 8;
int16_t sine[1 << WAVESIZE];
int16_t upperWave1[waveLength];
int16_t upperWave2[waveLength];
int16_t upperWave3[waveLength];
int16_t lowerWave1[waveLength];
int16_t lowerWave2[waveLength];
int16_t lowerWave3[waveLength];
int16_t* wave[6] = {upperWave1, upperWave2, upperWave3, lowerWave1, lowerWave2, lowerWave3};

void WaveMix()
{ 
//  Wave mixer step counter
	static uint16_t wavemixCounter;
	uint16_t s = waveLength - 1 ;
	if (  wavemixCounter  < s ){ wavemixCounter ++; }else{ wavemixCounter = 0 ;}

//  Percusive keys timer
	#if UPPERMODE == 2
		percCurrentMicros = micros();
		if (percCurrentMicros - percStartMicros >= percPeriod)
		{
			percStartMicros = percCurrentMicros;
			byte p=0;
			for (byte i = 0; i < POLYPHONY; i++)
			{
				if ( noteChannel[i] != 0 and waveNumber[i] < 3 ){ p++; }
				//p+=noteChannel[i]>>2;
			}
			if (p == 0)
			{
				percVolume = 250;
			}
			if (percVolume > 0)
			{
				percVolume--;
			}
		}

//  Percusive keys mix
		if ( percEnable != 0)
		{
			enableDrawbar8=0;
			perc = (sine[wavemixCounter * percType & s]*percVolume)>>4;
		}else{
			enableDrawbar8=1;
			perc = 0;
		}
	#endif


	#if UPPERMODE > 0
//  upper wave mixer
		wave[0][wavemixCounter] =
			#if UPPERMODE == 2
				perc +
			#endif
			(sine[wavemixCounter]                   * upperDrawbar[0]) +
			(sine[wavemixCounter  * 3 & s] * upperDrawbar[1]) +
			(sine[wavemixCounter << 1 & s] * upperDrawbar[2]) +
			(sine[wavemixCounter << 2 & s] * upperDrawbar[3]) +
			(sine[wavemixCounter  * 6 & s] * upperDrawbar[4]) +
			(sine[wavemixCounter << 3 & s] * upperDrawbar[5]) +
			(sine[wavemixCounter * 10 & s] * upperDrawbar[6]) +
			(sine[wavemixCounter * 12 & s] * upperDrawbar[7]) +
			(sine[wavemixCounter << 4 & s] * upperDrawbar[8]*enableDrawbar8)
			>>wavemixerBitShifter
		;
    
		wave[1][wavemixCounter] =
			#if UPPERMODE == 2
				perc +
			#endif
			(sine[wavemixCounter]                   * upperDrawbar[0]) +
			(sine[wavemixCounter  * 3 & s] * upperDrawbar[1]<<1) +
			(sine[wavemixCounter << 1 & s] * upperDrawbar[2]) +
			(sine[wavemixCounter << 2 & s] * upperDrawbar[3]) +
			(sine[wavemixCounter  * 6 & s] * upperDrawbar[4]) +
			(sine[wavemixCounter << 3 & s] * upperDrawbar[5]) +
			(sine[wavemixCounter * 10 & s] * upperDrawbar[6]) +
			(sine[wavemixCounter  * 6 & s] * upperDrawbar[7]>>1) +
			((sine[wavemixCounter << 3 & s] * upperDrawbar[8]*enableDrawbar8))
			>>wavemixerBitShifter
		;
      
		wave[2][wavemixCounter] =
			#if UPPERMODE == 2
				perc +
			#endif
			(sine[wavemixCounter]                   * upperDrawbar[0]) +
			(sine[wavemixCounter  * 3 & s] * upperDrawbar[1]<<1) +
			(sine[wavemixCounter << 1 & s] * upperDrawbar[2]) +
			(sine[wavemixCounter << 2 & s] * upperDrawbar[3]) +
			(sine[wavemixCounter  * 6 & s] * upperDrawbar[4]) +
			(sine[wavemixCounter << 2 & s] * upperDrawbar[5]) +
			(sine[wavemixCounter  * 5 & s] * upperDrawbar[6]) +
			(sine[wavemixCounter  * 3 & s] * upperDrawbar[7]) +
			((sine[wavemixCounter << 2 & s] * upperDrawbar[8]*enableDrawbar8))
			>>wavemixerBitShifter
		;
	#endif
    
//  lower wave mixer
  
	#if LOWERMODE == 1
			wave[3][wavemixCounter] =
			(sine[wavemixCounter]          * lowerDrawbar[0]) +
			(sine[wavemixCounter  * 3 & s] * lowerDrawbar[1]) +
			(sine[wavemixCounter << 1 & s] * lowerDrawbar[2]) +
			(sine[wavemixCounter << 2 & s] * lowerDrawbar[3])
			>>wavemixerBitShifter
		;
	#endif

	#if LOWERMODE == 2
		wave[3][wavemixCounter] =
			(sine[wavemixCounter]          * lowerDrawbar[0]) +
			(sine[wavemixCounter  * 3 & s] * lowerDrawbar[1]) +
			(sine[wavemixCounter << 1 & s] * lowerDrawbar[2]) +
			(sine[wavemixCounter << 2 & s] * lowerDrawbar[3]) +
			(sine[wavemixCounter  * 6 & s] * lowerDrawbar[4]) +
			(sine[wavemixCounter << 3 & s] * lowerDrawbar[5]) +
			(sine[wavemixCounter * 10 & s] * lowerDrawbar[6]) +
			(sine[wavemixCounter * 12 & s] * lowerDrawbar[7]) +
			(sine[wavemixCounter << 4 & s] * lowerDrawbar[8])
			>>wavemixerBitShifter
		;
		wave[4][wavemixCounter] =
			(sine[wavemixCounter]          * lowerDrawbar[0]) +
			(sine[wavemixCounter  * 3 & s] * lowerDrawbar[1]) +
			(sine[wavemixCounter << 1 & s] * lowerDrawbar[2]) +
			(sine[wavemixCounter << 2 & s] * lowerDrawbar[3]) +
			(sine[wavemixCounter  * 6 & s] * lowerDrawbar[4]) +
			(sine[wavemixCounter << 3 & s] * lowerDrawbar[5]) +
			(sine[wavemixCounter * 10 & s] * lowerDrawbar[6]) +
			(sine[wavemixCounter  * 6 & s] * lowerDrawbar[7]) +
			(sine[wavemixCounter << 3 & s] * lowerDrawbar[8])
			>>wavemixerBitShifter
		;
		wave[5][wavemixCounter] =
			(sine[wavemixCounter]          * lowerDrawbar[0]) +
			(sine[wavemixCounter  * 3 & s] * lowerDrawbar[1]) +
			(sine[wavemixCounter << 1 & s] * lowerDrawbar[2]) +
			(sine[wavemixCounter << 2 & s] * lowerDrawbar[3]) +
			(sine[wavemixCounter  * 6 & s] * lowerDrawbar[4]) +
			(sine[wavemixCounter << 2 & s] * lowerDrawbar[5]) +
			(sine[wavemixCounter  * 5 & s] * lowerDrawbar[6]) +
			(sine[wavemixCounter  * 3 & s] * lowerDrawbar[7]) +
			(sine[wavemixCounter << 2 & s] * lowerDrawbar[8])
			>>wavemixerBitShifter
		;
	#endif
}

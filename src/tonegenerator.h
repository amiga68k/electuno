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

// Note frequency and polyphony variables.

//uint8_t keyPressed;
float finetune	= 1;
uint8_t freeChannel = 0;
int16_t mainOut;
int16_t lowerOut;
int16_t upperOut;
uint16_t channelFreq[POLYPHONY];
uint16_t oscillators[POLYPHONY];

void ToneGenerator()
{
	mainOut = 0;
	lowerOut = 0;
	upperOut = 0;

	for ( uint8_t i = 0 ; i < POLYPHONY ; i++ )
	{
		oscillators[i] += channelFreq[i];
		if (waveNumber[i] >= 3)
		{
			lowerOut += (wave[waveNumber[i]][oscillators[i] >> waveDivider] >> 4 );
		}else{
			upperOut += (wave[waveNumber[i]][oscillators[i] >> waveDivider] >> 4 );
		}
		mainOut = lowerOut + upperOut;
	}
}

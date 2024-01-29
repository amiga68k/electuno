/*
File "tonegenerator.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.12 
Copyright 2019-2023 Israel Reyes Rodríguez.

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	'For' has been replaced by 'do-while' loop. I've noticed that 8-bit AVRs handle it faster. 
	Changed 'if-else' to 'switch-case' for test.
	Added pedal tone generator.
	Added volume controls for all parts ( lower, upper, pedal ).
	Fix variables for Arduino Due compatibility (Arduino Due cannot reset variables as 'uint8_t i;'. Correct mode is 'uint8_t i = 0;' )
	
Problems:
	This function generates a lot of noise in the higher notes; If the wave is sharper, the noise is more noticeable.
	Perhaps the problem is in the way of mixing the drawbars into a single array and then reproducing it.

*/

#ifndef LOWRAM

	void ToneGenerator()
	{
		static const uint8_t w = 16 - WAVESIZE ;
		mainOut = lowerOut = upperOut = pedalOut= 0;	
		uint8_t i = 0;

		do
		{	
			oscillators[i] += channelFreq[i];
			switch (audioChannel[i])
			{
				case 1:
					upperOut += wave[waveNumber[i]][oscillators[i] >> w] >> 3; // '>> 3' is used so that the generated sample does not exceed 4608
					break;
				case 2:
					lowerOut += wave[waveNumber[i]][oscillators[i] >> w] >> 3;
					break;
				case 3:
					pedalOut += wave[waveNumber[i]][oscillators[i] >> w] >> 3;
					break;
			}
			i++ ;		
		} while (i < POLYPHONY);

			
		mainOut = lowerOut + upperOut ;
	}

#else

	void ToneGenerator()
	{
		static const uint8_t w = 16 - WAVESIZE ;

		oscillators[0] += channelFreq[0];
		oscillators[1] += channelFreq[1];
		oscillators[2] += channelFreq[2];
		oscillators[3] += channelFreq[3];
		oscillators[4] += channelFreq[4];
		oscillators[5] += channelFreq[5];
		oscillators[6] += channelFreq[6];
		oscillators[7] += channelFreq[7];

	mainOut =
		wave[waveNumber[0]][oscillators[0] >> w] +
		wave[waveNumber[1]][oscillators[1] >> w] +
		wave[waveNumber[2]][oscillators[2] >> w] +
		wave[waveNumber[3]][oscillators[3] >> w] +
		wave[waveNumber[4]][oscillators[4] >> w] +
		wave[waveNumber[5]][oscillators[5] >> w] +
		wave[waveNumber[6]][oscillators[6] >> w] +
		wave[waveNumber[7]][oscillators[7] >> w] 
		;
	}
#endif

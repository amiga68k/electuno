/*
File "wavegenerator.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.13
Copyright 2019-2024 Israel Reyes Rodríguez (amiga68k@gmail.com).

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	NEW:

	
	OLD from 0.1.0 :
	Fix generated wave output. Now the generated wave maintains a limit of -2304 to +2304. 
	Added pedal wave array and drawbars mixer.
	New option called WAVEMIXMODE now loudness some tones for limits the volume of certain drawbars in some parts of the keyboard, similar to  old analog and tonewheel organs
	'#define WAVEMIXMODE 0 Disable this modification'

Problems:
	
*/

void WaveSetup()	//	Fill wave array with a sine wave.
{
	int16_t s = ((sizeof(sine) / sizeof(sine[0]))-1)>>1;
	for ( int i = 0 ; i < s ; i++)
	{
		sine[i] = (sin(i * PI / s )) * s;
	}
	for ( int i = 0 ; i < s ; i++)
	{
		sine[i + s ] = (sin(i * PI / s )) * -s;
	}
}

void WaveMix()
{ 
//  Wave mixer step counter
	const uint8_t ws = waveMode + ( WAVESIZE - 9 ) ; // This is used so that the generated sample does not exceed 36864
	const uint16_t wl = waveLength - 1 ;
	static uint16_t wavemixCounter;
	static const bool wm = WAVEMIXMODE ;
	
	if (  wavemixCounter  < wl ){ wavemixCounter ++; }else{ wavemixCounter = 0 ;}

//  Percusive keys timer
	#if UPPERMODE == 2
		percCurrentMicros = Micros ;
		if (percCurrentMicros - percStartMicros >= percPeriod)
		{
			percStartMicros = percCurrentMicros;
			uint8_t p=0;
			for (byte i = 0; i < POLYPHONY; i++)
			{
				if ( noteChannel[i] != 0 and waveNumber[i] < 3 ){ p++; }
				
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
			perc = (sine[wavemixCounter * percType & wl]*percVolume)>>4;
		}else{
			enableDrawbar8=1;
			perc = 0;
		}
	#endif
/*
This function generates the mixture of a sine wave with its harmonic corresponding to each drawbar.
It fills different buffers because depending on the keyboard area, ancient organs did not have linear harmonics, 
rather tones were repeated in the last octaves to compensate for this.
*/
	#if UPPERMODE > 0
//  upper wave mixer
		wave[0][wavemixCounter] =
			#if UPPERMODE == 2
				perc +
			#endif
			(sine[wavemixCounter]            * upperDrawbar[0]>>wm) +
			(sine[wavemixCounter  * 3 & wl ] * upperDrawbar[1]>>wm) +
			(sine[wavemixCounter << 1 & wl ] * upperDrawbar[2]>>wm) +
			(sine[wavemixCounter << 2 & wl ] * upperDrawbar[3]) +
			(sine[wavemixCounter  * 6 & wl ] * upperDrawbar[4]) +
			(sine[wavemixCounter << 3 & wl ] * upperDrawbar[5]) +
			(sine[wavemixCounter * 10 & wl ] * upperDrawbar[6]) +
			(sine[wavemixCounter * 12 & wl ] * upperDrawbar[7]) +
			((sine[wavemixCounter << 4 & wl ] * upperDrawbar[8]*enableDrawbar8))
			>>ws
		;

		wave[1][wavemixCounter] =
			#if UPPERMODE == 2
				perc +
			#endif
			(sine[wavemixCounter]            * upperDrawbar[0]<<wm) +
			(sine[wavemixCounter  * 3 & wl ] * upperDrawbar[1]<<wm) +
			(sine[wavemixCounter << 1 & wl ] * upperDrawbar[2]) +
			(sine[wavemixCounter << 2 & wl ] * upperDrawbar[3]) +
			(sine[wavemixCounter  * 6 & wl ] * upperDrawbar[4]) +
			(sine[wavemixCounter << 3 & wl ] * upperDrawbar[5]) +
			(sine[wavemixCounter * 10 & wl ] * upperDrawbar[6]) +
			(sine[wavemixCounter  * 6 & wl ] * upperDrawbar[7]>>wm) +
			((sine[wavemixCounter << 3 & wl ] * upperDrawbar[8]*enableDrawbar8)>>wm)
			>>ws
		;
		wave[2][wavemixCounter] =
			#if UPPERMODE == 2
				perc +
			#endif
			(sine[wavemixCounter]            * upperDrawbar[0]<<wm) +
			(sine[wavemixCounter  * 3 & wl ] * upperDrawbar[1]<<wm) +
			(sine[wavemixCounter << 1 & wl ] * upperDrawbar[2]) +
			(sine[wavemixCounter << 2 & wl ] * upperDrawbar[3]) +
			(sine[wavemixCounter  * 6 & wl ] * upperDrawbar[4]) +
			(sine[wavemixCounter << 2 & wl ] * upperDrawbar[5]) +
			(sine[wavemixCounter  * 5 & wl ] * upperDrawbar[6]>>wm) +
			(sine[wavemixCounter  * 3 & wl ] * upperDrawbar[7]>>wm) +
			((sine[wavemixCounter << 2 & wl ] * upperDrawbar[8]*enableDrawbar8)>>wm)
			>>ws
		;
	#endif

//  lower wave mixer
  
	#if LOWERMODE == 1
			wave[3][wavemixCounter] =
			(sine[wavemixCounter]            * lowerDrawbar[0] >> wm ) +
			(sine[wavemixCounter  * 3 & wl ] * lowerDrawbar[1] >> wm) +
			(sine[wavemixCounter << 1 & wl ] * lowerDrawbar[2] >> wm) +
			(sine[wavemixCounter << 2 & wl ] * lowerDrawbar[3])
			>>ws
		;
	#endif

	#if LOWERMODE == 2
		wave[3][wavemixCounter] =
			(sine[wavemixCounter]            * lowerDrawbar[0] >> wm ) +
			(sine[wavemixCounter  * 3 & wl ] * lowerDrawbar[1] >> wm ) +
			(sine[wavemixCounter << 1 & wl ] * lowerDrawbar[2] >> wm) +
			(sine[wavemixCounter << 2 & wl ] * lowerDrawbar[3]) +
			(sine[wavemixCounter  * 6 & wl ] * lowerDrawbar[4]) +
			(sine[wavemixCounter << 3 & wl ] * lowerDrawbar[5]) +
			(sine[wavemixCounter * 10 & wl ] * lowerDrawbar[6]) +
			(sine[wavemixCounter * 12 & wl ] * lowerDrawbar[7]) +
			(sine[wavemixCounter << 4 & wl ] * lowerDrawbar[8])
			>>ws
		;
		wave[4][wavemixCounter] =
			(sine[wavemixCounter]            * lowerDrawbar[0]) +
			(sine[wavemixCounter  * 3 & wl ] * lowerDrawbar[1]) +
			(sine[wavemixCounter << 1 & wl ] * lowerDrawbar[2]) +
			(sine[wavemixCounter << 2 & wl ] * lowerDrawbar[3]) +
			(sine[wavemixCounter  * 6 & wl ] * lowerDrawbar[4]) +
			(sine[wavemixCounter << 3 & wl ] * lowerDrawbar[5] >> wm) +
			(sine[wavemixCounter * 10 & wl ] * lowerDrawbar[6] >> wm ) +
			(sine[wavemixCounter  * 6 & wl ] * lowerDrawbar[7] >> wm ) +
			(sine[wavemixCounter << 3 & wl ] * lowerDrawbar[8] >> wm )
			>>ws
		;
		wave[5][wavemixCounter] =
			(sine[wavemixCounter]            * lowerDrawbar[0] << wm )  +
			(sine[wavemixCounter  * 3 & wl ] * lowerDrawbar[1] << wm ) +
			(sine[wavemixCounter << 1 & wl ] * lowerDrawbar[2]) +
			(sine[wavemixCounter << 2 & wl ] * lowerDrawbar[3]) +
			(sine[wavemixCounter  * 6 & wl ] * lowerDrawbar[4]) +
			(sine[wavemixCounter << 2 & wl ] * lowerDrawbar[5] >> wm ) +
			(sine[wavemixCounter  * 5 & wl ] * lowerDrawbar[6] >> wm ) +
			(sine[wavemixCounter  * 3 & wl ] * lowerDrawbar[7] >> wm ) +
			(sine[wavemixCounter << 2 & wl ] * lowerDrawbar[8] >> wm )
			>>ws
		;
	#endif

	#if PEDALMODE > 0
		wave[6][wavemixCounter] =
			(sine[wavemixCounter]           * pedalDrawbar[0]) +
			(sine[wavemixCounter  * 3 & wl] * pedalDrawbar[1]) +
			(sine[wavemixCounter << 1 & wl] * pedalDrawbar[2]) +
			(sine[wavemixCounter << 2 & wl] * pedalDrawbar[3]) +
			(sine[wavemixCounter  * 6 & wl] * pedalDrawbar[4]) +
			(sine[wavemixCounter << 3 & wl] * pedalDrawbar[5]) +
			(sine[wavemixCounter * 10 & wl] * pedalDrawbar[6]) +
			(sine[wavemixCounter * 12 & wl] * pedalDrawbar[7]) +
			(sine[wavemixCounter << 4 & wl] * pedalDrawbar[8])
			>>ws
		;

	#endif
}
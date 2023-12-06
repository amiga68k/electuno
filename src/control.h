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

void electunoNoteOn(byte channel, byte rvalue1, byte rvalue2)
{
	if (channel == 1 or channel == 2)
	{
		for (byte i = 0; i < POLYPHONY; i++)
		{
			if (noteChannel[i] == 0)
			{
				freeChannel = i;
			}
		}

		float noteFreq = ((6.1*finetune)*pow(1.059463094359,(rvalue1)));
		if (channel == 1)
		{
			channelFreq[freeChannel] = noteFreq;
			noteChannel[freeChannel] = rvalue1;
			if (rvalue1 < 77)
			{
				waveNumber[freeChannel] = 0;
			} else if (rvalue1 >= 77 and rvalue1 < 89)
			{
				waveNumber[freeChannel] = 1;

			} else if (rvalue1 >= 89)
			{
				waveNumber[freeChannel] = 2;
			}
		}

		#if LOWERMODE == 1
			channelFreq[freeChannel] = noteFreq;
			noteChannel[freeChannel] = rvalue1;
			if (channel == 2)
			{
				waveNumber[freeChannel] = 3;
			}
		#endif

		#if LOWERMODE == 2
			channelFreq[freeChannel] = noteFreq;
			noteChannel[freeChannel] = rvalue1;
			if (channel == 2)
			{
				if (rvalue1 < 77)
				{
					waveNumber[freeChannel] = 3;
				} else if (rvalue1 >= 77 and rvalue1 < 89)
				{
					waveNumber[freeChannel] = 4;
				} else if (rvalue1 >= 89)
				{
					waveNumber[freeChannel] = 5;
				}
			}
#endif
  }
}

void electunoNoteOff(byte channel, byte rvalue1, byte rvalue2)
{  
	if (channel == 1 )
	{
		for (byte i = 0; i < POLYPHONY; i++)
		{
			if (noteChannel[i] == rvalue1 and waveNumber[i] < 3 )
			{
				if (channel == 1)
				{
					oscillators[i] = 0;
					channelFreq[i] = 0;
					noteChannel[i] = 0;
				}
			}
		}
	}

		if (channel == 2 )
		{
			for (byte i = 0; i < POLYPHONY; i++)
			{
				if (noteChannel[i] == rvalue1 and waveNumber[i] > 2 )
				{
					if (channel == 2)
					{
						oscillators[i] = 0;
						channelFreq[i] = 0;
						noteChannel[i] = 0;
					}
				}
			}
		}
}

void electunoControlChange(byte channel, byte rvalue1, byte rvalue2)
{
	switch (rvalue1)
	{
		#if UPPERMODE > 0
			case 79: upperDrawbar[0] =       map(rvalue2,  0,126,  0,  8); break;
			case 80: upperDrawbar[1] =       map(rvalue2,  0,126,  0,  8); break;
			case 81: upperDrawbar[2] =       map(rvalue2,  0,126,  0,  8); break;
			case 82: upperDrawbar[3] =       map(rvalue2,  0,126,  0,  8); break;
			case 83: upperDrawbar[4] =       map(rvalue2,  0,126,  0,  8); break;
			case 84: upperDrawbar[5] =       map(rvalue2,  0,126,  0,  8); break;
			case 85: upperDrawbar[6] =       map(rvalue2,  0,126,  0,  8); break;
			case 86: upperDrawbar[7] =       map(rvalue2,  0,126,  0,  8); break;
			case 87: upperDrawbar[8] =       map(rvalue2,  0,126,  0,  8); break;
			#if UPPERMODE == 2
				case 68: percEnable =            map(rvalue2,  0,126,  0,  1); break;
				case 69: percType =              harmonic[map(rvalue2,  0,126,  0,  1)]; break;
				case 70: percPeriod =            percSpeed[map(rvalue2,  0,126,  0,  1)]; break;
			#endif
		#endif

		#if LOWERMODE > 0
			case 88: lowerDrawbar[0] =       map(rvalue2,  0,126,  0,  8); break;
			case 89: lowerDrawbar[1] =       map(rvalue2,  0,126,  0,  8); break;
			case 90: lowerDrawbar[2] =       map(rvalue2,  0,126,  0,  8); break;
			case 91: lowerDrawbar[3] =       map(rvalue2,  0,126,  0,  8); break;
			#if LOWERMODE == 2
				case 92: lowerDrawbar[4] =       map(rvalue2,  0,126,  0,  8); break;
				case 93: lowerDrawbar[5] =       map(rvalue2,  0,126,  0,  8); break;
				case 94: lowerDrawbar[6] =       map(rvalue2,  0,126,  0,  8); break;
				case 95: lowerDrawbar[7] =       map(rvalue2,  0,126,  0,  8); break;
				case 96: lowerDrawbar[8] =       map(rvalue2,  0,126,  0,  8); break;
			#endif
		#endif

		#if CHORUS > 0
			case 71: lowerVibratoSwitch =    map(rvalue2,  0,126,  0,  1); break;
			case 72: upperVibratoSwitch =    map(rvalue2,  0,126,  0,  1); break;
			//case 72: vibratoType =           map(rvalue2,  0,126,  0,  1); break;

		#endif

		#if REVERB > 0
			case 76: reverb  =               map(rvalue2,  0,126,  0, 64); break;
		#endif

		#if OVERDRIVE > 0
			case 77: overdrive  =       map(rvalue2,  0,126,  0,  32); break;
		#endif

		#if LESLIE > 0
			case 73: rotaryValue =           map(rvalue2,  0,126,  0,  2); break;
			case 78: leslieHornVib  =       map(rvalue2,  0,126,  0,  16); break;
		#endif
	}
}

/*
File "control.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.12 
Copyright 2019-2023 Israel Reyes Rodríguez.

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	Rearranged midi controls.
	Now, the midi controllers that drive the drawbars are the same as setBfree, they can be changed, but for me it is more comfortable.
	Added controllers for pedal.

Problems:

*/

void electunoControlChange(byte channel, byte rvalue1, byte rvalue2)
{
	switch (channel)
	{
		case 1:
			switch (rvalue1)
			{
				case 1:  reverb  =              	map(rvalue2,  0,127,  0, 64); break;
				case 11: expressionPedal =      	map(rvalue2,  0,127,  0,127); break;
	// RESERVED case 49: chorusSpeed
				case 50: lowerVibratoSwitch =   	map(rvalue2,  0,127,  0,  1); break;
				case 51: upperVibratoSwitch =   	map(rvalue2,  0,127,  0,  1); break;
	  			case 52: vibratoType =          	map(rvalue2,  0,127,  0,  5); break;
	// RESERVED case 53:	
				case 54: overdrive  =      			map(rvalue2,  0,127,  0, 32); break;
				case 55: rotaryValue =          	map(rvalue2,  0,127,  0,  2); break;
				case 56: leslieDrumVibrato  =   	map(rvalue2,  0,127,  0, 16); break;
				case 57: leslieHornVibrato  =   	map(rvalue2,  0,127,  0, 16); break;
				case 58: leslieLowpassFilter  =     map(rvalue2,  0,127,  0, 16); break;
				case 59: leslieHipassFilter  =      map(rvalue2,  0,127,  0, 16); break;
				case 60: leslieDrumVolume  =      	map(rvalue2,  0,127,  0, 16); break;
				case 61: leslieHornVolume  =       	map(rvalue2,  0,127,  0, 16); break;
	// RESERVED case 62: leslieHornSpeedSlow
	// RESERVED case 63: leslieHornSpeedFast
	// RESERVED case 64: leslieHornAcceleration
	// RESERVED case 65: leslieHornDeceleration
	// RESERVED case 66: leslieDrumSpeedSlow
	// RESERVED case 67: leslieDrumSpeedFast
	// RESERVED case 68: leslieDrumAcceleration
	// RESERVED case 69: leslieDrumDeceleration
				case 3:  upperVolume =				map(rvalue2,  0,127,  0,127); break;
				case 80: percEnable =           	map(rvalue2,  0,127,  0,  1); break;
				case 81: percType =        harmonic[map(rvalue2,  0,127,  0,  1)]; break;
				case 82: percPeriod =     percSpeed[map(rvalue2,  0,127,  0,  1)]; break;				
				case 70: upperDrawbar[0] =      	map(rvalue2,  0,127,  0,  8); break;
				case 71: upperDrawbar[1] =      	map(rvalue2,  0,127,  0,  8); break;
				case 72: upperDrawbar[2] =      	map(rvalue2,  0,127,  0,  8); break;
				case 73: upperDrawbar[3] =      	map(rvalue2,  0,127,  0,  8); break;
				case 74: upperDrawbar[4] =      	map(rvalue2,  0,127,  0,  8); break;
				case 75: upperDrawbar[5] =      	map(rvalue2,  0,127,  0,  8); break;
				case 76: upperDrawbar[6] =      	map(rvalue2,  0,127,  0,  8); break;
				case 77: upperDrawbar[7] =      	map(rvalue2,  0,127,  0,  8); break;
				case 78: upperDrawbar[8] =      	map(rvalue2,  0,127,  0,  8); break;
							}
		break;	
		case 2:
			switch (rvalue1)
			{
				case 1:  lowerVolume =				map(rvalue2,  0,127,  0,127); break;
				case 70: lowerDrawbar[0] =      	map(rvalue2,  0,127,  0,  8); break;
				case 71: lowerDrawbar[1] =      	map(rvalue2,  0,127,  0,  8); break;
				case 72: lowerDrawbar[2] =      	map(rvalue2,  0,127,  0,  8); break;
				case 73: lowerDrawbar[3] =      	map(rvalue2,  0,127,  0,  8); break;
				case 74: lowerDrawbar[4] =      	map(rvalue2,  0,127,  0,  8); break;
				case 75: lowerDrawbar[5] =      	map(rvalue2,  0,127,  0,  8); break;
				case 76: lowerDrawbar[6] =      	map(rvalue2,  0,127,  0,  8); break;
				case 77: lowerDrawbar[7] =      	map(rvalue2,  0,127,  0,  8); break;
				case 78: lowerDrawbar[8] =      	map(rvalue2,  0,127,  0,  8); break;
			}
		break;
		case 3:
			switch (rvalue1)
			{
				case 1:  pedalVolume =				map(rvalue2,  0,127,  0,127); break;
				case 70: pedalDrawbar[0] =      	map(rvalue2,  0,127,  0,  8); break;
				case 71: pedalDrawbar[1] =      	map(rvalue2,  0,127,  0,  8); break;
				case 72: pedalDrawbar[2] =      	map(rvalue2,  0,127,  0,  8); break;
				case 73: pedalDrawbar[3] =      	map(rvalue2,  0,127,  0,  8); break;
				case 74: pedalDrawbar[4] =      	map(rvalue2,  0,127,  0,  8); break;
				case 75: pedalDrawbar[5] =      	map(rvalue2,  0,127,  0,  8); break;
				case 76: pedalDrawbar[6] =      	map(rvalue2,  0,127,  0,  8); break;
				case 77: pedalDrawbar[7] =      	map(rvalue2,  0,127,  0,  8); break;
				case 78: pedalDrawbar[8] =      	map(rvalue2,  0,127,  0,  8); break;
			}
		break;		
	}
}

void electunoNoteOn(byte channel, byte rvalue1, byte rvalue2)
{
	if (channel == 1 or channel == 2 or channel == 3)
	{			
		for (byte i = 0; i < POLYPHONY; i++)
		{
			if (noteChannel[i] == 0)
			{
				freeChannel = i;
			}
		}

		float noteFreq = (((6.05 * FREQTUNE))*pow(1.059463094359,(rvalue1)));
		//float noteFreq = (((6.05*(1 << FREQTUNE)))*pow(1.059463094359,(rvalue1)));
		if (channel == 1)
		{	
			channelFreq[freeChannel] = noteFreq;
			noteChannel[freeChannel] = rvalue1;
			audioChannel[freeChannel] = channel;
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
			if (channel == 2)
			{
				channelFreq[freeChannel] = noteFreq;
				noteChannel[freeChannel] = rvalue1;
				audioChannel[freeChannel] = channel;
				waveNumber[freeChannel] = 3;
			}
		#endif

		#if LOWERMODE == 2			
			if (channel == 2)
			{
				channelFreq[freeChannel] = noteFreq;
				noteChannel[freeChannel] = rvalue1;
				audioChannel[freeChannel] = channel;
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

		#if PEDALMODE > 0
			if (channel == 3)
			{
				channelFreq[freeChannel] = noteFreq;
				noteChannel[freeChannel] = rvalue1;
				audioChannel[freeChannel] = channel;
				waveNumber[freeChannel] = 6;
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
			if (noteChannel[i] == rvalue1 and waveNumber[i] <= 2 )
			{
				if (channel == 1)
				{
					oscillators[i] = 0; // cause noise when change keystate
					channelFreq[i] = 0;
					noteChannel[i] = 0;
					audioChannel[i] = 0;
					
				}
			}
		}
	}

	if (channel == 2 )
	{
		for (byte i = 0; i < POLYPHONY; i++)
		{
			if (noteChannel[i] == rvalue1 and waveNumber[i] >= 3 and waveNumber[i] <= 5 )
			{
				if (channel == 2)
				{
					oscillators[i] = 0; // cause noise when change keystate
					channelFreq[i] = 0;
					noteChannel[i] = 0;
					audioChannel[i] = 0;
					
				}
			}
		}
	}

	if (channel == 3 )
	{
		for (byte i = 0; i < POLYPHONY; i++)
		{
			if (noteChannel[i] == rvalue1 and waveNumber[i] == 6 )
			{
				if (channel == 3)
				{
					oscillators[i] = 0; // cause noise when change keystate
					channelFreq[i] = 0;
					noteChannel[i] = 0;
					audioChannel[i] = 0;
					
				}
			}
		}
	}

}

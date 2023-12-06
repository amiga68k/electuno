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





void Chorus()
{ 
	static int16_t chorusLowerBuffer[1<<CHORUSBUFFERSIZE];
	static int16_t chorusUpperBuffer[1<<CHORUSBUFFERSIZE];
	static const uint16_t chorusPeriod = (1000000 / chorusSpeed) / (1<<CHORUSBUFFERSIZE);
	static const uint8_t s=(1<<CHORUSBUFFERSIZE)-1;
	static uint32_t chorusStartMicros = micros();
	static uint32_t chorusCurrentMicros;
	static uint8_t c, d, vibrato, bCounter;

	chorusCurrentMicros = micros();
	if (chorusCurrentMicros - chorusStartMicros >= chorusPeriod)
	{
		chorusStartMicros = chorusCurrentMicros;
		if ( c < s )
		{
			c++;
		}else{
			c=0;
		}
		if (c < s>>1)
		{
			vibrato = (c>>vibratoType)+1;
		}else{
			vibrato = ((s-c)>>vibratoType)+1;
		}
	}

	if ( bCounter < s ){ bCounter++ ;} else  { bCounter = 0 ;}
  
	if (lowerVibratoSwitch == 1)
	{
		chorusLowerBuffer[bCounter] = lowerOut>>2;
		mainOut += (chorusLowerBuffer[bCounter + vibrato & s]) + (chorusLowerBuffer[bCounter - vibrato & s]);
	}
  
	if (upperVibratoSwitch == 1)
	{
		chorusUpperBuffer[bCounter] = upperOut>>2;
		mainOut += (chorusUpperBuffer[bCounter + vibrato & s]) + (chorusUpperBuffer[bCounter - vibrato & s]);
	}
}

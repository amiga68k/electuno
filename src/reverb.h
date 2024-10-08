/*
File "reverb.h"
Author: Israel Reyes Rodríguez (isRasta)
Url: https://github.com/amiga68k/electuno
Mail: amiga68k@gmail.com

Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.13
Copyright 2019-2024 Israel Reyes Rodríguez (amiga68k@gmail.com).

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	New code for test.
	Fix some variables to working with Arduino Due	
Problems:
	
*/

void Reverb()
{
	uint16_t reverbBufferSize = ( 1 << REVERBBUFFERSIZE ) - 1 ;
	static int16_t reverbBuffer[ 1 << REVERBBUFFERSIZE ] ;
	static uint16_t reverbStep;
	static int16_t reverbOut ;
	static const uint16_t reverbLines[4] = 
	{
		reverbBufferSize >> 3 ,
		( reverbBufferSize >> 3 ) <<1,
		reverbBufferSize >> 2 ,
		reverbBufferSize - 1
	};
  
	if ( reverbStep < reverbBufferSize )
	{
		reverbStep++ ;
	}else{
		reverbStep = 0 ;
	}
	
	reverbBuffer[reverbStep] = mainOut + ( reverbOut >>1 ) >> 1;

	reverbOut = 0;

	uint8_t rlc = 0 ;

	do
	{
		reverbOut += reverbBuffer[reverbStep - reverbLines[rlc] & reverbBufferSize] >> rlc ; 
		rlc ++ ;
	} while ( rlc < 4 ) ;

	// mainOut += (( reverbOut * reverb ) >> 7 );
	mainOut = (( mainOut * (127-reverb) ) >> 7 ) + (( reverbOut * reverb ) >> 7 );
  
}

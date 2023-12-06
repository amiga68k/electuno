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






void Reverb()
{
	uint16_t reverbBufferSize = REVERBBUFFER - 1 ;
	static int16_t reverbBuffer[REVERBBUFFER];
	static uint16_t reverbStep;
  
	if ( reverbStep < reverbBufferSize )
	{
		reverbStep++ ;
	}else{
		reverbStep = 0 ;
	}

	reverbBuffer[reverbStep] = mainOut ;

	int16_t reverbOut =
		(
			( reverbBuffer[reverbStep -(reverbBufferSize>>7) & reverbBufferSize] >> 0 )
			+
			( reverbBuffer[reverbStep -(reverbBufferSize>>6) & reverbBufferSize] >> 1 )
			+
			( reverbBuffer[reverbStep -(reverbBufferSize>>5) & reverbBufferSize] >> 1 )
			+
			( reverbBuffer[reverbStep -(reverbBufferSize>>4) & reverbBufferSize] >> 1 )
			+
			( reverbBuffer[reverbStep -(reverbBufferSize>>3) & reverbBufferSize] >> 2 )
			+
			( reverbBuffer[reverbStep -(reverbBufferSize>>2) & reverbBufferSize] >> 2 )
			+
			( reverbBuffer[reverbStep -(reverbBufferSize>>1) & reverbBufferSize] >> 2 )
			+
			( reverbBuffer[reverbStep -(reverbBufferSize>>0) & reverbBufferSize] >> 3 )
		)
	;

	mainOut = (( mainOut  +  ((reverbOut * reverb)>>7) ) * 64-reverb)>>6 ;
  
}

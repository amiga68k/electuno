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
	Little changes in vibrato sample mixer.
	Create 2 modes : 
		CHORUS 1 : Upper and lower outputs for the same vibrato bus.
		CHORUS 2 : Each keyboard has one vibrato bus. Can enable vibrato effect for upper or lower keyboards independently		

Problems:
	Noises.
	
	
*/



#if CHORUS == 1
	void Chorus()
	{ 
	static const int16_t cbs = (1 << CHORUSBUFFERSIZE) -1 ;	//	Chorus buffer size
		static uint16_t csc ;	//	Chorus step counter 
		static int16_t chorusBuffer[1<<CHORUSBUFFERSIZE];
		int16_t vibratoOut ;

		csc++; if ( csc > cbs ){ csc = 0 ; }
  
		if (lowerVibratoSwitch == 1 or upperVibratoSwitch == 1)
		{
			chorusBuffer[csc] = mainOut >> 2 ;
			vibratoOut = (chorusBuffer[csc + vibrato & cbs] ) + (chorusBuffer[csc - vibrato & cbs]);
		}else{
			vibratoOut = mainOut ;
		}

		mainOut = vibratoOut;
	}

#elif CHORUS == 2
	void Chorus()
	{ 
		static const int16_t cbs = (1 << CHORUSBUFFERSIZE) -1 ;	//	Chorus buffer size
		static uint16_t csc ;	//	Chorus step counter 
		static int16_t chorusLowerBuffer[1<<CHORUSBUFFERSIZE];
		static int16_t chorusUpperBuffer[1<<CHORUSBUFFERSIZE];
		int16_t vibratoLowerOut, vibratoUpperOut ;

		csc++; if ( csc > cbs ){ csc = 0 ; }
  
		if (lowerVibratoSwitch == 1)
		{
			chorusLowerBuffer[csc] = lowerOut >> 2 ;
			vibratoLowerOut = (chorusLowerBuffer[csc + vibrato & cbs] ) + (chorusLowerBuffer[csc - vibrato & cbs]);
			vibratoLowerOut += lowerOut >> 1 ;
		}else{
			vibratoLowerOut = lowerOut ;
		}
  
		if (upperVibratoSwitch == 1)
		{
			chorusUpperBuffer[csc] = upperOut >> 2 ;
			vibratoUpperOut = (chorusUpperBuffer[csc + vibrato & cbs] >> 1) + (chorusUpperBuffer[csc - vibrato & cbs]);
			vibratoUpperOut += upperOut >> 1 ;
		}else{
			vibratoUpperOut = upperOut ;
		}

		mainOut = vibratoLowerOut + vibratoUpperOut;
	}
#endif
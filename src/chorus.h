/*
File "chorus.h"
Author: Israel Reyes Rodríguez (isRasta)
Url: https://github.com/amiga68k/electuno
Mail: amiga68k@gmail.com

Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.1.3
Copyright 2019-2023 Israel Reyes Rodríguez (amiga68k@gmail.com).

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
	Now, the maximum buffer size for the vibrato effect is 7 bits (128).
	Added ChorusTimer() function to this file.
	Little changes in vibrato sample mixer.
	Create 2 modes : 
		CHORUS 1 : Upper and lower outputs for the same vibrato bus.
		CHORUS 2 : Each keyboard has one vibrato bus. Can enable vibrato effect for upper or lower keyboards independently

Problems:
	Noises.
	
*/

uint8_t vibrato ; 
	
void ChorusTimer()
{	
	static const uint8_t cbs = ( 1 <<CHORUSBUFFERSIZE ) -1 ;
	static uint16_t chorusPeriod = ( 1000000 / chorusSpeed) / ( 1 <<CHORUSBUFFERSIZE ) ;
	static uint32_t chorusStartMicros ;
	static uint32_t chorusCurrentMicros ;
	static uint8_t ctc ;	//	Chorus timer counter

	chorusCurrentMicros = Micros ;
	if ( chorusCurrentMicros - chorusStartMicros >= chorusPeriod )
	{
		chorusStartMicros = chorusCurrentMicros;
		if ( ctc < cbs )
		{
			ctc++ ;
		}else{
			ctc = 0 ;
		}
		if ( ctc < cbs >>1 )
		{
			vibrato = ( ctc >>vibratoType ) + 1 ;
		}else{
			vibrato = ( ( cbs - ctc ) >>vibratoType) + 1 ;
		}
	}
}

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
			chorusBuffer[csc] = mainOut >> 1;
			vibratoOut = (chorusBuffer[csc + vibrato & cbs] );
		}else{
			vibratoOut = mainOut ;
		}

		mainOut += vibratoOut;
	}

#elif CHORUS == 2

	void Chorus()
	{ 
		static const int16_t cbs = (1 << CHORUSBUFFERSIZE) -1 ;	//	Chorus buffer size
		static uint16_t csc ;	//	Chorus step counter 
		static int16_t chorusLowerBuffer[1<<CHORUSBUFFERSIZE];
		static int16_t chorusUpperBuffer[1<<CHORUSBUFFERSIZE];
		int16_t vibratoLowerOut, vibratoUpperOut ;

		csc++ ; if ( csc > cbs ){ csc = 0 ; }
		chorusLowerBuffer[csc] = lowerOut ;
		chorusUpperBuffer[csc] = upperOut ;

		if (lowerVibratoSwitch == 1)
		{			
			vibratoLowerOut = (chorusLowerBuffer[csc + vibrato & cbs] ) >> 1;
			vibratoLowerOut += lowerOut ;
		}else{
			vibratoLowerOut = lowerOut ;
		}
  
		if (upperVibratoSwitch == 1)
		{			
			vibratoUpperOut = (chorusUpperBuffer[csc + vibrato & cbs] ) >> 1;
			vibratoUpperOut += upperOut ;
		}else{
			vibratoUpperOut = upperOut ;
		}

		mainOut = vibratoLowerOut + vibratoUpperOut;
	}

#endif

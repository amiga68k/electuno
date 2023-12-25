/*
File "chorustimer.h"
Electuno Organ Simulator library (based on Lo-Fi Tonewheel).
Version 0.12 
Copyright 2019-2023 Israel Reyes Rodríguez.

This file is part of Electuno organ simulator library.
Electuno is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Electuno is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.

File notes:
    This is a new file
	
Problems:


*/

uint8_t vibrato ; // Shared variable used by Chorus() function
	
void ChorusTimer()
{	
	static const uint8_t cbs = ( 1 <<CHORUSBUFFERSIZE ) -1 ;
	static uint16_t chorusPeriod = ( 1000000 / chorusSpeed) / ( 1 <<CHORUSBUFFERSIZE ) ;
	static uint32_t chorusStartMicros = micros() ;
	static uint32_t chorusCurrentMicros ;
	static uint8_t ctc ;	//	Chorus timer counter

	chorusCurrentMicros = micros();
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
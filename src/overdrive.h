/*
File "overdrive.h"
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
	Little changes on 'constrain' output.
	
Problems:
	Uggly distortion effect.
	
*/

void OverDrive()
{
	mainOut += constrain( ((mainOut * overdrive)>>5), -500 , 500 ) ;
}

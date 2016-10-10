/*	This file is part of SimpleNMEA.

    SimpleNMEA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SimpleNMEA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SimpleNMEA.  If not, see <http://www.gnu.org/licenses/>.*/

#include "nmea.h"
#include <stdlib.h>

static void NMEA_GGA_Parser(NMEA_GGA_t* un, uint8_t* p, int state){
	char* e;
	switch(state){
		case 1:
			un->time = DECIMALPARSER(p, NULL);
			break;
		case 2://DDMM.MMMMM
			un->latitudeD = strtol(p, &e, 10) / 100;
			if((char*)p != e)
				un->latitudeM = DECIMALPARSER(p+2, NULL);
			else
				un->latitudeM = 0;
			break;
		case 3:
			if(p[0] == 'S') un->latitudeD = -un->latitudeD;
			break;
		case 4://DDDMM.MMMMM
			un->longitudeD = strtol(p, &e, 10) / 100;
			if((char*)p != e)
				un->longitudeM = DECIMALPARSER(p+3, NULL);
			else
				un->longitudeM = 0;
			break;
		case 5:
			if (p[0] == 'W') un->longitudeD = -un->longitudeD;
			break;
		case 6:
			un->fixQuality = strtol(p, NULL, 10);
			break;
		case 7:
			un->gpsNum = strtol(p, NULL, 10);
			break;
		case 8:
			un->horDilution = DECIMALPARSER(p, NULL);
			break;
		case 9:
			un->altitude = DECIMALPARSER(p, NULL);
			break;
		case 10: break;//M?
		case 11:
			un->geoidHeight = DECIMALPARSER(p, NULL);
			break;
	}
}

void NMEAProcess(NMEA_t* un, uint8_t ch){
	if(un->index >= NMEABSIZE-1){
		un->index = 0;
		un->pstart = false;
	}
	if(ch == '$'){
		un->pstart = true;
		un->index = 0;
		un->state = 0;
		un->ppos = 0;
		un->checksum = 0;
		un->ended = false;
	}
	un->buffer[un->index++] = ch;
	un->buffer[un->index] = 0;
	if(un->pstart == false)
		return;
	if(ch == ','){
		uint8_t* p = un->buffer+un->ppos;
		if(un->state == 0){
			if(strncmp(p, "$GPGGA", 6)==0)	{
				un->sentence = NMEA_GGA;
				un->data = NMEAALLOC(un, sizeof(NMEA_GGA_t));
			}
			else 
				un->sentence = NMEA_OTHER;
		}
		else if(un->sentence == NMEA_GGA){
			NMEA_GGA_Parser((NMEA_GGA_t*)un->data, p, un->state);
		}
		un->ppos = un->index;
		un->state++;
	}
	if(ch != '$' && ch != '*' && un->ended == false)
		un->checksum ^= ch;
	if(ch == '*'){
		un->ppos = un->index;
		un->ended = true;
	}
	if(ch == '\r' || ch == '\n')
	{
		int chks = strtol(un->buffer+un->ppos, NULL, 16);
		if(chks == un->checksum){//validate data using the checksum
			if(un->sentence == NMEA_GGA)
				NMEAGGACallback((NMEA_GGA_t*)un->data);
		}
		else
			NMEAWrongChecksum(un);
		un->pstart = false;
	}
}

__weak void NMEAWrongChecksum(NMEA_t* un){

}

__weak void NMEAGGACallback(NMEA_GGA_t* un){

}

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

#ifndef NMEA_H
#define NMEA_H

#include <stdint.h>
#include <stdbool.h>

#include "nmea_config.h"

typedef enum{
	NMEA_GGA,
	NMEA_OTHER,
} NMEASentence_t;

typedef struct {
	uint8_t buffer[NMEABSIZE];
	NMEASentence_t sentence;
	int index;
	bool pstart;
	int state;
	int ppos;
	volatile void* volatile data;
	int checksum;
	bool ended;
} NMEA_t;

typedef struct {
	DECIMAL time;
	int latitudeD;
	DECIMAL latitudeM;
	int longitudeD;
	DECIMAL longitudeM;
	int fixQuality;
	int gpsNum;
	DECIMAL horDilution;
	DECIMAL altitude;
	DECIMAL geoidHeight;
} NMEA_GGA_t;

__weak void NMEAGGACallback(NMEA_GGA_t* un);
__weak void NMEAWrongChecksum(NMEA_t* un);
void NMEAProcess(NMEA_t* un, uint8_t ch);

#endif
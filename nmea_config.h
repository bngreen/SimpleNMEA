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

#ifndef NMEA_CONFIG_H
#define NMEA_CONFIG_H

#include <stdlib.h>

#define NMEABSIZE 256

#ifdef DHWK
#define __weak __attribute__ ((weak))
#endif

#ifdef USEDOUBLE
	#define DECIMAL double
	#define DECIMALPARSER strtod
#else
	#define DECIMAL float
	#define DECIMALPARSER strtof
#endif

#define NMEAALLOC(a, b) malloc(b)

#endif
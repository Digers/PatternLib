/*
 PatternLib Copyright (C) 2012  Daniel Digerås
 
 PatternLib is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 PatternLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser Public License for more details.
 
 You should have received a copy of the GNU Lesser Public License
 along with PatternLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _H_PATTERN_GEN
#define _H_PATTERN_GEN
#include "private.h"

#pragma mark Public functions

/** distance
 * calculate distance between two values.
 * The special is that the distance is calculated as two
 * angular values.
 *
 * Ex. (min is -32768 and max is 32767)
 *  the values -32768 and 32767 have a distance of 1
 */
c_int distance(c_uint a, c_uint b);

/** dayPatternTick
 *
 *
 */

void dayPatternTick(c_pattern_store * pattern);

#endif

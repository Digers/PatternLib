/*
 PatternLib Copyright (C) 2012  Daniel Digerås
 
 Foobar is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Foobar is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser Public License for more details.
 
 You should have received a copy of the GNU Lesser Public License
 along with PatternLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <time.h>
#include "private.h"
//typedef struct {
//	char second;
//	char minute;
//	char hour;
//} c_time;

void generate_pattern_from_time(c_enable_overlay * pattern, time_t * time);
void calculate_pattern(c_enable_overlay * source, c_enable_overlay * wanted);
void swap_random(c_enable_overlay * overlay, c_enable_overlay * wanted);

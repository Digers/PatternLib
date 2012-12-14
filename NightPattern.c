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

#include "NightPattern.h"

/* start watermark marks the lightness value starting pixels should get */
c_uint start_mark = 5 << 13;

/* Low watermark marks the absolute lowest lightness value */
c_uint low_watermark = 1 << 14;

/* High mark marks the gotovalue once the low watermark is reaced */
c_uint high_mark = 1 << 15;

/* Falloff indicates how much each pixel should decrease each tick */
c_uint falloff = 1 << 5;

/* Stop falloff indicates the fallof rate when a pixel is turned off */
c_uint stop_faloff = 4 << 5;

void nightPatternTick(c_pattern_store * pattern)
{
    int i = 0;
    int probability;
    for (i = 0; i < 27; i++) {
        if(pattern->enable_overlay.states[i] == on)
        {
            pattern->hue.raw[i] = pattern->hue.raw[i] - ((pattern->hue.raw[i] > 128) ? 128 : 0);
            pattern->saturation.raw[i] = ~1;
            if(pattern->lighness.raw[i] != low_watermark)
                probability = rand() % (pattern->lighness.raw[i] - low_watermark);
            else
                probability = 0;
            if((pattern->lighness.raw[i] < low_watermark) || (probability < 5))
                pattern->lighness.raw[i] = high_mark;
            else
                pattern->lighness.raw[i] -= falloff;
        }
        else if(pattern->enable_overlay.states[i] == start)
        {
            pattern->enable_overlay.states[i] = on;
            pattern->saturation.raw[i] = ~1;
            pattern->lighness.raw[i] = start_mark;
        }
        else if (pattern->enable_overlay.states[i] == stop)
        {
            if(pattern->lighness.raw[i] < (stop_faloff + 10))
            {
                pattern->enable_overlay.states[i] = off;
                pattern->lighness.raw[i] = 0;
            }
            else
            {
                pattern->lighness.raw[i] -= stop_faloff;
            }
        }
    }
}
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

#include "interface.h"
#include "private.h"

void testPatternSetup(c_pattern_store * pattern)
{
    int i;
    for (i = 0; i < 3; i++) {
        pattern->ripple_velocity.low_hour[i] = 0;
        pattern->ripple_velocity.low_minute[i] = 0;
        
        pattern->lighness.v.low_hour[i] = -1;
        pattern->lighness.v.low_minute[i] = -1;
        
        pattern->hue.v.low_hour[i] = 0;
        pattern->hue.v.low_minute[i] = 0;
        if(i < 6)
        {
            pattern->ripple_velocity.high_minute[i] = 0;
            pattern->lighness.v.high_minute[i] = -1;
            pattern->hue.v.high_minute[i] = 0;

            if(i < 3)
            {
                pattern->ripple_velocity.high_hour[i] = 0;
                pattern->lighness.v.high_hour[i] = -1;
                pattern->hue.v.high_hour[i] = 0;
            }
        }
    }
    /*
    for (i = 0; i < 8; i++) {
        pattern->lighness.raw[i] = -1 / 2;
    }*/
}
static c_uint carusel = 0;
void testPatternTick(c_pattern_store * pattern, int divider)
{
    
    int i, j = 0;
    
    carusel = carusel + (512);
    
    for (i = 0; i < 27; i++) {
        
        pattern->saturation.raw[i] = -1;
        if (pattern->enable_overlay.states[i] == start)
        {
            pattern->saturation.raw[i] = ~1;
            pattern->enable_overlay.states[i] = on;
        }
        else
        {
            pattern->hue.raw[i] = carusel + (512 * j++);
        }
        
        pattern->lighness.raw[i] = (pattern->enable_overlay.states[i] != off) ? 127 << 8 : 0;
        
        
    }
}
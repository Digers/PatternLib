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

#include <stdio.h>
#include "stdlib.h"
#include "interface.h"
#include "private.h"
#include "color.h"
#include "DayPattern.h"
#include "clockController.h"
#include "TestPattern.h"
#include "NightPattern.h"

/* TODO Remove memset dependency? */
extern void devPatternTick(struct patternData * data);

/* Dummy functions */
struct patternData * patternInit()
{
    struct patternData * data = malloc(sizeof(struct patternData));
    data->config.ticksPerSecond = 60;
    data->config.pixelSwapCycle = 30 * data->config.ticksPerSecond;
    data->config.mode = 0;
    data->internal_struct = malloc(sizeof(struct internal_store));
    memset(data->internal_struct, 0, sizeof(struct internal_store));
    return data;
}

/**
 */
void patternTick (struct patternData * data)
{
    int i;
    struct internal_store * s = (struct internal_store *) data->internal_struct;
    
    s->curr_tick ++;
    
    /* Check if we are at second mark */
    if (!(s->curr_tick % data->config.ticksPerSecond)) {
        time_t ti = time(NULL);
        generate_pattern_from_time(&s->targetPattern, &ti);
        calculate_pattern(&s->patternData.enable_overlay.eo, &s->targetPattern);
    }
    
    /* Check if we should swap pixels */
    if (!(s->curr_tick % data->config.pixelSwapCycle)) {
        printf("Swap\n");
        swap_random(&s->patternData.enable_overlay.eo, &s->targetPattern);
    }
    
    /* Update colors based on mode */
    switch (data->config.mode) {
        case 0:
            testPatternTick(&s->patternData, 30);
            break;
        case 1:
            dayPatternTick(&s->patternData);
            break;
        case 2:
            nightPatternTick(&s->patternData);
            break;
        case 3:
            break;
        default:
            memset(&s->patternData, 0, sizeof(s->patternData));
            break;
    }
    
    /* Recalculate pixel data into RGB */
    if (data->config.mode != 3) {
        for (i = 0; i < 27; i++) {
            c_uint hue = s->patternData.hue.raw[i];
            c_uint lightness = s->patternData.lighness.raw[i];
            c_uint saturation = s->patternData.saturation.raw[i];
        
            hue = hue >> 8;
            lightness = lightness >> 8;
            saturation = saturation >> 8;
        
            hslToRgb(hue, saturation, lightness, &data->colorData.pixels[i]);
        }
    }
    else
    {
        devPatternTick(data);
    }
    
    return;
}

void patternDestroy(struct patternData * data)
{
    /* Todo */
}

/* interface structure */
struct patternInterface pif =
{
    patternInit,
    patternDestroy,
    patternTick
};


struct patternInterface * initPatternLib(void)
{    
    return &pif;
}

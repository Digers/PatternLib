//
//  DevPattern.c
//  PatternLib
//
//  Created by Daniel Digerås on 2012-12-24.
//  Copyright (c) 2012 Daniel Digerås. All rights reserved.
//

#include <stdio.h>

#include "interface.h"
#include "private.h"

#define PIXCOLOR(pixel, r, g, b) {data->colorData.pixels[pixel].red = r; \
                                  data->colorData.pixels[pixel].green = g; \
                                  data->colorData.pixels[pixel].blue = b;}

void devPatternTick(struct patternData * data)
{
    struct internal_store * s = (struct internal_store *) data->internal_struct;
    if (!(s->curr_tick % data->config.ticksPerSecond)) {
        PIXCOLOR(0, 255, 255, 255);
    }
    else
    {
        PIXCOLOR(0, 0, 0, 0);
    }
    
    PIXCOLOR(1, 255, 0, 0);
    PIXCOLOR(2, 0, 255, 0);
    PIXCOLOR(3, 0, 0, 255);
    PIXCOLOR(4, 100, 100, 100);
    PIXCOLOR(5, 255, 255, 255);
    
    {
        int i = 6 + (s->curr_tick / (data->config.ticksPerSecond / 2)) % 21;
        int c = ((s->curr_tick / (data->config.ticksPerSecond / 2)) / 21) % 5;
        switch (c) {
            case 0:
                PIXCOLOR(i, 255, 255, 255);
                break;
            case 1:
                PIXCOLOR(i, 255, 0, 0);
                break;
            case 2:
                PIXCOLOR(i, 0, 255, 0);
                break;
            case 3:
                PIXCOLOR(i, 0, 0, 255);
                break;
                
            default:
                PIXCOLOR(i, 0, 0, 0);
                break;
        }
    }
    
    
    
}
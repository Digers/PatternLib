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

/*Test*/

#ifndef pattern_interface_h
#define pattern_interface_h

struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

union colorData {
    unsigned char raw[3*3*9];
    struct pixel pixels[3*9];
};

struct configuration
{
    int ticksPerSecond;
    int mode;
    int pixelSwapCycle;
};

struct patternData
{
    void * internal_struct;
    union colorData colorData;
    struct configuration config;
};

struct patternInterface
{
    struct patternData * (*init) (void);
    void (*destroy) (struct patternData *);
    void (*tick) (struct patternData *);
};

struct patternInterface * initPatternLib(void);

#endif

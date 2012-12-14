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

#ifndef PatternLib_private_h
#define PatternLib_private_h

typedef short int c_int;
typedef short unsigned int c_uint;

/* Overlay
 * The overlay have four states:
 * off		- The pixel is disabled and is not connected to any
 *			  other pixels.
 * on		- The pixel is subjected to the differential patterns.
 *			  This is normal on-state
 * start	- The pixel is sheduled for beggining to glow. A start
 *			  function sets all initial data and assign a ranom
 *			  color to the pixel
 * stop		- This is the end of the pixels lifetime, the pixel
 *			  is decoupled and faing out...
 */
typedef enum {
	off = 0,
	on = 1,
	start = 2,
	stop = 3
} c_states;

// c_enable_overlay is a storage for the clocks
//    individual pixel states
typedef struct {
	c_states high_hour[3];
	c_states low_hour[9];
	c_states high_minute[6];
	c_states low_minute[9];
} c_enable_overlay;

typedef struct {
	c_int high_hour[3];
	c_int low_hour[9];
	c_int high_minute[6];
	c_int low_minute[9];
} c_data_signed_store;

typedef struct {
	c_uint high_hour[3];
	c_uint low_hour[9];
	c_uint high_minute[6];
	c_uint low_minute[9];
} c_data_unsigned_store;

typedef c_data_signed_store c_velocity;
typedef	c_data_unsigned_store c_value;

/**
 Array organization:
 /----v----v----v
 |  0 |  3 |  6 |
 |    |    |    |
 l----x----x----x
 |  1 |  4 |  7 |
 |    |    |    |
 l----x----x----x
 |  2 |  5 |  8 |
 |    |    |    |
 \----^----^----^
 */

union c_value {
    c_uint raw[27];
    struct {
        c_uint high_hour[3];
        c_uint low_hour[9];
        c_uint high_minute[6];
        c_uint low_minute[9];
    } v;
    
    /*c_data_unsigned_store v;*/
};

//struct to condese all needed data.
typedef struct {
	union c_value hue;
	union c_value lighness;
    union c_value saturation;
	c_velocity velocity;
	c_velocity ripple_velocity;
    union {
        c_states states[27];
        c_enable_overlay eo;
    } enable_overlay;
} c_pattern_store;

/* Private structure */
struct internal_store
{
    int curr_tick;
    int iterations;
    c_enable_overlay targetPattern;
    c_pattern_store patternData;
};

#endif

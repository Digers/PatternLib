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

#include "DayPattern.h"
#include <stdlib.h>

/*
 Private declarations
 */
#pragma mark Private definitions
void array_tick(c_int * hue_velocity, c_int * lightness_velocity
				, c_uint * hue_values, c_uint * lightness_values
                , c_uint * saturation_values
				, c_states * enable_overlay, char vectsize);

void tick_9(int * distvect, c_uint * values, c_states * enable_overlay, char vectsize, char i);
void tick_6(int * distvect, c_uint * values, c_states * enable_overlay, char vectsize, char i);
void tick_3(int * distvect, c_uint * values, c_states * enable_overlay, char vectsize, char i);

c_int distance(c_uint a, c_uint b);
/*c_int distance_ch(c_uint *a, c_uint *b, c_states * overlay, int i);*/
c_int distance_ch(c_uint *value_array, c_states * overlay, c_int a, c_int b);
c_int truncate(int v);

//void prepare_vects(c_velocity * velocity, c_value *values, c_velocity *ripple_velocity, 
//				   c_value *lighness, c_enable_overlay *enable_overlay);

#pragma mark Private data
/*
 Private data
 */
int near_force = 1024;
int far_force = 2048;
int friction = 32; /*32*/
int weight = 1;

const c_states shadowOverlay[] = {on,on,on,on,on,on,on,on,on};
/*const char datavect[] = {0,1,0};*/

#pragma mark Public functions

/*
 Functions
 */

void dayPatternTick(c_pattern_store * pattern){
	array_tick(pattern->velocity.high_hour, pattern->ripple_velocity.high_hour
			   , pattern->hue.v.high_hour, pattern->lighness.v.high_hour
               , pattern->saturation.v.high_hour
			   , pattern->enable_overlay.eo.high_hour, 3);
	array_tick(pattern->velocity.low_hour, pattern->ripple_velocity.low_hour
			   , pattern->hue.v.low_hour, pattern->lighness.v.low_hour
               , pattern->saturation.v.low_hour
			   , pattern->enable_overlay.eo.low_hour, 9);
	array_tick(pattern->velocity.high_minute, pattern->ripple_velocity.high_minute
			   , pattern->hue.v.high_minute, pattern->lighness.v.high_minute
               ,pattern->saturation.v.high_minute
			   , pattern->enable_overlay.eo.high_minute, 6);
	array_tick(pattern->velocity.low_minute, pattern->ripple_velocity.low_minute
			   , pattern->hue.v.low_minute, pattern->lighness.v.low_minute
               , pattern->saturation.v.low_minute
			   , pattern->enable_overlay.eo.low_minute, 9);
}

c_int distance(c_uint b, c_uint a){
	int t;
	const int tipping_point = 1 << 15;
	
	//return ;
	
	if((a-b) < 0)
		t = b - a;
	else
		t = a - b;
	
	if(t > tipping_point)
		return a + ((1 << 16) - b);
	return truncate(a - b);

}

c_int distance_ch(c_uint *value_array, c_states * overlay, c_int a, c_int b){
	if(/*(*/overlay[b] == off/*) || (overlay[b] == stop)*/)
		return 0;
	else
		return distance(value_array[a], value_array[b]);
}

c_int truncate(int v){
	//return ( ((v & 0x7fffffff) >> 16 ) | (v & 0x00007fff) ) & 0x0000ffff;
	if((v < - 0xffff))
		return  (- 0x8000);
	if((v > 0x7fff))
		return 0x7fff;
	//if(v < (c_int)(1 << 15))
	//	return (1 << 15);
	return v;
}

void array_tick(c_int * hue_velocity,c_int  * lightness_velocity
				, c_uint * hue_values, c_uint * lightness_values
                , c_uint * saturation_values
				, c_states * enable_overlay, char vectsize)
{
	char i; // counter
	int hue_dist[vectsize]; // working vector containing distance for hue
	int lightness_dist[vectsize]; // working vector containing distance for lightness 
	void (*add_funct)(int *, c_uint *, c_states *, char, char) = NULL;	
	
	//check wich function to use
	switch (vectsize) {
		case 3: add_funct = &tick_3; break;
		case 6: add_funct = &tick_6; break;
		case 9: add_funct = &tick_9; break;
		default: return;
	}
    
	//preprocessing
	for(i = 0; i < vectsize ; i++){
		if(enable_overlay[i] == start){
			hue_velocity[i] = 0;
			hue_values[i] = random();
			lightness_values[i] = 0;
			lightness_velocity[i] = 100;
			enable_overlay[i] = on;
		}

        saturation_values[i] = ~1;

	}
	
	//processing
	for(i = 0; i < vectsize ; i++){
		hue_dist[i] = 0;
		lightness_dist[i] = 0;
		if(enable_overlay[i] == off)
        {
			continue;
        }
		(*add_funct)(hue_dist, hue_values,  enable_overlay, vectsize, i);
        if(enable_overlay[i] != stop)
            (*add_funct)(lightness_dist, lightness_values,  (c_states *)&shadowOverlay, vectsize, i);
	}
    
    //postprocessing
	for(i = 0; i < vectsize ; i++){
        if (enable_overlay[i] == off) {
            continue;
        }
        if(enable_overlay[i] == on)
            lightness_dist[i] = distance(lightness_values[i], 127 << 8) / 128;
        else if(enable_overlay[i] == stop)
            lightness_dist[i] = -256;
        else
        {}
            /*lightness_dist[i] = 0;*/
        lightness_velocity[i] += truncate(lightness_dist[i])/friction;
        lightness_velocity[i] = truncate(lightness_velocity[i] - lightness_velocity[i] / friction);
        lightness_values[i] += lightness_velocity[i];
        if(enable_overlay[i] == stop && (lightness_values[i] < (1 << 8)))
        {
            enable_overlay[i] = off;
            lightness_values[i] = 0;
        }
		
		hue_velocity[i] = truncate(hue_dist[i] + hue_velocity[i]  - (hue_velocity[i] / friction));
		hue_values[i] += hue_velocity[i];
	}
	
    //randomness
    for (i = 0; i < vectsize; i++) {
        if (hue_velocity[i] > -2 && hue_velocity[i] < 2 && (random() % 256 ) < 5) {
            hue_velocity[i] = 2048;
        }
    }
}

#pragma mark tick distance adder functions
void tick_9(int * dist_val, c_uint * values, c_states * enable_overlay, char vectsize, char i){
	switch (i) {
		case 0:
			dist_val[0] = (distance_ch(values, enable_overlay, 0, 1) / near_force 
						   + distance_ch(values, enable_overlay, 0, 3) / near_force
						   + distance_ch(values, enable_overlay, 0, 4) / far_force);
			break;
		case 1:
			dist_val[1] = (distance_ch(values, enable_overlay, 1, 0) / near_force 
						   + distance_ch(values, enable_overlay, 1, 2) / near_force
						   + distance_ch(values, enable_overlay, 1, 4) / near_force
						   + distance_ch(values, enable_overlay, 1, 3) / far_force
						   + distance_ch(values,  enable_overlay, 1, 5) / far_force) / weight;
			break;
		case 2:
			dist_val[2] = (distance_ch(values, enable_overlay, 2, 1) / near_force 
						   + distance_ch(values, enable_overlay, 2, 5) / near_force
						   + distance_ch(values, enable_overlay, 2, 4) / far_force);
			break;
		case 3:
			dist_val[3] = (distance_ch(values, enable_overlay, 3, 0) / near_force 
						   + distance_ch(values, enable_overlay, 3, 4) / near_force
						   + distance_ch(values, enable_overlay, 3, 6) / near_force
						   + distance_ch(values, enable_overlay, 3, 1) / far_force
						   + distance_ch(values, enable_overlay, 3, 7) / far_force) / weight;
			break;
		case 4:
			dist_val[4] = (distance_ch(values, enable_overlay, 4, 1) / near_force 
						   + distance_ch(values, enable_overlay, 4, 3) / near_force
						   + distance_ch(values, enable_overlay, 4, 5) / near_force
						   + distance_ch(values, enable_overlay, 4, 7) / near_force
						   + distance_ch(values, enable_overlay, 4, 0) / far_force
						   + distance_ch(values, enable_overlay, 4, 2) / far_force
						   + distance_ch(values, enable_overlay, 4, 6) / far_force
						   + distance_ch(values, enable_overlay, 4, 8) / far_force);
			break;
		case 5:
			dist_val[5] = (distance_ch(values, enable_overlay, 5, 2) / near_force 
						   + distance_ch(values, enable_overlay, 5, 4) / near_force
						   + distance_ch(values, enable_overlay, 5, 8) / near_force
						   + distance_ch(values, enable_overlay, 5, 1) / far_force
						   + distance_ch(values, enable_overlay, 5, 7) / far_force) / weight;
			break;
		case 6:
			dist_val[6] = (distance_ch(values, enable_overlay, 6, 3) / near_force 
						   + distance_ch(values, enable_overlay, 6, 7) / near_force
						   + distance_ch(values, enable_overlay, 6, 4) / far_force) / weight;
			break;
		case 7:
			dist_val[7] = (distance_ch(values, enable_overlay, 7, 8) / near_force 
						   + distance_ch(values, enable_overlay, 7, 6) / near_force
						   + distance_ch(values, enable_overlay, 7, 4) / near_force
						   + distance_ch(values, enable_overlay, 7, 3) / far_force
						   + distance_ch(values, enable_overlay, 7, 5) / far_force) / weight;
			break;
		case 8:
			dist_val[8] = (distance_ch(values, enable_overlay, 8, 5) / near_force 
						   + distance_ch(values, enable_overlay, 8, 7) / near_force
						   + distance_ch(values, enable_overlay, 8, 4) / far_force) / weight;
			break;
	}
}
void tick_6(int * dist_val, c_uint * values, c_states * enable_overlay, char vectsize, char i){
	switch (i) {
		case 0:
			dist_val[0] = (distance_ch(values, enable_overlay, 0, 1) / near_force 
						   + distance_ch(values, enable_overlay, 0, 3) / near_force
						   + distance_ch(values, enable_overlay, 0, 4) / far_force);
			break;
		case 1:
			dist_val[1] = (distance_ch(values, enable_overlay, 1, 0) / near_force 
						   + distance_ch(values, enable_overlay, 1, 2) / near_force
						   + distance_ch(values, enable_overlay, 1, 4) / near_force
						   + distance_ch(values, enable_overlay, 1, 3) / far_force
						   + distance_ch(values, enable_overlay, 1, 5) / far_force) / weight;
			break;
		case 2:
			dist_val[2] = (distance_ch(values, enable_overlay, 2, 1) / near_force 
						   + distance_ch(values, enable_overlay, 2, 5) / near_force
						   + distance_ch(values, enable_overlay, 2, 4) / far_force);
			break;
		case 3:
			dist_val[3] = (distance_ch(values, enable_overlay, 3, 0) / near_force 
						   + distance_ch(values, enable_overlay, 3, 4) / near_force
						   + distance_ch(values, enable_overlay, 3, 1) / far_force) / weight;
			break;
		case 4:
			dist_val[4] = (distance_ch(values, enable_overlay, 4, 1) / near_force 
						   + distance_ch(values, enable_overlay, 4, 3) / near_force
						   + distance_ch(values, enable_overlay, 4, 5) / near_force
						   + distance_ch(values, enable_overlay, 4, 0) / far_force
						   + distance_ch(values, enable_overlay, 4, 2) / far_force) / weight;
			break;
		case 5:
			dist_val[5] = (distance_ch(values, enable_overlay, 5, 4) / near_force 
						   + distance_ch(values, enable_overlay, 5, 2) / near_force
						   + distance_ch(values, enable_overlay, 5, 1) / far_force) / weight;
			break;
	}	
}
void tick_3(int * dist_val, c_uint * values, c_states * enable_overlay, char vectsize, char i){
	switch (i) {
		case 0:
			dist_val[0] = (distance_ch(values, enable_overlay, 0, 1) / near_force);
			break;
		case 1:
			dist_val[1] = (distance_ch(values, enable_overlay, 1, 0) / near_force 
						   + distance_ch(values, enable_overlay, 1, 2) / near_force) / weight;
			break;
		case 2:
			dist_val[2] = (distance_ch(values, enable_overlay, 2, 1) / near_force);
			break;
	}
}

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

#include "clockController.h"
#include <stdlib.h>

int align_vect(c_states *source, c_states * wanted, c_uint array_size);
int swap_vect(c_states *source, c_states *wanted, c_uint array_size);

/**
 * match_pattern_random(c_states * pattern, char vectsize, char value)
 * Match the number of lit pixels in patten as the number in value.
 * vectsize tells the size of the pattern vector
 */
void match_pattern_random(c_states * pattern, char vectsize, char value){
	int i;
	char onvects[9];
	char offvects[9];
	char onstates = 0;
	char offstates = 0;
	char randval;
	
	/*
	 Special cases, if value either is zero or the size of patternvector
	 we do not need to individually generate random numbers - everything 
	 is either on or off
	 */
	if(value == 0){
		for (i = 0; i < vectsize; i++) pattern[i] = off;/*stop*/
		return;
	} else if(value >= vectsize){
		for (i = 0; i < vectsize; i++) pattern[i] = on; /*start*/
		return;		
	}
	
	/*calculate the number of on and off pixels.
	 the index of on and off pixels is stored in two
	 seperate vectors.
	*/
	for(i = 0; i < vectsize ;i++){
		if((pattern[i] == on) || (pattern[i] == start)){
			onvects[onstates] = i;
			onstates ++;
		} else {
			offvects[offstates] = i;
			offstates ++;
		}
	}
	
	//If the number of on pixels is the same as our value 
	//do nothing and return
	if(value == onstates)
		return;
	
	//Light random pixels
	else if(value > onstates){
		for(i = value - onstates;i > 0; i--){
            if(offstates)
                randval = random() % offstates;
            else
                randval = 0;
			offstates --;
			pattern[offvects[randval]] = on; /*start*/
			offvects[randval] = offvects[i];
		}
	}
	
	//Turn off random pixels
	else if(value < onstates){
		for(i = offstates - value;i > 0; i--){
            if (onstates)
                randval = random() % onstates;
            else
                randval = 0;
			onstates --;
			pattern[onvects[randval]] = off; /*stop*/
			onvects[randval] = onvects[i];
		}
	}	
}

/**
 * generate_pattern_from_time(c_enable_overlay * pattern, time_t * time)
 * create random pattern from time. The function does only update the pattern,
 * not create a new one. Thus if the pattern have right number of pixels lit
 * nothing further will happen.
 */
void generate_pattern_from_time(c_enable_overlay * pattern, time_t * time){
	struct tm * ts;
	signed char temp;

	ts = localtime(time);
    
    /* Test code */
#if 0
    if (ts->tm_min % 2) {
        ts->tm_hour = 0;
        ts->tm_min = 0;
    }
#endif
	
	temp = ts->tm_hour / 10;
	match_pattern_random(pattern->high_hour, 3, temp);
	temp = ts->tm_hour % 10;
	match_pattern_random(pattern->low_hour, 9, temp);
	
	temp = ts->tm_min / 10;
	match_pattern_random(pattern->high_minute, 6, temp);
	temp = ts->tm_min % 10;
	match_pattern_random(pattern->low_minute, 9, temp);
	
	
}


void calculate_pattern(c_enable_overlay * source, c_enable_overlay * wanted){
#define patternGen_Use_Old_Routine
#ifndef patternGen_Use_Old_Routine
	char states[27];
	
#endif
#ifdef patternGen_Use_Old_Routine
	align_vect(source->high_hour, wanted->high_hour, 3);
	align_vect(source->low_hour, wanted->low_hour, 9);
	align_vect(source->high_minute, wanted->high_minute, 6);
	align_vect(source->low_minute, wanted->low_minute, 9);
	
#endif
}

void swap_random(c_enable_overlay * overlay, c_enable_overlay * wanted)
{
    int r = rand() % 6;
    switch (r) {
        case 0: swap_vect(overlay->high_hour, wanted->high_hour, 3); break;
        case 1: swap_vect(overlay->low_hour, wanted->low_hour, 9); break;
        case 2: swap_vect(overlay->high_minute, wanted->high_minute, 6); break;
        case 3: swap_vect(overlay->low_minute, wanted->low_minute, 9); break;
        default:
            /* Some chance to not do anything at all */
            break;
    }
}

#pragma mark Private functions

int swap_vect(c_states *source, c_states *wanted, c_uint array_size)
{
    int onvects[9];
    int offvects[9];
    int onnum = 0, offnum = 0;
    int i;
    
    for (i=0; i < array_size; i++) {
        if(source[i] == on)
        {
            onvects[onnum] = i;
            onnum ++;
        } else if(source[i] == off)
        {
            offvects[offnum] = i;
            offnum ++;
        }
    }
    
    /* Check for possibility to swap */
    if (onnum == 0 || onnum == array_size) {
        return -1;
    }
    
    if(offnum == 0)
        return -2;
    
    /* swap */
    i = rand() % onnum;
    source[onvects[i]] = stop;
    wanted[onvects[i]] = off;
    
    i = rand() % offnum;
    source[offvects[i]] = start;
    wanted[offvects[i]] = on;
    
    return 0;
}

int align_vect(c_states *source, c_states * wanted, c_uint array_size){
	char unaligned;
	char states[9];
	char rand;
	char i;
	int retnum = 0;
	
	//check vector
	unaligned = 0;
	for(i = 0; i < array_size; i++){
		if(source[i] != wanted[i]){
			if((source[i] == stop) && (wanted[i] == off))
				continue;
			else if((source[i] == start) && (wanted[i] == on))
				continue;
			states[unaligned] = i;
			unaligned ++;
		}
	}
	if(unaligned != 0){
		rand = random() % unaligned;
		/*if(source[states[rand]] == on || source[states[rand]] == start)*/
        if(wanted[states[rand]] == off)
			source[states[rand]] = stop;
		else
			source[states[rand]] = start;
	}
	return retnum;
}
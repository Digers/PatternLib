/*
 *  color.h
 *  pattern
 *
 *  Created by Daniel Digerås on 2009-05-24.
 *  The contents of this file is public domain
 *
 */

#ifndef _H_COLOR
#define _H_COLOR
#include "interface.h"

typedef struct {
	int hue;
	int saturation;
	int luminance;
} hsl_color;

void hslToRgb(int hue, int saturation, int luminance, struct pixel * rgbVect);

#endif
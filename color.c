/*
 *  color.c
 *
 *  Created by Daniel Digerås on 2009-05-24.
 *  The contents of this file is public domain
 *
 */

#include "color.h"

/**
 Convert from hsl to rgb color space
 */
void hslToRgb(int hue, int saturation, int luminance, struct pixel * rgbVect)
{
    int v;
	
    v = (luminance < 128) ? (luminance * (256 + saturation)) >> 8 :
	(((luminance + saturation) << 8) - luminance * saturation) >> 8;
    if (v <= 0) {
        rgbVect->red = rgbVect->green = rgbVect->blue = 0;
    } else {
        int m;
        int sextant;
        int fract, vsf, mid1, mid2;
		
        m = luminance + luminance - v;
        hue *= 6;
        sextant = hue >> 8;
        fract = hue - (sextant << 8);
        vsf = v * fract * (v - m) / v >> 8;
        mid1 = m + vsf;
        mid2 = v - vsf;
        switch (sextant) {
			case 0: rgbVect->red = v; rgbVect->green = mid1; rgbVect->blue = m; break;
			case 1: rgbVect->red = mid2; rgbVect->green = v; rgbVect->blue = m; break;
			case 2: rgbVect->red = m; rgbVect->green = v; rgbVect->blue = mid1; break;
			case 3: rgbVect->red = m; rgbVect->green = mid2; rgbVect->blue = v; break;
			case 4: rgbVect->red = mid1; rgbVect->green = m; rgbVect->blue = v; break;
			case 5: rgbVect->red = v; rgbVect->green = m; rgbVect->blue = mid2; break;
        }
    }
}

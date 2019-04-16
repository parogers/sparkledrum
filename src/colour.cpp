/*     
 * sparkledrum - Flashes lights in time with a beating drum
 * Copyright (C) 2018  Peter Rogers (peter.rogers@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "colour.h"

void hsv2rgb(
    uint16_t hue, byte saturation, byte value,
    byte *red, byte *green, byte *blue)
{
    unsigned int p, q, t, ff;

    if (saturation == 0)
    {
        *red = value;
        *green = value;
        *blue = value;
        return;
    }
    hue %= 360;
    int quad = hue/60;

    ff = (byte)((hue % 60) * 4.25);
    
    p = (value * (255 - saturation)) >> 8;
    q = (value * (255 - ((saturation * ff) >> 8))) >> 8;
    t = (value * (255 - ((saturation * (255 - ff)) >> 8))) >> 8;

    switch(quad) {
    case 0:
        *red = value;
        *green = t;
        *blue = p;
        break;

    case 1:
        *red = q;
        *green = value;
        *blue = p;
        break;
    
    case 2:
        *red = p;
        *green = value;
        *blue = t;
        break;

    case 3:
        *red = p;
        *green = q;
        *blue = value;
        break;
    
    case 4:
        *red = t;
        *green = p;
        *blue = value;
        break;

    default:
        *red = value;
        *green = p;
        *blue = q;
        break;
    }
}


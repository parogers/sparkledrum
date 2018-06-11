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

#include <Adafruit_NeoPixel.h>

#define STRIP_PIN           PB3
#define SENSOR_PIN          A2
#define NUM_LEDS            12
#define SENSOR_THRESHOLD    10
#define SENSOR_COOLDOWN     75

/* Globals */

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PB3, NEO_GRB | NEO_KHZ800);

float amplitude = 0;
uint32_t cooldown = 0;
float pos = 0;

/* Main */

void setup() 
{
    strip.begin();
    strip.show();
}

void loop() 
{
    bool reading = analogRead(SENSOR_PIN) > SENSOR_THRESHOLD;

    if (reading && millis() > cooldown) {
        amplitude = 1;
        cooldown = millis() + SENSOR_COOLDOWN;
    }

    // Have the strip colour vary slightly over time
    byte peak_red = 150+80*abs(sin(millis()/500.0));
    byte peak_green = 50*abs(sin(millis()/500.0));
    byte peak_blue = 50+50*abs(sin(millis()/800.0));

    for (int n = 0; n < NUM_LEDS; n++) 
    {
        // Have the strip peak in the middle, and fade out at the ends
        // (ie strip cell brightness is mapped onto a half wavelength sine curve)
        float off = PI*n/(NUM_LEDS);
        float s = abs(sin(pos+off));

        // Squaring the result has the affect of smoothing out the half sine curve
        // when the value of 's' is closer to zero.
        s *= s;

        byte red = (byte)(s*peak_red*amplitude);
        byte green = (byte)(s*peak_green*amplitude);
        byte blue = (byte)(s*peak_blue*amplitude);

        strip.setPixelColor(n, strip.Color(red, green, blue));
    }
    strip.show();

    // Keep the sine wave moving across the strip 
    // (somewhat slower when the light is dimmer)
    pos += 0.02*(1+amplitude/200.0);

    // Have the pulse fade out over time
    amplitude *= 0.97;
}


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

/*
 * attiny85 pinout:
 * 
 *               +------+
 *          RST  |  \/  | PWR
 *    (LED) PB3  |      | PB2 (BTN)
 *    (MIC)  A2  |      | PB1
 *          GND  |      | PB0
 *               +------+
 */

#include <Adafruit_NeoPixel.h>
#include "colour.h"
#include "fsine.h"

#define STRIP_PIN           2
#define MIC_PIN             A2
#define NUM_LEDS            15

/* Globals */

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STRIP_PIN, NEO_GRB | NEO_KHZ800);


class StripAnimation 
{
public:
    uint32_t last_time = 0;
    float amplitude;
    float pos = 0;
    float speed;

    StripAnimation() {
        last_time = millis();
    }

    void update() 
    {
        uint32_t now = millis();

        // Have the strip colour vary slightly over time
        byte peak_red, peak_green, peak_blue;
        hsv2rgb(
            210+70*sin(now/800.0), 200, 200, 
            &peak_red, &peak_green, &peak_blue);
    
        float f_red = peak_red * amplitude;
        float f_green = peak_green * amplitude;
        float f_blue = peak_blue * amplitude;
    
        f_red += (1-amplitude)*8;
        f_green += (1-amplitude)*4*(1+sin(now/500.0));
        f_blue += (1-amplitude)*8;
        
        float step = 2*PI/NUM_LEDS;
        float off = 0;
        
        for (int n = 0; n < NUM_LEDS; n++) 
        {
            // Have the strip peak in the middle, and fade out at the ends
            // (ie strip cell brightness is mapped onto a half wavelength sine curve)
            off += step;
            float s = sin(pos+off);

            if (s < 0) s = -s;
    
            // Raising the result to a power has the affect of smoothing out the half sine curve
            // when the value of 's' is closer to zero.
            //s = s*s;
    
            byte red = (byte)(s*f_red);
            byte green = (byte)(s*f_green);
            byte blue = (byte)(s*f_blue);
    
            strip.setPixelColor(n, red+1, green+1, blue+1);
        }
        strip.show();
    
        // Keep the sine wave moving across the strip 
        // (somewhat slower when the light is dimmer)
        //pos += 0.01*(0.5+2.5*amplitude);

        float dt = (now - last_time)/1000.0;

        pos += speed*dt;

#if 0
        // Have the pulse fade out over time according to an exponential decay curve
        // 
        // y = y0*e^(Bx)
        // y' = y0*B*e^(Bx)
        // y' = B*y
        //
        // So: y(t+dt) = y(t) + y'(t)*dt = y(t) + B*y*dt
        //
        if (amplitude > 0) {
            amplitude -= 4*amplitude*dt;
        }
#endif
        last_time = now;
    }
};


StripAnimation anim;


/* Main */

void setup() 
{
    Serial.begin(9600);
    strip.begin();
    strip.show();
}

uint32_t next_time = 0;
int num_samples = 0;
uint8_t count = 0;

void loop() 
{
    int reading = analogRead(MIC_PIN);
    num_samples++;

    uint32_t now = millis();

    if (now > next_time) {
        Serial.println(num_samples);
        num_samples = 0;
        next_time = millis() + 1000;
    }
    
    /* TODO - handle drum strikes */
    /* ... */

    anim.amplitude = 1;
    anim.speed = 4;

    // Make only occasional updates to the strip so we can spend more time
    // monitoring for drum strikes.
    count++;
    if (count % 4 == 0) {
        anim.update();
    }
}


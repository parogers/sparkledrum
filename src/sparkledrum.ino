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

#include "animation.h"
#include "fsine.h"

#define MIC_PIN             A2

/* Globals */

/* Main */

void setup() 
{
    animation.begin();
    Serial.begin(9600);
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

    animation.amplitude = 1;
    animation.speed = 4;

    // Make only occasional updates to the strip so we can spend more time
    // monitoring for drum strikes.
    count++;
    if (count % 10 == 0) {
        animation.update();
    }

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
}

